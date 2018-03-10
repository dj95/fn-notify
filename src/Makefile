# fn-notify 
# See LICENSE file for copyright and license details.

include config.mk

SRC = fn-notify.c draw.c
OBJ = ${SRC:.c=.o}

all: fn-notify

options:
	@echo fn-notify build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.o: $(SRCS)
	@echo CC -o $@
	$(CC) $(CFLAGS) $< -o $@

${OBJ}: config.mk

fn-notify: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f ${OBJ}

.PHONY: all options clean
