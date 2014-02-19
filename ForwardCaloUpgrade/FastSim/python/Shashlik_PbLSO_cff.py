import FWCore.ParameterSet.Config as cms

def myForwardECAL( process ):

	#======  Geometrical material properties ========

	# Light Collection efficiency
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightColl = cms.double(0.01)
###    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightColl = cms.double(0.999)

	# Light Collection uniformity
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightCollUnif = cms.double(0.0000)
###    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightCollUnif = cms.double(0.00001)

	# Photostatistics (photons/GeV) in the homegeneous material.
	# estimate the photo-electron fluctuations.
	# Be carefull when you sumulate to keep: photoStatistics*lightColl*ParticleEnergy<2^31. Otherwise TRandom3 would refuse to
	## photoStatistics = cms.double(50.E3),
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.photoStatistics = cms.double(2.26E+05)
	###    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.photoStatistics = cms.double(1.E7)



	# Thickness of the detector in cm
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.thickness = cms.double(22.0)

	    #====== Global parameters of the material ========

	# Interaction length in cm provided by A. Ledovskoy
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.interactionLength  = cms.double(18.65)
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.Aeff = cms.double(197.52)
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.Zeff = cms.double(78.44)
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.rho = cms.double(10.55)


	# Radiation length in g/cm^2
	# Interaction length in cm see Zhu "Crystal Calorimeters in the Next
	# decade"
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.radLenIngcm2 = cms.double(6.59)


	# ===== Those parameters might be entered by hand
	# or calculated out of the previous ones

	# Radiation length in cm. If value set to -1, FastSim uses internally the
	# formula radLenIngcm2/rho
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.radLenIncm = cms.double(-1)
	# Critical energy in GeV. If value set to -1, FastSim uses internally the
	# formula (2.66E-3*(x0*Z/A)^1.1): 8.74E-3 for ECAL EndCap
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.criticalEnergy = cms.double(0.0077)
	# Moliere Radius in cm.If value set to -1, FastSim uses internally the
	# formula : Es/criticalEnergy*X0 with Es=sqrt(4*Pi/alphaEM)*me*c^2=0.0212 GeV
	# This value is known to be 2.190 cm for ECAL Endcap, but the formula gives 2.159 cm
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.moliereRadius = cms.double(1.70)
	
	# Is a homogenious detector?
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.bHom = cms.bool(False)
	
	#====== Parameters for sampling ECAL ========
	
	# Sampling Fraction: Fs = X0eff/(da+dp) where X0eff is the average X0
	# of the active and passive media and da/dp their thicknesses
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.Fs = cms.double(0.62)
	
	# e/mip for the calorimeter. May be estimated by 1./(1+0.007*(Zp-Za))
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.ehat = cms.double(0.85)
	
	# a rough estimate of ECAL resolution sigma/E = resE/sqrt(E)
	# it is used to generate Nspots in radial profiles.
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.resE = cms.double(0.136)

	# the width in cm of the active layer
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.da = cms.double(0.20)
	
	# the width in cm of the passive layer
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.dp = cms.double(0.80)



	# Activate the LogDebug
	process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.debug = cms.bool(True)
