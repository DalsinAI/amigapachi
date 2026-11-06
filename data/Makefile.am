group = games
perms = 666

datdir = $(DATAPATH)/data
scoredir = $(SCOREPATH)/data

dat_DATA = \
backs.dat \
monsters.dat \
objects_v2.dat \
rooms_v2.dat

score_DATA = \
scores.dat

EXTRA_DIST = $(dat_DATA) $(score_DATA)

install-data-local:
	for file in $(score_DATA); do\
	    chgrp $(group) $(scoredir)/$$file; \
	    chmod $(perms) $(scoredir)/$$file; \
	done					    
