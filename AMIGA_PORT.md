Pachi el marciano 1.0 for AmigaOS 3.x
=====================================

Pachi el marciano (Santiago and Nicolas Radeff, Dragon Technologies;
music by Peter "Skaven" Hajba), the platform game: Pachi the Martian
gathers the objects in each room of a haunted world and finds the way out.
For AmigaOS 3.x on a 68040 or 68060 with an FPU and an RTG screen, on
OpenGPU's SDL 2.

Base: pachi_1.0.orig.tar.gz from Debian (sha256
134a51773d5e441dc31aed4e57b3543afdefe2d8efedeaa05acb85cac0fa9c52), with
Debian's patches 1.0-17 (source-fixes, scorefile and sdl2-migration, which
moves it from SDL 1.2 to SDL 2) as the second commit. Branch
team2/pachi-amiga.

Status: builds; not yet run in the lab.


What you need
-------------

- A 68040 or 68060 with an FPU, AmigaOS 3.x, an RTG card (or the
  AmigaChrome lab).
- GFOpenUp 0.8.18 or later: LIBS:OpenGPU/SDL2.module and SDL2_mixer.module.


How to play
-----------

Open the Pachi icon, or run Pachi from a Shell. The game reads its data
from the drawer's data/ and keeps its high scores in data/pachi.scores.


Building
--------

    make -f amiga/Makefile.amigaos dist

with GFOpenAmigaGCC 16.2.0-open4 and the OpenGPU kit (see the Makefile's
head). The drawer is dist/Pachi (and dist/Pachi.info).


What was changed
----------------

- amiga/config.h (configure is not run): DATADIR and SCOREDIR are
  PROGDIR:data; the high-score file is made from data/scores.dat, as
  Debian's package does.
- amiga/stack.c: 512 KB of stack.


Licences
--------

Pachi el marciano, its pictures and its music: GPL 2 or later (COPYING.txt).
The Amiga files (amiga/): GPL 2 or later, the AmiCradle Team.
