import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

# Include DQMStore, needed by the famosSimHits
process.DQMStore = cms.Service( "DQMStore")

# Include the RandomNumberGeneratorService definition
process.load("FastSimulation/Configuration/RandomServiceInitialization_cff")

# Include the source
#process.load("Configuration/Generator/H200ZZ4L_cfi")

#process.source = cms.Source("PoolSource",
#    # replace 'myfile.root' with the source file you want to use
#    fileNames = cms.untracked.vstring(
#        'file:myfile.root'
#'file:/afs/cern.ch/cms/Tutorials/TWIKI_DATA/RelValTTbar_RECO_331.root'
#    )
#)

##process.generator = cms.EDProducer("FlatRandomPtGunProducer",
process.generator = cms.EDProducer("FlatRandomEGunProducer",
    firstRun = cms.untracked.uint32(1),
    PGunParameters = cms.PSet(
        PartID = cms.vint32(211),
        MinEta = cms.double( 1.392),
        MaxEta = cms.double( 2.650),
        MinPhi = cms.double(-3.1415927),
        MaxPhi = cms.double( 3.1415927),
        MinE = cms.double(100.00),
        MaxE = cms.double(100.01)
    ),
    AddAntiParticle = cms.bool(False), # back-to-back particles
    Verbosity = cms.untracked.int32(0) ## for printouts, set it to 1 (or greater)   
)

# Famos sequences (Frontier conditions)
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('FastSimulation.Configuration.Geometries_cff')
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['mc']

process.load("FastSimulation/Configuration/FamosSequences_cff")

# Parametrized magnetic field (new mapping, 4.0 and 3.8T)
#process.load("Configuration.StandardSequences.MagneticField_40T_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.VolumeBasedMagneticFieldESProducer.useParametrizedTrackerField = True

# If you want to turn on/off pile-up
#process.load('FastSimulation.PileUpProducer.PileUpSimulator_2012_Startup_inTimeOnly_cff')
#process.load('FastSimulation.PileUpProducer.mix_2012_Startup_inTimeOnly_cff')
process.load('FastSimulation.PileUpProducer.PileUpSimulator_NoPileUp_cff')
process.famosPileUp.PileUpSimulator.averageNumber = 0.0    

# You may not want to simulate everything for your study
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.famosSimHits.SimulateMuons = True

process.demo = cms.EDAnalyzer('DemoAnalyzer')

process.TFileService = cms.Service("TFileService",
            fileName = cms.string('histodemo_Pi_gun.root') )

#
# Lines below show how to change HE longitudinal segmentation
#============================================================ 
# SLHCMode should set (True) for activation of new HE segmentation
#
process.HcalTopologyIdealEP.SLHCMode  = cms.untracked.bool(True)
#
# User can choose desired HE segmentation: Eta16 - Eta29;
# for example for Eta16 - first 5 layers will be combined in the first tower
# and all other to the second tower.
# Each line is valid for all subsequent eta's ring till new line.  
#
# If SLHCMode is True and there are no lines with Eta's below ==> the default
# (LS1) upgrade HE segmentation will be set.
#
process.HcalTopologyIdealEP.Eta1  = cms.untracked.vint32(1,2,2,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5)
process.HcalTopologyIdealEP.Eta16 = cms.untracked.vint32(1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2)
process.HcalTopologyIdealEP.Eta17 = cms.untracked.vint32(1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,8,8,8)
process.HcalTopologyIdealEP.Eta18 = cms.untracked.vint32(1,1,2,2,2,3,3,4,5,5,6,6,7,7,8,8,8,8,8)
#
process.source = cms.Source("EmptySource")
process.p1 = cms.Path(process.generator*process.simulationSequence*process)
#
# "famosSimulationSequence" produces also caloRecHits. For running this
# sequence please don't forget add in FastSimulation/CaloRecHitsProducer/src
# new version of HcalRecHitsMaker.cc 
#
#process.p1 = cms.Path(process.generator*process.famosSimulationSequence)

# Make the job crash in case of missing product
process.options = cms.untracked.PSet( Rethrow = cms.untracked.vstring('ProductNotFound') )

