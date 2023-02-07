#include <errno.h>
#include <getopt.h>
#include <stdlib.h>

#include "libhirte/common/common.h"
#include "libhirte/common/opt.h"
#include "libhirte/log/log.h"

#include "client.h"

const struct option options[] = { { ARG_HELP, no_argument, 0, ARG_HELP_SHORT }, { NULL, 0, 0, '\0' } };

#define OPTIONS_STR ARG_HELP_SHORT_S

static char *op;
static char **opargv;
static int opargc;
static bool no_action = false;

static void usage(char *argv[]) {
        hirte_log_errorf("Usage: %s COMMAND ... \n", argv[0]);
}

static int get_opts(int argc, char *argv[]) {
        int opt = 0;

        while ((opt = getopt_long(argc, argv, OPTIONS_STR, options, NULL)) != -1) {
                if (opt == ARG_HELP_SHORT) {
                        usage(argv);
                        no_action = true;
                        return 0;
                } else {
                        hirte_log_errorf("Unsupported option %c\n", opt);
                        usage(argv);
                        return -EINVAL;
                }
        }

        if (optind < argc) {
                op = argv[optind++];
                opargv = &argv[optind];
                opargc = argc - optind;
        } else {
                hirte_log_error("No command given\n");
                usage(argv);
                return -EINVAL;
        }

        return 0;
}

int main(int argc, char *argv[]) {
        int r = 0;
        _cleanup_sd_bus_ sd_bus *bus = NULL;

        hirte_log_init();

        r = get_opts(argc, argv);
        if (r < 0) {
                return EXIT_FAILURE;
        }
        if (no_action) {
                return EXIT_SUCCESS;
        }

        hirte_log_set_log_fn(hirte_log_to_journald_with_location);

        _cleanup_client_ Client *client = new_client(op, opargc, opargv);
        r = client_call_manager(client);
        if (r < 0) {
                hirte_log_errorf("Call to manager failed: %s", strerror(-r));
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}