#!/bin/sh -e

[ -f /tmp/mini ] || exit 1

mv /tmp/mini /var/lib/mini/packages/$1
. /var/lib/mini/packages/$1
. /usr/lib/mini/mpm
install
