####################################################################################
##
## Radiation damage simulation parameters
## Brian Francis, Alexander Ledovskoy
## University of Virginia
##
## Usage: https://twiki.cern.ch/twiki/bin/view/CMS/FCALSimSLHCFastSimAging
##
####################################################################################

import FWCore.ParameterSet.Config as cms

# Radiation simulation may be enabled using isDamaged = True
# Total integrated and instantaneous luminosity to simulate ECAL at
ECALRadiationDamageBlock = cms.PSet(
	isDamaged = cms.bool(False),
	lumiTotal = cms.double(500.),
	lumiInstant = cms.double(5.0e+34)
	)

# Suggested settings for Full Readout without noise (specific studies). Add the following 6 lines to FastSim config
#process.ecalRecHit.RecHitsFactory.ECALEndcap.Noise = cms.double(0.)
#process.ecalRecHit.RecHitsFactory.ECALEndcap.Threshold = cms.double(-1e+6)
#process.ecalRecHit.RecHitsFactory.ECALEndcap.HighNoiseParameters = cms.vdouble(5.72,1.65)
#process.ecalRecHit.RecHitsFactory.ECALBarrel.Noise = cms.double(0.)
#process.ecalRecHit.RecHitsFactory.ECALBarrel.Threshold = cms.double(-1e+6)
#process.ecalRecHit.RecHitsFactory.ECALBarrel.HighNoiseParameters = cms.vdouble(5.72,1.65)


# Suggested settings for Full Readout with default noise (specific studies). Add the following 4 lines to FastSim config
#process.ecalRecHit.RecHitsFactory.ECALEndcap.Threshold = cms.double(-1e+6)
#process.ecalRecHit.RecHitsFactory.ECALEndcap.HighNoiseParameters = cms.vdouble(5.72,1.65)
#process.ecalRecHit.RecHitsFactory.ECALBarrel.Threshold = cms.double(-1e+6)
#process.ecalRecHit.RecHitsFactory.ECALBarrel.HighNoiseParameters = cms.vdouble(5.72,1.65)



