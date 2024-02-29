

SRCFILES= $(shell ./listfiles.sh)

amak: amak.c
	$(CC) -o $@ $^

amak.c: inc.c listfiles.sh $(SRCFILES)
	cat $(SRCFILES) > $@

data.c: data/* mkdata
	./mkdata

mkdata: mkdata.c
	$(CC) -o $@ $^
