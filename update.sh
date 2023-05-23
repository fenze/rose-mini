#!/bin/sh -e

[ -f /var/lib/mini/packages/$1 ] || exit 1

. /var/lib/mini/packages/$1
. /usr/lib/mini/mpm
update
