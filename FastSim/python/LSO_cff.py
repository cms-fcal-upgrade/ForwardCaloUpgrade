import FWCore.ParameterSet.Config as cms

def myForwardECAL( process ): 

    #======  Geometrical material properties ========
    
    # Light Collection efficiency

    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightColl = cms.double(0.03)

    # Light Collection uniformity
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightCollUnif = cms.double(0.003)

    # Photostatistics (photons/GeV) in the homegeneous material
    ## photoStatistics = cms.double(50.E3),
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.photoStatistics = cms.double(1610.E3)
                 
    # Thickness of the detector in cm
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.thickness = cms.double(22.0)

    #====== Global parameters of the material ========

    # Interaction length in cm
    # Interaction length in cm see Zhu "Crystal Calorimeters in the Next
    # decade"
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.interactionLength  = cms.double(20.9)
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.Aeff = cms.double(138.2)
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.Zeff = cms.double(56.6)
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.rho = cms.double(7.4)

    
    # Radiation length in g/cm^2
    # Interaction length in cm see Zhu "Crystal Calorimeters in the Next
    # decade"
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.radLenIngcm2 = cms.double(8.436)
                 

    # ===== Those parameters might be entered by hand
    # or calculated out of the previous ones 

    # Radiation length in cm. If value set to -1, FastSim uses internally the
    # formula radLenIngcm2/rho
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.radLenIncm = cms.double(-1)
    # Critical energy in GeV. If value set to -1, FastSim uses internally the
    # formula (2.66E-3*(x0*Z/A)^1.1): 8.74E-3 for ECAL EndCap
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.criticalEnergy = cms.double(-1)
    # Moliere Radius in cm.If value set to -1, FastSim uses internally the
    # formula : Es/criticalEnergy*X0 with Es=sqrt(4*Pi/alphaEM)*me*c^2=0.0212 GeV
    # This value is known to be 2.190 cm for ECAL Endcap, but the formula gives 2.159 cm
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.moliereRadius = cms.double(2.07)

    # Is a homogenious detector?
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.bHom = cms.bool(True)

    # Activate the LogDebug
    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.debug = cms.bool(True)


