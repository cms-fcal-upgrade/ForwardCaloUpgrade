# $Id: GNUmakefile,v 1.2 2003-01-23 15:31:39 maire Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := testBeam
G4TARGET := $(name)
G4EXLIB := true

CPPFLAGS := `root-config --cflags` -I/uscms/home/c0wd3n/nobackup/softwares/HepMC-2.06.09
LDFLAGS := `root-config --libs`  -L/uscms/home/c0wd3n/nobackup/softwares/HepMC-build/lib -lHepMC -L${G4WORKDIR}/lib -lmyVecDict_h

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
