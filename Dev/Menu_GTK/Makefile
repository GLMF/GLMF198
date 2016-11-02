PACKAGE=cb-mastermind
VERSION=1.0
COPYRIGHT=IUT de Beziers
EMAIL=cb@iutbeziers.fr

CC:=gcc
BASENAME:=$(shell basename $(PWD))
CFLAGS_GTK:=$(shell pkg-config --cflags gtk+-3.0)
LDFLAGS_GTK:=$(shell pkg-config --libs gtk+-3.0)

CSRCS:=$(wildcard [^lib]*.c)
CEXES:=$(CSRCS:.c=)
LIB:=lib-mastermind.a
LIBOBJS:=lib-gtk-utils.o lib-mastermind.o lib-mastermind-check.o

CFLAGS:=-g -O2 -Wall $(CFLAGS_GTK)
LDFLAGS:=$(LIB) $(LDFLAGS_GTK)

all: $(LIB) $(CEXES)
$(LIB): $(LIBOBJS)
	ar -crv $@ $^
install:
	@make -C po install
	@make -C icons install
uninstall:
	@make -C po uninstall
	@make -C icons uninstall
po: $(PACKAGE).pot
$(PACKAGE).pot: gtk3-menus-11.c gtk3-menus-40.c lib-mastermind.c
	xgettext --sort-output --no-wrap --escape --keyword=_ \
		--from-code utf-8 \
		--copyright-holder="$(COPYRIGHT)" \
		--msgid-bugs-address="$(EMAIL)" \
		--package-name="$(PACKAGE)" \
		--package-version="$(VERSION)" \
		-o $@ $?
tar: clean
	@cd ..;tar cvJf $(BASENAME).tar.xz --exclude=.directory --exclude=*.odt --exclude=*figure*.png $(BASENAME)

.c:
	$(CC) $< $(CFLAGS) -o $@ $(LDFLAGS)
.c.o:
	$(CC) -c $< $(CFLAGS) -o $@

clean_bak:
	@rm -vf *~ *.bak *.org
clean_data: clean_bak
	@rm -vf *.dat *.stackdump core
clean: clean_data
	@rm -vf $(CEXES) $(LIB) $(LIBOBJS) $(PACKAGE).pot
	@make -C po clean
