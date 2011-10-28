#! /bin/sh

# Done by Maxime Gouzevitch
# mgouzevi@cern.ch
# 2011/10/28

REP=$1
PART=$2
ENERGY=$3
REGION=$4
NFILES=$5
NEVENTS=$6
PRODUCE=$7
QUEUE=$8

if [ $NFILES -le 0 ] 
then
    NFILES=20
fi

if [ $NEVENTS -le 0 ] 
then
    NEVENTS=1000
fi

if [ $# -le 7 ]
then
  echo "Usage: `basename $0` {Repertory ProdName SourceNameForOutRep bProduce DataType}"
  exit
fi

PTITLE=$PART

case $PART in
11) PTITLE='ELECTRON';;
12) PTITLE="NEUTRINO";;
22) PTITLE="PHOTON";;
211) PTITLE='PION';;
*) echo "This particle is not in the list of cmgAnalysis_Analyser.sh, you may want to update it. For the moment we name the repertory with "$PART". Do you want to continue? Y or N"
	read answer
	if [ $answer -eq 'Y' ] 
	    then
	    PTITLE=$PART
	else 
	    echo "stop the script"
	    exit
	fi;;
esac

OUT=${REP}'/'${PTITLE}'/'${ENERGY}'GEV/'${REGION}

echo "------------------------------------"
echo "------------------------------------"
echo "CASTOR ROOT REPERTORY "$REP
echo "FILES Store repertory "$OUT
echo "Particle with PdgId ="$PART" is "$PTITLE
echo "Energy ="$ENERGY
echo "Calorimeter region = "$REGION
echo "Number of files to produce "$NFILES
echo "with "$NEVENTS" events"
echo "have to be produced = "$PRODUCE
echo "------------------------------------"
echo "------------------------------------"

MINE=$[ENERGY-1]


sed s/'#PART '/''/ <SinglePigun_FASTSIM_cfg.py | 
sed s/'#MIN'/${MINE}/ |
sed s/'#MAX'/${ENERGY}/ | 
sed s/'#'$REGION' '/''/ |
sed s/'#ENERGY '/''/ |
sed s/'#PARTID'/$PART/ |
sed s/'#MEVENTS '/''/ |
sed s/'#NEVENTS'/$NEVENTS/ |
sed s/'#FILENAME '/''/ >SinglePigun_FASTSIM_launch_cfg.py



if [ $PRODUCE -eq 1 ] 
then
    OUT=${REP}'/'${PTITLE}
    nsmkdir $OUT
    OUT=$OUT'/'${ENERGY}'GEV'
    nsmkdir $OUT
    OUT=$OUT'/'${REGION}
    nsmkdir $OUT
    echo "We are launching the production for "$OUT
    
    cmsBatch.py $NFILES SinglePigun_FASTSIM_launch_cfg.py  -o OutResonance_$PTITLE_$ENERGY_$REGION -r ${OUT} -b 'bsub -q '$QUEUE' < batchScript.sh'
else 

    nsls -l $OUT
    ../scripts/massNsCheckFileValidity.sh  ${REP} ${PTITLE}/${ENERGY}'GEV'/${REGION} printGood tmp.py
    mv tmp.py ../python/Samples/${PTITLE}'_'${ENERGY}'_'${REGION}.py

fi
