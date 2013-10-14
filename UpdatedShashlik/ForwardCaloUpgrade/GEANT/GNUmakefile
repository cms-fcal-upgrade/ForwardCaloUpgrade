# --------------------------------------------------------------
# Makefile for building G4 Root aplication
# --------------------------------------------------------------

name := fcalor
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

##export ROOTSYS=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.30.00/x86_64-slc5-gcc43-opt/root
export HEPMC_DIR=/storage/6/work/aackert/ecalupgrade/HepMC-2.06.08/x86_64-slc5-gcc43-opt/

include $(G4INSTALL)/config/binmake.gmk

ifdef HEPMC_DIR
  INCFLAGS  += -I$(HEPMC_DIR)/include
  LDLIBS    += -L$(HEPMC_DIR)/lib -lHepMC
endif

ifdef ROOTSYS
  CPPFLAGS += -I$(shell $(ROOTSYS)/bin/root-config --incdir)
  LDLIBS   += $(shell $(ROOTSYS)/bin/root-config --libs)
endif

ifeq ($(G4SYSTEM),Linux-g++)
  LDLIBS   += -Wl,-rpath,$(shell $(ROOTSYS)/bin/root-config --libdir)
endif

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

