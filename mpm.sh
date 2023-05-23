#!/bin/sh

. /etc/mini.conf

[ "$CC" ] || CC=cc
[ "$PREFIX" ] || PREFIX="/usr"
[ "$CFLAGS" ] || CFLAGS="-O3"
[ "$LDFLAGS" ] || LDFLAGS="-s"
[ "$MAKEFLAGS" ] || MAKEFLAGS="-j$(nproc)"

error() {
	printf "[\033[31m$NAME\033[m]: "
	printf "%s\n" "$*"
}

warn() {
	printf "[\033[33m$NAME\033[m]: "
	printf "%s\n" "$*"
}

log() {
	printf "[\033[32m$NAME\033[m]: "
	printf "%s\n" "$*"
}

die() {
	error "$*"
	exit 1
}

run() {
	printf '  \033[34m > \033[m%s\n' "$*"
	"$@" > /dev/null || return "${_ret:=0}"
}
