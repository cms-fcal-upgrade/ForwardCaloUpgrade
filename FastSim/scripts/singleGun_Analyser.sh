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
PROD=$9

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


OUT=${REP}'/'${PROD}'/'${PTITLE}'/'${ENERGY}'GEV/'${REGION}

echo "------------------------------------"
echo "------------------------------------"
echo "CASTOR ROOT REPERTORY "${REP}
echo "PRODUCTION IS "${PROD}
echo "FILES Store repertory "${OUT}
echo "Particle with PdgId ="${PART}" is "${PTITLE}
echo "Energy ="${ENERGY}
echo "Calorimeter region = "${REGION}
echo "Number of files to produce "${NFILES}
echo "with "${NEVENTS}" events"
echo "have to be produced = "${PRODUCE}
echo "------------------------------------"
echo "------------------------------------"

MINE=$[ENERGY-1]



if [ $PRODUCE -eq 1 ] 
then

    sed s/'#PART '/''/ <SinglePigun_FASTSIM_cfg.py | 
    sed s/'#MIN'/${MINE}/ |
    sed s/'#MAX'/${ENERGY}/ | 
    sed s/'#'${REGION}' '/''/ |
    sed s/'#ENERGY '/''/ |
    sed s/'#PARTID'/${PART}/ |
    sed s/'#MEVENTS '/''/ |
    sed s/'#NEVENTS'/${NEVENTS}/ |
    sed s/'#FILENAME '/''/ >SinglePigun_FASTSIM_launch_cfg.py
	
    echo "We are launching the production for "$OUT
    
    

    cmsBatch.py $NFILES SinglePigun_FASTSIM_launch_cfg.py  -r ${OUT} -b 'bsub -q '$QUEUE' < batchScript.sh'
elif [ $PRODUCE -eq 0 ] 
then

    rm tmp.py
    outputFile=tmp.py

    touch $outputFile

    echo "import FWCore.ParameterSet.Config as cms" >>$outputFile
    echo "" >>$outputFile
    echo "fileName=cms.untracked.vstring()" >>$outputFile
    echo "" >>$outputFile
    echo "source = cms.Source(
	\"PoolSource\", 
	noEventSort = cms.untracked.bool(True), 
	duplicateCheckMode = cms.untracked.string(\"noDuplicateCheck\"), 
	fileNames = fileName
        )" >>$outputFile

    echo "" >>$outputFile
    echo "fileName.extend([" >>$outputFile 
    
    NINF=$[NFILES-2]
    i=0
    while [ $i -le $NINF ]; do
	
	test=$(cmsLs ${OUT}"/DQM_V0001_R000000001__ParticleGun__FastSim__DQM_"${i}".root")
	if `echo ${test} | grep "No such file or directory" 1>/dev/null 2>&1`
	    then
	    echo "No File" ${OUT}"/DQM_V0001_R000000001__ParticleGun__FastSim__DQM_"${i}".root"
	else
	    echo "'root://eoscms//eos/cms/"${OUT}"/SinglePigun_FASTSIM_"${i}".root',">>$outputFile 	
	fi
	i=$[i+1]
    done
    NINF=$[NFILES-1]


    test=$(cmsLs ${OUT}"/DQM_V0001_R000000001__ParticleGun__FastSim__DQM_"${NINF}".root")
    if `echo ${test} | grep "No such file or directory" 1>/dev/null 2>&1`
	then
	echo "No File" ${OUT}"/DQM_V0001_R000000001__ParticleGun__FastSim__DQM_"${i}".root"
    else
	echo "'root://eoscms//eos/cms/"${OUT}"/SinglePigun_FASTSIM_"${NINF}".root'">>$outputFile 	
    fi

    echo "])" >>$outputFile
    
    mv tmp.py ../python/Samples/${PROD}_${PTITLE}'_'${ENERGY}'_'${REGION}.py


else 
    rm /tmp/mgouzevi/*.*
    mkdir ${PTITLE}_${REGION}
    sed s@'#SOURCE '@''@ <caloanalyzer_cfg.py | 
    sed s@'#SRC'@ForwardCaloUpgrade.FastSim.Samples.${PROD}_${PTITLE}'_'${ENERGY}'_'${REGION}@ >caloanalyzer_launch_cfg.py 
    cmsRun caloanalyzer_launch_cfg.py 
    cp calorimeter_histograms.root  ${PTITLE}_${REGION}/calorimeter_histograms_${PROD}_${PTITLE}_${ENERGY}_${REGION}.root

    hadd="hadd /tmp/mgouzevi/DQM_ShowerShape_"${PROD}"_"${PTITLE}"_"${ENERGY}_${REGION}".root "

    NINF=$[NFILES-1]
    i=0
    while [ $i -le $NINF ]; do
	test=$(cmsLs ${OUT}"/DQM_V0001_R000000001__ParticleGun__FastSim__DQM_"${i}".root")
	if `echo ${test} | grep "No such file or directory" 1>/dev/null 2>&1`
	    then
	    echo "No File" ${OUT}"/DQM_V0001_R000000001__ParticleGun__FastSim__DQM_"${i}".root"
	else
	    cmsStage ${OUT}"/DQM_V0001_R000000001__ParticleGun__FastSim__DQM_"${i}".root" /tmp/mgouzevi/
	    hadd=$hadd" /tmp/mgouzevi/DQM_V0001_R000000001__ParticleGun__FastSim__DQM_"${i}".root"
	fi
	i=$[i+1]
    done

    echo $hadd
    $hadd
    cp /tmp/mgouzevi/DQM_ShowerShape_"${PROD}"_"${PTITLE}_${ENERGY}_${REGION}".root ELECTRON_FORWARD
    rm /tmp/mgouzevi/*.*
fi
