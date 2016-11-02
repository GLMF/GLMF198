PACKAGE=cb-mastermind
DESTDIR=/usr/share/icons/hicolor

PNG:=$(wildcard *.png)
SIZE:=$(PNG:.png=)

all: install
install:
	@for f in $(SIZE); do \
		if [ ! -f $(DESTDIR)/$$f/apps/$(PACKAGE).png ]; then \
		 cp -v $$f.png $(DESTDIR)/$$f/apps/$(PACKAGE).png; \
		fi \
	done
uninstall:
	@for f in $(SIZE); do \
		if [ -f $(DESTDIR)/$$f/apps/$(PACKAGE).png ]; then \
		 rm -vf $(DESTDIR)/$$f/apps/$(PACKAGE).png; \
		fi \
	done
