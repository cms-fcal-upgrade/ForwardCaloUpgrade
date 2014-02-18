#include "HFPrimaryGeneratorMessenger.hh"
#include "HFPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4SystemOfUnits.hh"

HFPrimaryGeneratorMessenger::HFPrimaryGeneratorMessenger(HFPrimaryGeneratorAction* HFGun)
:HFAction(HFGun)
{
  dir = new G4UIdirectory("/generator/");
  dir->SetGuidance("Control commands for primary generator");

  GenTypeCmd = new G4UIcmdWithAString("/generator/GenType", this);
  GenTypeCmd->SetGuidance("select generator type");
  GenTypeCmd->SetParameterName("GenType", false, false);
  GenTypeCmd->SetCandidates("particleGun hepmcAscii");
  GenTypeCmd->SetDefaultValue("particleGun");   

}

HFPrimaryGeneratorMessenger::~HFPrimaryGeneratorMessenger()
{
  delete dir;
  delete GenTypeCmd;
}


void HFPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{
  if (command==GenTypeCmd){
    HFAction->SetGenerator(newValue);
  }
}

G4String HFPrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv, st;
  if (command == GenTypeCmd) {
    cv = HFAction->GetGeneratorName();
  }

 return cv;
}
