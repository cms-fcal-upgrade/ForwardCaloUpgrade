import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('FastSimulation.Configuration.Geometries_MC_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'MC_42_V15A::All'
process.GlobalTag.globaltag = 'MC_50_V13::All'
#process.GlobalTag.globaltag = 'MC_60_V4::All'

# -------- This is what you would need if you want to do the multilayers ECAL ---------- 
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'MC_60_V4::All', '')
#process.GlobalTag.toGet = cms.VPSet(
#	    cms.PSet(record = cms.string('PEcalEndcapRcd'),
#				                          tag = cms.string('EERECO_Geometry_newTag'),
#				                          connect = cms.untracked.string('sqlite_file:/afs/cern.ch/user/m/mgouzevi/scratch0/GEOMETRY/FINAL/CMSSW_6_0_0/src/ForwardCaloUpgrade/FastSim/test/EERECO_Geometry.db')
#				                                                   )
#		)    


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

