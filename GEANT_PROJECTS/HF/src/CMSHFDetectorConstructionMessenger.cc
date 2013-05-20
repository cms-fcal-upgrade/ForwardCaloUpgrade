
#include "CMSHFDetectorConstructionMessenger.hh"

#include "CMSHFDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

#include <iostream>


CMSHFDetectorConstructionMessenger::CMSHFDetectorConstructionMessenger(CMSHFDetectorConstruction *det)
:m_detector(det)
{

  std::cout << "CMSHF Constructing messenger" << std::endl;
  m_dir = new G4UIdirectory("/testBeam/");
  m_dir->SetGuidance("CMSHF W calorimeter control");


  m_lengthCmd = new G4UIcmdWithADoubleAndUnit("/testBeam/length",this);
  m_lengthCmd->SetGuidance("Set the 1/2 Length of the calorimeter");
  m_lengthCmd->SetParameterName("length",false);
  m_lengthCmd->SetDefaultValue(60);
  m_lengthCmd->SetDefaultUnit("cm");
  m_lengthCmd->AvailableForStates(G4State_Idle);

  m_widthCmd = new G4UIcmdWithADoubleAndUnit("/testBeam/width",this);
  m_widthCmd->SetGuidance("Set the 1/2 width of the tungsten block");
  m_widthCmd->SetParameterName("width",false);
  m_widthCmd->SetDefaultValue(25);
  m_widthCmd->SetDefaultUnit("cm");
  m_widthCmd->AvailableForStates(G4State_Idle);

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

  m_overlapCheckCmd = new G4UIcmdWithABool("/testBeam/checkOverlaps",this);
  m_overlapCheckCmd->SetGuidance("Set flag to check geometry for overlapping volumes");
  m_overlapCheckCmd->SetDefaultValue(false);
  m_overlapCheckCmd->AvailableForStates(G4State_Idle);

}

CMSHFDetectorConstructionMessenger::~CMSHFDetectorConstructionMessenger()
{

  delete m_lengthCmd;
  delete m_widthCmd;
  delete m_fibreIndexCmd;
  delete m_cladIndexCmd;
  delete m_overlapCheckCmd;

}

void CMSHFDetectorConstructionMessenger::SetNewValue(G4UIcommand* cmd,G4String newValue)
{

  std::cout << "CMSHF parsing commands " << cmd << " " << newValue << std::endl;

  if ( cmd == m_lengthCmd ) {
    m_detector->SetLength(m_lengthCmd->GetNewDoubleValue(newValue));
  } 
  else if ( cmd == m_widthCmd ) {
    m_detector->SetWidth(m_widthCmd->GetNewDoubleValue(newValue));
  }
  else if ( cmd == m_fibreIndexCmd ) {
    m_detector->SetFibreIndex(m_fibreIndexCmd->GetNewDoubleValue(newValue));
  } 
  else if ( cmd == m_cladIndexCmd ) {
    m_detector->SetCladIndex(m_cladIndexCmd->GetNewDoubleValue(newValue));
  }
  else if ( cmd == m_overlapCheckCmd ) {
    m_detector->SetOverlapCheck(m_overlapCheckCmd->GetNewBoolValue(newValue));
  }

}
