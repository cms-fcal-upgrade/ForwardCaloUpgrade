#!/bin/sh

source /afs/cern.ch/sw/lcg/external/gcc/4.3.2/x86_64-slc5/setup.sh

if [ ! $?LD_LIBRARY_PATH ]; then
  export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/HepMC/2.05.01/x86_64-slc5-gcc43-opt/lib
else
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/afs/cern.ch/sw/lcg/external/HepMC/2.05.01/x86_64-slc5-gcc43-opt/lib
fi
if [ ! $?LD_LIBRARY_PATH ]; then
  export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.7/x86_64-slc5-gcc43-opt/lib
else
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.7/x86_64-slc5-gcc43-opt/lib
fi
export LHAPATH=/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.7/x86_64-slc5-gcc43-opt/../share/PDFsets
if [ ! $?LD_LIBRARY_PATH ]; then
  export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/fastjet/2.4.3/x86_64-slc5-gcc43-opt/lib
else
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/afs/cern.ch/sw/lcg/external/fastjet/2.4.3/x86_64-slc5-gcc43-opt/lib
fi
export PYTHIA8DATA=/afs/cern.ch/sw/lcg/external/MCGenerators/pythia8/170/x86_64-slc5-gcc43-opt/xmldoc
if [ ! $?LD_LIBRARY_PATH ]; then
  export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.7.0/x86_64-slc5-gcc43-opt/lib/archive
else
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.7.0/x86_64-slc5-gcc43-opt/lib/archive
fi

export LHAPATH=/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.7.0/share/PDFsets
