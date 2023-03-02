#!/usr/bin/bash -e

# Checked files regex
CHECKED_FILES="*.[c|h]"

#
# List of licenses which are OK when found in a source code
#
APPROVED_LICENSES=""


result=0
found_files="$(find -type f -name ${CHECKED_FILES} -not -path './builddir/*')"

for f in ${found_files} ; do
    # scan for license only within the 1st 5 lines of each file
    license_found=$(head -n 5 ${f} | grep -Po '.*SPDX-License-Identifier\: \K\S+' || echo "-1")
    if [ "${license_found}" == "-1" ]; then
        result=1
        echo "File '${f}' does not contain SPDX header!"
    else
        valid_license=1
        for l in ${APPROVED_LICENSES} ; do
            if [ "${license_found}" == "$l" ] ; then
                valid_license=0
            fi
        done
        if [ "${valid_license}" == "1" ] ; then
            result=1
            echo "File '${f}' contains unapproved license '${license_found}'"
        fi
    fi
done

exit ${result}
