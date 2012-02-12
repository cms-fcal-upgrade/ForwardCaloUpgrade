//
// ********************************************************************
// ********************************************************************
//

#include "DetectorMessenger.hh"

#include <sstream>

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:Detector(Det)
{ 
  ecalDir = new G4UIdirectory("/ecal/");
  ecalDir->SetGuidance("UI commands specific to this example");
  
  detDir = new G4UIdirectory("/ecal/det/");
  detDir->SetGuidance("detector construction commands");

  NbLayersCmd = new G4UIcmdWithAnInteger("/ecal/det/setNbOfLayers",this);
  NbLayersCmd->SetGuidance("Set number of Ecal layers.");
  NbLayersCmd->SetParameterName("NbLayers",false);
  NbLayersCmd->SetRange("NbLayers>0");
  NbLayersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  HcaMaterCmd = new G4UIcmdWithAString("/ecal/det/setHcalAbsMat",this);
  HcaMaterCmd->SetGuidance("Select Material of the Hcal Absorber.");
  HcaMaterCmd->SetParameterName("choice",false);
  HcaMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  AbsMaterCmd = new G4UIcmdWithAString("/ecal/det/setEcalAbsMat",this);
  AbsMaterCmd->SetGuidance("Select Material of the Ecal Absorber.");
  AbsMaterCmd->SetParameterName("choice",false);
  AbsMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  AbsThickCmd = new G4UIcmdWithADoubleAndUnit("/ecal/det/setEcalAbsThick",this);
  AbsThickCmd->SetGuidance("Set Thickness of the Ecal Absorber");
  AbsThickCmd->SetParameterName("Size",false);
  AbsThickCmd->SetRange("Size>=0. && Size<=220.");
  AbsThickCmd->SetUnitCategory("Length");
  AbsThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SensMaterCmd = new G4UIcmdWithAString("/ecal/det/setEcalSensMat",this);
  SensMaterCmd->SetGuidance("Select Sensitive Material of Ecal");
  SensMaterCmd->SetParameterName("choice",false);
  SensMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SensThickCmd = new G4UIcmdWithADoubleAndUnit("/ecal/det/setEcalSensThick",this);
  SensThickCmd->SetGuidance("Set Thickness of the Ecal Sensitive");
  SensThickCmd->SetParameterName("Size",false);
  SensThickCmd->SetRange("Size>0.0 && Size<=220.");
  SensThickCmd->SetUnitCategory("Length");  
  SensThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  LBinSensCmd = new G4UIcmdWith3Vector("/ecal/det/setSensLbin",this);
  LBinSensCmd->SetGuidance("set longitudinal bining");
  LBinSensCmd->SetGuidance("nb of bins; bin thickness (in cm)");
  LBinSensCmd->SetParameterName("LtRee","nLtot","dLbin",true);
  LBinSensCmd->SetRange("LtRee>=0 && nLtot>=1 && dLbin>0");
  LBinSensCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  RBinSensCmd = new G4UIcmdWith3Vector("/ecal/det/setSensRbin",this);
  RBinSensCmd->SetGuidance("set radial bining");
  RBinSensCmd->SetGuidance("nb of bins; bin thickness (in cm)");
  RBinSensCmd->SetParameterName("RtRee","nRtot","dRbin",true);
  RBinSensCmd->SetRange("RtRee>=0 && nRtot>=1 && dRbin>0");
  RBinSensCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  RBinHcalCmd = new G4UIcmdWith3Vector("/ecal/det/setHcalRbin",this);
  RBinHcalCmd->SetGuidance("set radial bining");
  RBinHcalCmd->SetGuidance("nb of bins; bin thickness (in cm)");
  RBinHcalCmd->SetParameterName("RtReeH","nRtotH","dRbinH",true);
  RBinHcalCmd->SetRange("RtReeH>=0 && nRtotH>=1 && dRbinH>0");
  RBinHcalCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/ecal/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);

  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/ecal/det/setMagField",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false);
  MagFieldCmd->SetUnitCategory("Magnetic flux density");
  MagFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete NbLayersCmd;
  delete AbsMaterCmd;
  delete AbsThickCmd; 
  delete SensMaterCmd;
  delete SensThickCmd;
  delete LBinSensCmd;
  delete RBinSensCmd;
  delete MagFieldCmd;
  delete UpdateCmd;
  delete detDir;
  delete ecalDir;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == NbLayersCmd )
   { Detector->SetNbOfEcalLayers(NbLayersCmd->GetNewIntValue(newValue));}

  if( command == HcaMaterCmd )
   { Detector->SetHcalAbsMaterial(newValue);}

  if( command == SensMaterCmd )
   { Detector->SetEcalSensMaterial(newValue);}

  if( command == AbsMaterCmd )
   { Detector->SetEcalAbsMaterial(newValue);}

  if( command == AbsThickCmd )
   { Detector->SetEcalAbsThickness(AbsThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == SensThickCmd )
   { Detector->SetEcalSensThickness(SensThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

  if( command == MagFieldCmd )
   { Detector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}

  if( command == LBinSensCmd )
   { Detector->SetSensLBining(LBinSensCmd->GetNew3VectorValue(newValue));}

  if( command == RBinSensCmd ) 
   { Detector->SetSensRBining(RBinSensCmd->GetNew3VectorValue(newValue));}    

  if( command == RBinHcalCmd ) 
   { Detector->SetHcalRBining(RBinHcalCmd->GetNew3VectorValue(newValue));}    

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
