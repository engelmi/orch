#!/usr/bin/env python
# SPDX-License-Identifier: MIT-0
#
# vim:sw=4:ts=4:et
from bluechi.ext import Unit

result = Unit("my-node-name").start_unit("chronyd.service")
print(result)
