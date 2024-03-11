CC := gcc
SRCDIR := ./src
INCDIR := ./include
SRC := $(wildcard $(SRCDIR)/*.c)
OUTDIR := ./out
CFLAGS := -Wall -Wextra -Werror -g3 -I$(INCDIR)
DFLAGS := -g
LDFLAGS := -lncurses

all: $(OUTDIR)/htop

debug: $(OUTDIR)/dhtop

$(OUTDIR)/htop: $(SRC) | $(OUTDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OUTDIR)/dhtop: $(SRC) | $(OUTDIR)
	$(CC) $(DFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OUTDIR):
	mkdir -p $@

.PHONY: all debug clean fclean run

clean:
	rm -rf $(OUTDIR)

fclean: clean

run: $(OUTDIR)/htop
	./$<
