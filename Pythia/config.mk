SHELL = /bin/sh
ARCH = Linux-gcc4
MYLCGPLATFORM = x86_64-slc5-gcc43-opt
SHAREDLIBS = yes
FC = gfortran
CC = gcc
FFLAGS = -O2
CFLAGS = -O2
CXXFLAGS = -O2 -ansi -pedantic -W -Wall -Wshadow -O2 -fPIC -Wall
FLIBS = -lgfortran -lgfortranbegin
LDFLAGSSHARED = -O2 -ansi -pedantic -W -Wall -Wshadow -fPIC -shared
LDFLAGLIBNAME = -Wl,-soname
SHAREDSUFFIX = so
INSTALLDIR = /afs/cern.ch/sw/lcg/external/MCGenerators/pythia8/170/x86_64-slc5-gcc43-opt
DATADIR = /afs/cern.ch/sw/lcg/external/MCGenerators/pythia8/170/share

ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --glibs)

PYTHIA8LOCATION = $(HOME)/extern
LHAPDFVERSION = 5.7.0
LHAPDFLOCATION = $(HOME)/extern/lib
LHAPDFLIBNAME = -lLHAPDF

HEPMCVERSION = 2.05.01
HEPMCLOCATION = $(HOME)/extern

