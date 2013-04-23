
#include "HFDetectorConstructionMessenger.hh"

#include "HFDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

#include <iostream>


HFDetectorConstructionMessenger::HFDetectorConstructionMessenger(HFDetectorConstruction *det)
:m_detector(det)
{

  std::cout << "HF Constructing messenger" << std::endl;
  m_dir = new G4UIdirectory("/testBeam/");
  m_dir->SetGuidance("HF W calorimeter control");


  m_nRodCmd = new G4UIcmdWithAnInteger("/testBeam/nRods",this);
  m_nRodCmd->SetGuidance("Set number of rods along diagonal");
  m_nRodCmd->SetParameterName("nRods",false);
  m_nRodCmd->SetDefaultValue(268);
  m_nRodCmd->AvailableForStates(G4State_Idle);
  

  m_rRodCmd = new G4UIcmdWithADoubleAndUnit("/testBeam/rRod",this);
  m_rRodCmd->SetGuidance("Set the W rod radius");
  m_rRodCmd->SetParameterName("rRod",false);
  m_rRodCmd->SetDefaultValue(2.5);
  m_rRodCmd->SetDefaultUnit("mm");
  m_rRodCmd->AvailableForStates(G4State_Idle);

  m_lengthCmd = new G4UIcmdWithADoubleAndUnit("/testBeam/length",this);
  m_lengthCmd->SetGuidance("Set the Length of the calorimeter");
  m_lengthCmd->SetParameterName("length",false);
  m_lengthCmd->SetDefaultValue(100);
  m_lengthCmd->SetDefaultUnit("cm");
  m_lengthCmd->AvailableForStates(G4State_Idle);

  m_fibreIndexCmd =  new G4UIcmdWithADouble("/testBeam/fibreIndex",this);
  m_fibreIndexCmd->SetGuidance("Set the fibre index of refraction");
  m_fibreIndexCmd->SetParameterName("fibreIndex",false);
  m_fibreIndexCmd->SetDefaultValue(1.457);
  m_fibreIndexCmd->AvailableForStates(G4State_Idle);
  
  m_cladIndexCmd =  new G4UIcmdWithADouble("/testBeam/cladIndex",this);
  m_cladIndexCmd->SetGuidance("Set the clad index of refraction");
  m_cladIndexCmd->SetParameterName("cladIndex",false);
  m_cladIndexCmd->SetDefaultValue(1.42);
  m_cladIndexCmd->AvailableForStates(G4State_Idle);

}

HFDetectorConstructionMessenger::~HFDetectorConstructionMessenger()
{

  delete m_nRodCmd;
  delete m_rRodCmd;
  delete m_lengthCmd;
  delete m_fibreIndexCmd;
  delete m_cladIndexCmd;

}

void HFDetectorConstructionMessenger::SetNewValue(G4UIcommand* cmd,G4String newValue)
{

  std::cout << "HF parsing commands " << cmd << " " << newValue << " | " <<
  m_nRodCmd << " " << m_rRodCmd << std::endl;

  if ( cmd == m_nRodCmd ) {
    m_detector->SetNRods(m_nRodCmd->GetNewIntValue(newValue));
  } 
  else if ( cmd == m_rRodCmd ) {
    m_detector->SetRRod(m_rRodCmd->GetNewDoubleValue(newValue));
  } 
  else if ( cmd == m_lengthCmd ) {
    m_detector->SetLength(m_lengthCmd->GetNewDoubleValue(newValue));
  } 
  else if ( cmd == m_fibreIndexCmd ) {
    m_detector->SetFibreIndex(m_fibreIndexCmd->GetNewDoubleValue(newValue));
  } 
  else if ( cmd == m_cladIndexCmd ) {
    m_detector->SetCladIndex(m_cladIndexCmd->GetNewDoubleValue(newValue));
  }

}
