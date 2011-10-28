#! /bin/sh

# Done by Yuri Gouzevitch
# ygouzevi@cern.ch
# 2011/10/28

# The root of the castor repertory where you want to store your data. This
# repertory must exist
REP=/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE
# The PDG id of your particle. The script would automatically transform
# it into particle name when known and add to the repertory name
PART=11
# Energy f your particle in GeV
ENERGY=10
# Region of the calorimeter where you want to fire your aprticle. 2 options possible: CENTRAL (-0.9 < eta < 0.9) or FORWARD (1.6 < eta < 2.4)
REGION=FORWARD
#Number of files you want to generate
NFILES=10
#Numer of events per file
NEVENTS=2000
# Produce those files (1) or create a source file (0)?
PRODUCE=0
# Batch queue may be you need to change if long jobs 
QUEUE=8nh


i=7
while [ $i -le 7 ]; do

case $i in
1) ENERGY=10;;
2) ENERGY=20;;
3) ENERGY=40;;
4) ENERGY=80;;
5) ENERGY=160;;
6) ENERGY=320;;
7) ENERGY=640;;
esac

../scripts/cmgAnalysis_Analyser.sh $REP $PART $ENERGY $REGION $NFILES $NEVENTS $PRODUCE $QUEUE

i=$[i+1]
done

