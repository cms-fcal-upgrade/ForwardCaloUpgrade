
#include "HFDataFormatMessenger.hh"

#include "HFDataFormat.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"

#include <iostream>


HFDataFormatMessenger::HFDataFormatMessenger(HFDataFormat *df)
{ 

  m_dataformat = df;

  std::cout << "HFDataFormatMessenger::" << std::endl;
  m_dir = new G4UIdirectory("/output/");
  m_dir->SetGuidance("output options");

  m_storeOpticalInfoCmd = new G4UIcmdWithABool("/output/storeOpticalInfo", this);
  m_storeOpticalInfoCmd->SetGuidance("Set if store Cerenkov optical photon info in the output ROOT file.");
  m_storeOpticalInfoCmd->SetParameterName("storeOpticalInfo", true);
  m_storeOpticalInfoCmd->SetDefaultValue(true);
  m_storeOpticalInfoCmd->AvailableForStates(G4State_Idle);

  m_storeParticleInfoCmd = new G4UIcmdWithABool("/output/storeParticleInfo", this);
  m_storeParticleInfoCmd->SetGuidance("Set if store all the secondary particles info in the output ROOT file.");
  m_storeParticleInfoCmd->SetParameterName("storeParticleInfo", true);
  m_storeParticleInfoCmd->SetDefaultValue(true);
  m_storeParticleInfoCmd->AvailableForStates(G4State_Idle);
   
  m_storeGeneratorInfoCmd = new G4UIcmdWithABool("/output/storeGeneratorInfo", this);
  m_storeGeneratorInfoCmd->SetGuidance("Set if store all the generator level particles info in the output ROOT file.");
  m_storeGeneratorInfoCmd->SetParameterName("storeGeneratorInfo", true);
  m_storeGeneratorInfoCmd->SetDefaultValue(true);
  m_storeGeneratorInfoCmd->AvailableForStates(G4State_Idle);

  m_storePMTInfoCmd = new G4UIcmdWithABool("/output/storePMTInfo", this);
  m_storePMTInfoCmd->SetGuidance("Set if store PMT info of photons at end of fibers.");
  m_storePMTInfoCmd->SetDefaultValue(true);
  m_storePMTInfoCmd->AvailableForStates(G4State_Idle);

  m_genTreeCmd = new G4UIcmdWithoutParameter("/output/genTree",this);
  m_genTreeCmd->SetGuidance("Required command!! Generate braches for the Tree in the output ROOT file.");

}

HFDataFormatMessenger::~HFDataFormatMessenger()
{

  delete m_storeOpticalInfoCmd;
  delete m_storeParticleInfoCmd;
  delete m_storeGeneratorInfoCmd;
  delete m_storePMTInfoCmd;
  delete m_genTreeCmd;

}

void HFDataFormatMessenger::SetNewValue(G4UIcommand* cmd,G4String newValue)
{

  if ( cmd ==   m_storeOpticalInfoCmd ) {
    m_dataformat->SetStoreOpticalInfo(m_storeOpticalInfoCmd->GetNewBoolValue(newValue));
  } 
  else if ( cmd ==   m_storeParticleInfoCmd ) {
    m_dataformat->SetStoreParticleInfo(m_storeParticleInfoCmd->GetNewBoolValue(newValue));
  } 
  else if ( cmd ==   m_storeGeneratorInfoCmd ) {
    m_dataformat->SetStoreGeneratorInfo(m_storeGeneratorInfoCmd->GetNewBoolValue(newValue));
  }
  else if ( cmd == m_storePMTInfoCmd ) {
    m_dataformat->SetStorePMTInfo(m_storePMTInfoCmd->GetNewBoolValue(newValue));
  }
  else if ( cmd == m_genTreeCmd ) { 
    m_dataformat->generateTrees();
  }

}
