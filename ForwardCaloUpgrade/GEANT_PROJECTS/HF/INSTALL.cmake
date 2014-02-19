
Install using cmake

- Setup your GEANT environment, find your GEANT installation.

> source $G4INSTALL/bin/geant4.sh

- Setup your ROOT environment if it is not yet done

> export ROOTSYS=<your root location>
> export PATH=${PATH}:${ROOTSYS}/bin
> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib

- Setup your HepMC environment if it is not yet done

> export HEPMC_DIR=/data/CMS/upgrade/FullSIM/HepMC
> export HEPMC_INCLUDE_DIR=/data/CMS/upgrade/FullSIM/HepMC/include
> export HEPMC_LIBRARY_DIR=/data/CMS/upgrade/FullSIM/HepMC/lib
> export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HEPMC_LIBRARY_DIR

- Make another directory outside the ./HF directory , e.g. ../HF_build

- Goto e.g. ../HF_build, and run:

> cmake -DGeant_DIR=$G4INSTALL/lib/Geant4-9.6.1 ../HF
> make or gmake

- To run: 

> ./testBeam -o out.root b.mac
