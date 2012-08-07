import FWCore.ParameterSet.Config as cms

def myForwardECAL( process ): 

    #======  Geometrical material properties ========
    
    # Light Collection efficiency

#    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightColl = cms.double(0.999)

    # Light Collection uniformity
#    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightCollUnif = cms.double(0.000)

    # Photostatistics (photons/GeV) in the homegeneous material.
    # estimate the photo-electron fluctuations.
    # Be carefull when you sumulate to keep: photoStatistics*lightColl*ParticleEnergy<2^31. Otherwise TRandom3 would refuse to
    ## photoStatistics = cms.double(50.E3),

#   process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.photoStatistics = cms.double(1.E7)

    # Activate the LogDebug
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.debug = cms.bool(True)


