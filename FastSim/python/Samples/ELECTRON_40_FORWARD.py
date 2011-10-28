import FWCore.ParameterSet.Config as cms

fileName=cms.untracked.vstring()

source = cms.Source(
	"PoolSource", 
	noEventSort = cms.untracked.bool(True), 
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"), 
	fileNames = fileName
        )

fileName.extend([
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_0.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_1.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_2.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_4.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_5.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_6.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_7.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_8.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_9.root",
])
