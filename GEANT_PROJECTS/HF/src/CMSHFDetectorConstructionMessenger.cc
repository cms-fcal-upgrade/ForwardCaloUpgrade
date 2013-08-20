
#include "CMSHFDetectorConstructionMessenger.hh"

#include "CMSHFDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

#include <iostream>


CMSHFDetectorConstructionMessenger::CMSHFDetectorConstructionMessenger(CMSHFDetectorConstruction *det)
:m_detector(det)
{

  std::cout << "CMSHF Constructing messenger" << std::endl;
  m_dir = new G4UIdirectory("/testBeam/");
  m_dir->SetGuidance("CMSHF W calorimeter control");

  m_posCmd = new G4UIcmdWith3VectorAndUnit("/testBeam/detPosition", this);
  m_posCmd->SetGuidance("Set the position of the center of the detector element.");
  m_posCmd->SetParameterName("X","Y","Z",true);
  m_posCmd->SetDefaultValue(G4ThreeVector(0.,0.,0.7));
  m_posCmd->SetDefaultUnit("m");  
  m_posCmd->AvailableForStates(G4State_Idle);

  m_lengthCmd = new G4UIcmdWithADoubleAndUnit("/testBeam/length",this);
  m_lengthCmd->SetGuidance("Set the 1/2 Length of the calorimeter");
  m_lengthCmd->SetParameterName("length",false);
  m_lengthCmd->SetDefaultValue(60);
  m_lengthCmd->SetDefaultUnit("cm");
  m_lengthCmd->AvailableForStates(G4State_Idle);

  m_NfibCmd = new G4UIcmdWithAnInteger("/testBeam/Nfib",this);
  m_NfibCmd->SetGuidance("Set the number of fibers along side of segment");
  m_NfibCmd->SetParameterName("Nfib",false);
  m_NfibCmd->SetDefaultValue(32);
  m_NfibCmd->AvailableForStates(G4State_Idle);

  m_NsegCmd = new G4UIcmdWithAnInteger("/testBeam/Nseg",this);
  m_NsegCmd->SetGuidance("Set the number of segments along side of detector");
  m_NsegCmd->SetParameterName("Nseg",false);
  m_NsegCmd->SetDefaultValue(13);
  m_NsegCmd->AvailableForStates(G4State_Idle);

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

  m_fieldCmd = new G4UIcmdWith3VectorAndUnit("/testBeam/magField", this);
  m_fieldCmd->SetGuidance("Define a uniform magnetic field");
  m_fieldCmd->SetParameterName("X","Y","Z",true);
  m_fieldCmd->SetDefaultValue(G4ThreeVector(0.,0.,0.7));
  m_fieldCmd->SetDefaultUnit("tesla");  
  m_fieldCmd->AvailableForStates(G4State_Idle);


}

CMSHFDetectorConstructionMessenger::~CMSHFDetectorConstructionMessenger()
{

  delete m_posCmd;
  delete m_lengthCmd;
  delete m_NfibCmd;
  delete m_NsegCmd;
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
  else if ( cmd == m_posCmd ) {
    m_detector->SetPositionXYZ(m_posCmd->GetNew3VectorValue(newValue));
  }
  else if ( cmd == m_NfibCmd ) {
    m_detector->SetNFibSeg(m_NfibCmd->GetNewIntValue(newValue));
  }
  else if ( cmd == m_NsegCmd ) {
    m_detector->SetNSeg(m_NsegCmd->GetNewIntValue(newValue));
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
  else if ( cmd == m_fieldCmd ) {
    m_detector->SetMagneticField(m_fieldCmd->GetNew3VectorValue(newValue));
  }

}
