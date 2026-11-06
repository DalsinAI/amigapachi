group = games
perms = 666

datdir = $(DATAPATH)/data
scoredir = $(SCOREPATH)/$(PACKAGE)/data

dat_DATA = \
backs.dat \
monsters.dat \
objects_v2.dat \
rooms_v2.dat

score_DATA = \
scores.dat

EXTRA_DIST = $(dat_DATA) $(score_DATA)

install-data-local:
#	for file in $(score_DATA); do\
#	    chgrp $(group) $(DESTDIR)$(scoredir)/$$file; \
#	    chmod $(perms) $(DESTDIR)$(scoredir)/$$file; \
#	done					    
