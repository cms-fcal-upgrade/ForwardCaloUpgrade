#! /bin/sh

# Done by Maxime Gouzevitch
# mgouzevi@cern.ch
# 2011/10/28

# The root of the castor repertory where you want to store your data. This
# repertory must exist
REP=/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE
# The PDG id of your particle. The script would automatically transform
# it into particle name when known and add to the repertory name
# e- = 11; nu = 12; gamma= 22; pion=211
PART=22
# Energy f your particle in GeV
ENERGY=10
# Region of the calorimeter where you want to fire your aprticle. 2 options possible: CENTRAL (-0.9 < eta < 0.9) or FORWARD (1.6 < eta < 2.4)
REGION=FORWARD
# ProdName to have a production with specific conditions
PROD=STANDARD
#Number of files you want to generate
NFILES=5
#Numer of events per file
NEVENTS=4000
# Batch queue may be you need to change if long jobs 
QUEUE=8nh
# Produce those files (1) or create a source file (0) or produce histograms (2)?
PRODUCE=2

i=1
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

../scripts/singleGun_Analyser.sh $REP $PART $ENERGY $REGION $NFILES $NEVENTS $PRODUCE $QUEUE $PROD

i=$[i+1]
done

