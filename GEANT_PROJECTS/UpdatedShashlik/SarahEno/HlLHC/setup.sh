
# source setup.sh /storage/5/cmssw/software/slc5_amd64_gcc434/external/

gcc_config_version=4.3.4-cms
mpfr_config_version=4.3.4-cms
gmp_config_version=4.3.4-cms
#LCGPLAT=x86_64-slc5-gcc34-opt
LCGPLAT=slc5_amd64_gcc434
LCG_lib_name=lib64

if [ -z $1 ]
#then LCG_contdir = /afs/cern.ch/sw/lcg/contrib
then LCG_contdir=/storage/5/cmssw/software/slc5_amd64_gcc434/external/
else LCG_contdir=$1
fi
#LCG_gcc_home = ${LCG_contdir}/gcc/${gcc_config_version}/${LCGPLAT}
#LCG_mpfr_home = ${LCG_contdir}/mpfr/${mpfr_config_version}/${LCGPLAT}
#LCG_gmp_home=${LCG_contdir}/gmp/${gmp_config_version}/${LCGPLAT}
LCG_gcc_home=${LCG_contdir}/gcc/${gcc_config_version}
LCG_mpfr_home=${LCG_contdir}/gcc/${mpfr_config_version}
LCG_gmp_home=${LCG_contdir}/gcc/${gmp_config_version}

export PATH=${LCG_gcc_home}/bin:${PATH}

if [ ${LD_LIBRARY_PATH} ]
then
  export LD_LIBRARY_PATH=${LCG_gcc_home}/${LCG_lib_name}:${LCG_mpfr_home}/lib:${LCG_gmp_home}/lib:${LD_LIBRARY_PATH}
else
  export LD_LIBRARY_PATH=${LCG_gcc_home}/${LCG_lib_name}:${LCG_mpfr_home}/lib:${LCG_gmp_home}/lib
fi
