set gcc_config_version = 4.3.4-cms
set mpfr_config_version = 4.3.4-cms
set gmp_config_version=4.3.4-cms
#set LCGPLAT = x86_64-slc5-gcc34-opt
set LCGPLAT = slc5_amd64_gcc434
set LCG_lib_name = lib64

if ( $1 == "" ) then
#set LCG_contdir = /afs/cern.ch/sw/lcg/contrib
set LCG_contdir = /sharesoft/cmssw/${LCGPLAT}/external
else
set LCG_contdir = "$1"
endif
#set LCG_gcc_home = ${LCG_contdir}/gcc/${gcc_config_version}/${LCGPLAT}
#set LCG_mpfr_home = ${LCG_contdir}/mpfr/${mpfr_config_version}/${LCGPLAT}
#set LCG_gmp_home=${LCG_contdir}/gmp/${gmp_config_version}/${LCGPLAT}
set LCG_gcc_home = ${LCG_contdir}/gcc/${gcc_config_version}
set LCG_mpfr_home = ${LCG_contdir}/gcc/${mpfr_config_version}
set LCG_gmp_home=${LCG_contdir}/gcc/${gmp_config_version}

setenv PATH ${LCG_gcc_home}/bin:${PATH}

if ($?LD_LIBRARY_PATH) then
setenv LD_LIBRARY_PATH ${LCG_gcc_home}/${LCG_lib_name}:${LCG_mpfr_home}/lib:${LCG_gmp_home}/lib:${LD_LIBRARY_PATH}
else
setenv LD_LIBRARY_PATH ${LCG_gcc_home}/${LCG_lib_name}:${LCG_mpfr_home}/lib:${LCG_gmp_home}/lib
endif
