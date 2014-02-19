import FWCore.ParameterSet.Config as cms

fileName=cms.untracked.vstring()

source = cms.Source(
	"PoolSource", 
	noEventSort = cms.untracked.bool(True), 
	duplicateCheckMode = cms.untracked.string("noDuplicateCheck"), 
	fileNames = fileName
        )

fileName.extend([
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/EF_LC_4TMS/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_0.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/EF_LC_4TMS/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_1.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/EF_LC_4TMS/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_2.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/EF_LC_4TMS/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_3.root",
    "rfio:/castor/cern.ch/cms/store/cmst3/user/mgouzevi/FCAL_UPGRADE/EF_LC_4TMS/ELECTRON/40GEV/FORWARD/SinglePigun_FASTSIM_4.root",
])
