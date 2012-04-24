//
// ********************************************************************
// ********************************************************************
//

#include "PrimaryGeneratorActionMessenger.hh"

#include <sstream>

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorActionMessenger::PrimaryGeneratorActionMessenger(PrimaryGeneratorAction* man)
:PrimAction(man)
{ 
  SetVxSmearCmd = new G4UIcmdWith3Vector("/gun/setVxSmearing",this);
  SetVxSmearCmd->SetGuidance("Set y-z smearing of the primary vertex.");
  SetVxSmearCmd->SetParameterName("OnOff","Type","dRwidth",true);
  SetVxSmearCmd->SetRange("OnOff>=0 && Type>=0 && dRwidth>=0");
 
  SetVxPositionCmd = new G4UIcmdWith3Vector("/gun/setVxPosition",this);
  SetVxPositionCmd->SetGuidance(" Set x,y and z of the primary vertex.");
  SetVxPositionCmd->SetParameterName("Vx","Vy","Vz",true);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorActionMessenger::~PrimaryGeneratorActionMessenger()
{
  delete SetVxSmearCmd;
  delete SetVxPositionCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == SetVxSmearCmd )
   { PrimAction->SetVxSmearing(SetVxSmearCmd->GetNew3VectorValue(newValue));}

  if( command == SetVxPositionCmd ) 
   { PrimAction->SetVxPosition(SetVxPositionCmd->GetNew3VectorValue(newValue));}    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
