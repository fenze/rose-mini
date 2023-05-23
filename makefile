include config.mk

mini: mini.c
	$(CC) $(CFLAGS) -o $@ $? -lcurl

install: mini
	mkdir -p /var/lib/mini/packages
	mkdir -p $(LIBDIR)
	cp -f mini $(BINDIR)
	cp -f update.sh $(LIBDIR)/update
	chmod 700 $(LIBDIR)/update
	cp -f install.sh $(LIBDIR)/install
	chmod 700 $(LIBDIR)/install
	cp -f remove.sh $(LIBDIR)/remove
	chmod 700 $(LIBDIR)/remove
	cp -f mpm.sh $(LIBDIR)/mpm
	cp -f mini.1 /usr/share/man/man1
	cp -f mini.conf.5 /usr/share/man/man5

uninstall:
	$(RM) -r $(BINDIR)/mini \
		$(LIBDIR) /var/lib/mini \
		/usr/share/man/man5/mini.conf.5 \
		/usr/share/man/man1/mini.1

clean:
	$(RM) mini

.PHONY: install uninstall clean
