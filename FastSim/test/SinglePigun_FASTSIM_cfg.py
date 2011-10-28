# Auto generated configuration file
# using: 
# Revision: 1.284.2.2 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: SinglePiPt1_cfi.py -s GEN,FASTSIM,HLT --pileup=NoPileUp --geometry DB --conditions=auto:mc --eventcontent=RECOSIM --datatier GEN-SIM-RECO -n 10
import FWCore.ParameterSet.Config as cms

process = cms.Process('HLT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('FastSimulation.PileUpProducer.PileUpSimulator_NoPileUp_cff')
process.load('FastSimulation.Configuration.Geometries_MC_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('FastSimulation.Configuration.FamosSequences_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedParameters_cfi')
process.load('FastSimulation.Configuration.HLT_GRun_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FastSimulation.Configuration.EventContent_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(2000)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.5 $'),
    annotation = cms.untracked.string('SinglePiPt1_cfi.py nevts:10'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition


process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    fileName = cms.untracked.string('/tmp/SinglePigun_FASTSIM.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.famosSimHits.SimulateCalorimetry = True
process.famosSimHits.SimulateTracking = True
process.simulation = cms.Sequence(process.simulationWithFamos)
process.HLTEndSequence = cms.Sequence(process.reconstructionWithFamos)
process.Realistic7TeVCollisionVtxSmearingParameters.type = cms.string("BetaFunc")
process.famosSimHits.VertexGenerator = process.Realistic7TeVCollisionVtxSmearingParameters
process.famosPileUp.VertexGenerator = process.Realistic7TeVCollisionVtxSmearingParameters
process.GlobalTag.globaltag = 'MC_42_V15A::All'
#'MC_311_V2::All'



process.generator = cms.EDProducer("FlatRandomEGunProducer",
    PGunParameters = cms.PSet(
	MinE = cms.double(9.99),
        MaxE = cms.double(10.01),
	# Pion = 211, nu_e = 12, e = 11
        PartID = cms.vint32(11),
	# Eta limits set up to be in the end-cap
        MinEta = cms.double(1.6),
        MaxEta = cms.double(2.5),
	MinPhi = cms.double(-3.14159265359),
        MaxPhi = cms.double(3.14159265359)
    ),
    Verbosity = cms.untracked.int32(0),
    psethack = cms.string('single pi pt 1'),
    AddAntiParticle = cms.bool(False),
    firstRun = cms.untracked.uint32(1)
)


# ---------------------------
# Switch off different noises
# ---------------------------

# this is important for particle gun studies
# you might like to change the noise parameters to those you expect from
# your detector model and then study.

process.ecalRecHit.doMiscalib = True
process.ecalRecHit.RecHitsFactory.ECALBarrel.Noise = cms.double(0)
process.ecalRecHit.RecHitsFactory.ECALBarrel.Threshold = cms.double(0.001)
process.ecalRecHit.RecHitsFactory.ECALBarrel.HighNoiseParameters = cms.vdouble()
process.ecalRecHit.RecHitsFactory.ECALBarrel.SRThreshold = cms.double(0.0)

process.ecalRecHit.RecHitsFactory.ECALEndcap.Noise = cms.double(0)
process.ecalRecHit.RecHitsFactory.ECALEndcap.Threshold = cms.double(0.001)
process.ecalRecHit.RecHitsFactory.ECALEndcap.SRThreshold = cms.double(0.0)
process.ecalRecHit.RecHitsFactory.ECALEndcap.HighNoiseParameters = cms.vdouble()

process.ecalPreshowerRecHit.RecHitsFactory.Noise = cms.double(0.000001)
process.ecalPreshowerRecHit.RecHitsFactory.Threshold = cms.double(4.5e-10)

process.hbhereco.doMiscalib = True
process.horeco.doMiscalib = True
process.hfreco.doMiscalib = True

process.hbhereco.RecHitsFactory.Noise = cms.vdouble(0, 0)
process.horeco.RecHitsFactory.Noise = cms.vdouble(0)
process.hfreco.RecHitsFactory.Noise = cms.vdouble(0)

# -----------------------------------------------------------------------------
# Switch off interactions in the CMS tracker
# -----------------------------------------------------------------------------

# this allow to have a more clean particles sample arriving to the calorimeters
# for more "physics oriented studies" you would probably like to switch them back

process.famosSimHits.MaterialEffects.PairProduction = False
process.famosSimHits.MaterialEffects.Bremsstrahlung = False
process.famosSimHits.MaterialEffects.EnergyLoss = False
process.famosSimHits.MaterialEffects.MultipleScattering = False
process.famosSimHits.MaterialEffects.NuclearInteraction = False



# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen_genonly)
process.reconstruction = cms.Path(process.reconstructionWithFamos)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)
process.sim = cms.Path(process.simulationWithFamos)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.sim)
#process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.reconstruction,process.RECOSIMoutput_step])
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 



#--------------------------- Dont change those lines -----------------------------#
#------- They are used by scripts to produce automatically large data samples ----#

#FORWARD process.generator.PGunParameters.MinEta = cms.double(1.6)
#FORWARD process.generator.PGunParameters.MinEta = cms.double(2.4)

#CENTRAL process.generator.PGunParameters.MinEta = cms.double(-0.9)
#CENTRAL process.generator.PGunParameters.MinEta = cms.double(0.9)

#ENERGY process.generator.PGunParameters.MinE = cms.double(#MIN.99)
#ENERGY process.generator.PGunParameters.MaxE = cms.double(#MAX.01)

#PART process.generator.PGunParameters.PartID = cms.vint32(#PARTID)
#MEVENTS process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(#NEVENTS))
#FILENAME process.RECOSIMoutput.fileName = cms.untracked.string('SinglePigun_FASTSIM.root')
