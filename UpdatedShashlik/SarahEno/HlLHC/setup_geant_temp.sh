#/bin/csh -v
#  Proto-script to setup environment to use CERN /afs binary releases
#
#  First version: 12 April 2002, J.A.
#  Modified:      24 May   2002, J.A.
#  Modified:      16 June  2003, I.M. 
#  Modified:      18 March 2004, I.M. 
#  Version 0.4.0
#                                       Eventual CVS ver no: $Id: setup_geant_temp.sh,v 1.1 2012/01/09 22:43:38 eno Exp $
# 

echo "Configuration script for Geant4 binary releases at CERN, Version 1.2 December 2005"
#*********************************************************************
#* start customization area
#*********************************************************************
# Configuration parameters, for release
#
#g4version="geant4.9.6.p01"
#g4releases=/data/users/eno/geant
#clhep=/data/users/eno/clhep
#XercesC=/data/users/eno/xerces
g4version="geant4.9.4.p04"
g4releases=/storage/6/work/aackert/ecalupgrade/geant/
clhep=/storage/6/work/aackert/ecalupgrade/clhep/
XercesC=/storage/6/work/aackert/ecalupgrade/xerces/

# Configuration parameters, for system and compiler
#
gccversion="4.3.4"
os="x86_64-slc5-gcc43"
#EXTRALIBS=-L/usr/X11R6/lib64
#EXTRALIBS=-L/usr/lib64
EXTRALIBS=-L/storage/5/Obsolete/usr/lib64
export EXTRALIBS
clhepversion="x86_64-slc5-gcc43-opt"
XercesCversion="xerces-c-3.1.1-x86_64-linux-gcc-3.4"


#*********************************************************************
#* end customization area
#*********************************************************************

g++ --version | grep $gccversion > /dev/null
echo `g++ --version` | grep $gccversion
if [ $? != 0 ]
then
  echo "It looks like your compiler settings are not suitable"
  echo "The Operating system is expected to be $os"
  echo    "The compiler version should be g++ (GCC) $gccversion"
  echo -n "The system reports that it is  "; g++ --version
  echo "Please your PATH and LD_LIBRARY_PATH environment variables"
  echo "You may use the setup script "
  echo " source /afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc43-opt/setup.sh"
  echo "  to set your environment for this compiler"

else

  echo "Setting up the environment for $g4version"

  export G4SYSTEM=Linux-g++
  export G4INSTALL=$g4releases/$g4version
  export G4LIB=$g4releases/$g4version/$os/lib
#  export CLHEP_BASE_DIR=$clhep/$clhepversion/${os}-opt
  export CLHEP_BASE_DIR=$clhep/$clhepversion
#  export XERCESCROOT=$XercesC/$XercesCversion/${os}-opt
  export XERCESCROOT=$XercesC/$XercesCversion

  export G4ABLADATA=$G4INSTALL/data/G4ABLA
  export G4LEDATA=$G4INSTALL/data/G4EMLOW
  export G4NEUTRONHPDATA=$G4INSTALL/data/G4NDL
  export G4LEVELGAMMADATA=$G4INSTALL/data/PhotonEvaporation
  export G4RADIOACTIVEDATA=$G4INSTALL/data/RadioactiveDecay
  export G4ELASTICDATA=$G4INSTALL/data/G4ELASTIC
  export G4REALSURFACEDATA=$G4INSTALL/data/RealSurface
  export G4NEUTRONXSDATA=$G4INSTALL/data/G4NEUTRONXS
  export G4PIIDATA=$G4INSTALL/data/G4PII

  # Geant 4 interface, visualisation and other variables
  export G4UI_USE_TERMINAL=1
  export G4UI_USE_TCSH=1
  export G4UI_USE_GAG=1
  export G4UI_USE_XAW=1
#  export G4UI_USE_XM=1
  #
  export G4VIS_USE_DAWN=1
  export G4VIS_USE_DAWNFILE=1
#  export G4VIS_USE_OPENGLX=1
#  export G4VIS_USE_OPENGLXM=1
  export G4VIS_USE_RAYTRACER=1
  export G4VIS_USE_RAYTRACERX=1
  export G4VIS_USE_VRML=1
  export G4VIS_USE_VRMLFILE=1
  #
  # Geant 4 build variables
  export G4VIS_BUILD_VRML_DRIVER=1
  export G4UI_BUILD_XAW_SESSION=1
#  export G4UI_BUILD_XM_SESSION=1
  export G4LIB_BUILD_G3TOG4=1
#  export G4VIS_BUILD_OPENGLX_DRIVER=1
  export G4VIS_BUILD_RAYTRACERX_DRIVER=1
#  export G4VIS_BUILD_OPENGLXM_DRIVER=1
  export G4UI_BUILD_TERMINAL_SESSION=1
  export G4UI_BUILD_GAG_SESSION=1
  export G4VIS_BUILD_RAYTRACER_DRIVER=1
  export G4VIS_BUILD_DAWNFILE_DRIVER=1
  export G4VIS_BUILD_DAWN_DRIVER=1
  export G4VIS_BUILD_VRMLFILE_DRIVER=1
  export G4LIB_BUILD_GDML=1

  export G4LIB_USE_G3TOG4=1
  # The following is used to store your executables 
  #            (in subdirectories, one per system)

  if [ $LD_LIBRARY_PATH ]
  then
     LD_LIBRARY_PATH=${G4LIB}:${CLHEP_BASE_DIR}/lib:${XERCESCROOT}/lib:${LD_LIBRARY_PATH}
  else
     LD_LIBRARY_PATH=${G4LIB}:${CLHEP_BASE_DIR}/lib:${XERCESCROOT}/lib
  fi
  export LD_LIBRARY_PATH

  if [ $G4WORKDIR ]
  then
    echo "G4WORKDIR already set"   
  else
    echo "Setting G4WORKDIR"
    export G4WORKDIR=/storage/6/work/aackert/ecalupgrade/geant/$g4version/$os
  fi

  if [ ! -d $G4WORKDIR ]
  then
     echo "Creating the G4WORKDIR directory $G4WORKDIR"
     mkdir -p $G4WORKDIR
  fi

  # The following is used to store your executables 
  #            (in subdirectories, one per system)
  if [ $G4BIN ]
  then
    echo "G4BIN already set"
  else
    export G4BIN=$G4WORKDIR/bin
  fi

fi
