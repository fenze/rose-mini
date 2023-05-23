PACKAGE_REPOSITORY := https://raw.githubusercontent.com/fenze/rose-packages/main/

PREFIX := /usr
BINDIR := $(PREFIX)/bin
LIBDIR := $(PREFIX)/lib/mini
CFLAGS += -Ofast -DREPOSITORY=\"$(PACKAGE_REPOSITORY)\"
