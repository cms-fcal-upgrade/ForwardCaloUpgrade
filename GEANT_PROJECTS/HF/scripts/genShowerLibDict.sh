#!/usr/bin/env bash

# Bash script to generate a root dictionary for
# some mysterious types (e.g. std::vector<float>)
# oh my.

CFCSCRIPTDIR=$(dirname $BASH_SOURCE)
CALLDIR=$PWD

# move into the scripts directory
cd $CFCSCRIPTDIR

# generate the library
root -l -b -q -L ShowLibDict.C+

if [ ! -e ShowLibDict_C.so ]; then
  echo "Something went wrong creating the dictionary!"
  return 0
fi

# prepare a plcae to install the library
if [ ! -d $G4WORKDIR/lib ]; then
  mkdir $G4WORKDIR/lib
fi

cp ShowLibDict_C.so $G4WORKDIR/lib/
cd ${G4WORKDIR}/lib
ln -s ShowLibDict_C.so libShowLibDict_C.so
cd -


# check LD_LIBRARY_PATH has G$WORKDIR/lib included
hasDir=0
addDir=$G4WORKDIR/lib
for dir in `echo $LD_LIBRARY_PATH | sed "s/:/\n/g"`; do 
  if [ $dir == $addDir ]; then
    hasDir=1
  fi
done

if [ $hasDir == "0" ]; then
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$addDir
fi

# change directory back to original  
cd $CALLDIR

unset CFCSCRIPTDIR
unset dir
unset hasDir
unset addDir
unset CALLDIR
