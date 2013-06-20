#
# Examples Makefile.
#
#                  M. Kirsanov 07.04.2006
#                     Modified 18.11.2006
#                     26.03.2008 CLHEP dependency removed

SHELL = /bin/sh

-include config.mk
ifeq (x$(PYTHIA8LOCATION),x)
 PYTHIA8LOCATION=..
endif
##-include $(PYTHIA8LOCATION)/config.mk
-include config.mk

# Location of directories.
TOPDIR=$(shell \pwd)
INCDIR=include
SRCDIR=src
LIBDIR=lib
LIBDIRARCH=lib/archive
BINDIR=bin

# Libraries to include if GZIP support is enabled
ifeq (x$(ENABLEGZIP),xyes)
LIBGZIP=-L$(BOOSTLIBLOCATION) -lboost_iostreams -L$(ZLIBLOCATION) -lz
endif

# There is no default behaviour, so remind user.
all:
	@echo "Usage: for NN = example number: make mainNN"

# Create an executable linked to HepMC (if all goes well).
# Owing to excessive warning output -Wshadow is not used for HepMC.
ifneq (x$(HEPMCLOCATION),x)
  JetsGen ParticleGunGen MinbiasGen: \
	$(PYTHIA8LOCATION)/$(LIBDIRARCH)/libpythia8.a $(PYTHIA8LOCATION)/$(LIBDIRARCH)/libhepmcinterface.a
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -Wno-shadow -I$(PYTHIA8LOCATION)/$(INCDIR) -I$(HEPMCLOCATION)/include \
	$@.cc -o $(BINDIR)/$@.exe \
	-L$(PYTHIA8LOCATION)/$(LIBDIRARCH) -lpythia8 -llhapdfdummy $(LIBGZIP) \
	-lhepmcinterface \
	-L$(HEPMCLOCATION)/lib -lHepMC
	@ln -fs $(BINDIR)/$@.exe $@.exe
else
  JetsGen:
	@echo ERROR, this target needs HepMC, variable HEPMCLOCATION
endif

# Clean up: remove executables and outdated files.
.PHONY: clean
clean:
	rm -rf $(BINDIR)
	rm -rf *.exe
	rm -f *~; rm -f \#*; rm -f core*
