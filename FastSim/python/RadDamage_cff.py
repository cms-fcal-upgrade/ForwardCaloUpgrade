import FWCore.ParameterSet.Config as cms

def RadDamage(process, isDam, lumiTot, lumiInst):
    process.famosSimHits.Calorimetry.CalorimeterProperties.ECALRadiationDamage = cms.PSet(
        isDamaged = cms.bool(isDam),
        lumiTotal = cms.double(lumiTot),
        lumiInstant = cms.double(lumiInst)
        )
    process.FamosCalorimetryBlock.Calorimetry.CalorimeterProperties.ECALRadiationDamage = process.famosSimHits.Calorimetry.CalorimeterProperties.ECALRadiationDamage
    process.ecalRecHit.RecHitsFactory.ECALBarrel.ECALRadiationDamage = process.famosSimHits.Calorimetry.CalorimeterProperties.ECALRadiationDamage
    process.ecalRecHit.RecHitsFactory.ECALEndcap.ECALRadiationDamage = process.famosSimHits.Calorimetry.CalorimeterProperties.ECALRadiationDamage
    process.hbhereco.RecHitsFactory.HCAL.ECALRadiationDamage = process.famosSimHits.Calorimetry.CalorimeterProperties.ECALRadiationDamage
