import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('FastSimulation.Configuration.Geometries_MC_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MC_42_V15A::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
    'file:SinglePigun_FASTSIM.root'
    )
)

process.demo = cms.EDAnalyzer('CaloAnalyzer'
)


process.p = cms.Path(process.demo)

process.MessageLogger.cerr.FwkReport.reportEvery = 1000

#SOURCE process.load('#SRC')

