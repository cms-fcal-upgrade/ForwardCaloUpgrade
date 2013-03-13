
#include "HFMessenger.hh"

#include "HFDetectorConstruction.hh"

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

#include <iostream>


HFMessenger::HFMessenger(HFDetectorConstruction *det)
:m_detector(det)
{

  std::cout << "HF Constructing messenger" << std::endl;
  m_dir = new G4UIdirectory("/testBeam/");
  m_dir->SetGuidance("HF W calorimeter control");


  m_nRodCmd = new G4UIcmdWithAnInteger("/testBeam/nRods",this);
  m_nRodCmd->SetGuidance("Set number of rods along diagonal");
  m_nRodCmd->SetParameterName("nRods",false);
  m_nRodCmd->SetDefaultValue(20);
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
  m_lengthCmd->SetDefaultValue(15);
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

HFMessenger::~HFMessenger()
{

  delete m_nRodCmd;
  delete m_rRodCmd;
  delete m_lengthCmd;
  delete m_fibreIndexCmd;
  delete m_cladIndexCmd;

}

void HFMessenger::SetNewValue(G4UIcommand* cmd,G4String newValue)
{

  std::cout << "HF parsing commands " << cmd << " " << newValue << " | " <<
  m_nRodCmd << " " << m_rRodCmd << std::endl;

  if ( cmd == m_nRodCmd ) {
    const unsigned nRod = m_nRodCmd->GetNewIntValue(newValue);
    m_detector->SetNRods(nRod);
  } else if ( cmd == m_rRodCmd ) {
    const double rRod = m_rRodCmd->GetNewDoubleValue(newValue); 
    m_detector->SetRRod(rRod);
  } else if ( cmd == m_lengthCmd ) {
    const double length = m_lengthCmd->GetNewDoubleValue(newValue);
    m_detector->SetLength(length);
  } else if ( cmd == m_fibreIndexCmd ) {
    const double nFib = m_fibreIndexCmd->GetNewDoubleValue(newValue);
    m_detector->SetFibreIndex(nFib);
  } else if ( cmd == m_cladIndexCmd ) {
    const double nClad = m_cladIndexCmd->GetNewDoubleValue(newValue);
    m_detector->SetCladIndex(nClad);
  }


}
