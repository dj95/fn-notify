# fn-notify 
# See LICENSE file for copyright and license details.

include config.mk

SRC = fn-notify.c
OBJ = ${SRC:.c=.o}

all: fn-notify

options:
	@echo dmenu build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC -c $<
	@${CC} -c $< ${CFLAGS}

${OBJ}: config.mk

sdash: fn-notify.o 
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f fn-notify.o

.PHONY: all options clean
