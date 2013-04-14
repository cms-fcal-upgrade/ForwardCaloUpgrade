
// copied over from Mokka
// *********************************************************
// *                         Mokka                         *
// *    -- A detailed Geant 4 simulation for the ILC --    *
// *                                                       *
// *  polywww.in2p3.fr/geant4/tesla/www/mokka/mokka.html   *
// *********************************************************

#include "ParticleGunMessenger.hh"
#include "ParticleGunGenerator.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

ParticleGunMessenger::ParticleGunMessenger(ParticleGunGenerator *primaryGenerator)
{
  fPrimaryGenerator = primaryGenerator;

  fGunPositionStepCmd = new G4UIcmdWith3VectorAndUnit("/gun/positionStep", this);
  fGunPositionStepCmd->SetGuidance("Set a step for the position of the particle.");
  fGunPositionStepCmd->SetGuidance("The step will be applied after each shot.");
  fGunPositionStepCmd->SetParameterName("deltaX", "deltaY", "deltaZ", true);
  fGunPositionStepCmd->SetDefaultValue(G4ThreeVector(0, 0, 0));
  fGunPositionStepCmd->SetDefaultUnit("cm"); // as in "/gun/position"
  
  fGunPositionSmearingCmd = new G4UIcmdWith3VectorAndUnit("/gun/positionSmearing", this);
  fGunPositionSmearingCmd->SetGuidance("Set an uncertainty for the position of the particle.");
  fGunPositionSmearingCmd->SetParameterName("sigmaX", "sigmaY", "sigmaZ", true);
  fGunPositionSmearingCmd->SetDefaultValue(G4ThreeVector(0, 0, 0));
  fGunPositionSmearingCmd->SetDefaultUnit("cm"); // as in "/gun/position"
  
  fGunThetaStepCmd = new G4UIcmdWithADoubleAndUnit("/gun/thetaStep", this);
  fGunThetaStepCmd->SetGuidance("Set a step for the theta direction of the particle.");
  fGunThetaStepCmd->SetGuidance("The step will be applied after each shot.");
  fGunThetaStepCmd->SetParameterName("deltaTheta", true);
  fGunThetaStepCmd->SetDefaultValue(0);
  fGunThetaStepCmd->SetDefaultUnit("deg"); // as in "/vis/viewer/set/..."
  
  fGunThetaSmearingCmd = new G4UIcmdWithADoubleAndUnit("/gun/thetaSmearing", this);
  fGunThetaSmearingCmd->SetGuidance("Set an uncertainty for the theta direction of the particle.");
  fGunThetaSmearingCmd->SetGuidance("See also \"/gun/directionSmearingMode\".");
  fGunThetaSmearingCmd->SetParameterName("sigmaTheta", true);
  fGunThetaSmearingCmd->SetDefaultValue(0);
  fGunThetaSmearingCmd->SetDefaultUnit("deg"); // as in "/vis/viewer/set/..."
  
  fGunPhiStepCmd = new G4UIcmdWithADoubleAndUnit("/gun/phiStep", this);
  fGunPhiStepCmd->SetGuidance("Set a step for the phi direction of the particle.");
  fGunPhiStepCmd->SetGuidance("The step will be applied after each shot.");
  fGunPhiStepCmd->SetParameterName("deltaPhi", true);
  fGunPhiStepCmd->SetDefaultValue(0);
  fGunPhiStepCmd->SetDefaultUnit("deg"); // as in "/vis/viewer/set/..."
  
  fGunPhiSmearingCmd = new G4UIcmdWithADoubleAndUnit("/gun/phiSmearing", this);
  fGunPhiSmearingCmd->SetGuidance("Set an uncertainty for the phi direction of the particle.");
  fGunPhiSmearingCmd->SetGuidance("See also \"/gun/directionSmearingMode\".");
  fGunPhiSmearingCmd->SetParameterName("sigmaPhi", true);
  fGunPhiSmearingCmd->SetDefaultValue(0);
  fGunPhiSmearingCmd->SetDefaultUnit("deg"); // as in "/vis/viewer/set/..."
  
  fGunMomentumCmd = new G4UIcmdWithADoubleAndUnit("/gun/momentum", this);
  fGunMomentumCmd->SetGuidance("Set the momentum of the currently chosen particle.");
  fGunMomentumCmd->SetGuidance("This command will overwrite the kinetic energy.");
  fGunMomentumCmd->SetGuidance("See also \"/gun/energy\".");
  fGunMomentumCmd->SetParameterName("momentum", false); // not omittable
  fGunMomentumCmd->SetDefaultUnit("GeV"); // as in "/gun/energy"
  
  fGunMomentumStepCmd = new G4UIcmdWithADoubleAndUnit("/gun/momentumStep", this);
  fGunMomentumStepCmd->SetGuidance("Set a step for the momentum of the particle.");
  fGunMomentumStepCmd->SetGuidance("The step will be applied after each shot.");
  fGunMomentumStepCmd->SetParameterName("deltaMomentum", true);
  fGunMomentumStepCmd->SetDefaultValue(0);
  fGunMomentumStepCmd->SetDefaultUnit("GeV"); // as in "/gun/energy"
  
  fGunMomentumSmearingCmd = new G4UIcmdWithADoubleAndUnit("/gun/momentumSmearing", this);
  fGunMomentumSmearingCmd->SetGuidance("Set an uncertainty for the momentum of the particle.");
  fGunMomentumSmearingCmd->SetParameterName("sigmaMomentum", true);
  fGunMomentumSmearingCmd->SetDefaultValue(0);
  fGunMomentumSmearingCmd->SetDefaultUnit("GeV"); // as in "/gun/energy"
  
  fGunDirectionSmearingModeCmd = new G4UIcmdWithAString("/gun/directionSmearingMode", this);
  fGunDirectionSmearingModeCmd->SetGuidance("Select a smearing mode for the angular uncertainties.");
  fGunDirectionSmearingModeCmd->SetGuidance("  \"gaussian\" - smearing values mean gaussian sigma");
  fGunDirectionSmearingModeCmd->SetGuidance("  \"uniform\"  - smearing values mean half the width");
  fGunDirectionSmearingModeCmd->SetGuidance("See also \"/gun/thetaSmearing\" and \"/gun/phiSmearing\".");
  fGunDirectionSmearingModeCmd->SetParameterName("smearingMode", false); // not omittable
  fGunDirectionSmearingModeCmd->SetCandidates("gaussian uniform");
  
  fGunMomentumSmearingModeCmd = new G4UIcmdWithAString("/gun/momentumSmearingMode", this);
  fGunMomentumSmearingModeCmd->SetGuidance("Select a smearing mode for the momentum uncertainties.");
  fGunMomentumSmearingModeCmd->SetGuidance("  \"gaussian\" - smearing values mean gaussian sigma");
  fGunMomentumSmearingModeCmd->SetGuidance("  \"uniform\"  - smearing values mean half the width");
  fGunMomentumSmearingModeCmd->SetGuidance("See also \"/gun/momentumSmearing\".");
  fGunMomentumSmearingModeCmd->SetParameterName("smearingMode", false); // not omittable
  fGunMomentumSmearingModeCmd->SetCandidates("gaussian uniform");

  
  fGunInfoCmd = new G4UIcmdWithoutParameter("/gun/info", this);
  fGunInfoCmd->SetGuidance("Print some information about the next shot.");

  fGunOptPhotonPolarCmd = new G4UIcmdWithADoubleAndUnit("/gun/GunOptPhotonPolar",this);
  fGunOptPhotonPolarCmd->SetGuidance("Set linear polarization");
  fGunOptPhotonPolarCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fGunOptPhotonPolarCmd->SetParameterName("GunOptPhotonPolar",true);
  fGunOptPhotonPolarCmd->SetUnitCategory("Angle");
  fGunOptPhotonPolarCmd->SetDefaultValue(-360.0);
  fGunOptPhotonPolarCmd->SetDefaultUnit("deg");
  fGunOptPhotonPolarCmd->AvailableForStates(G4State_Idle);

}

ParticleGunMessenger::~ParticleGunMessenger(void)
{
  delete fGunInfoCmd;
  delete fGunMomentumSmearingModeCmd;
  delete fGunDirectionSmearingModeCmd;
  delete fGunMomentumSmearingCmd;
  delete fGunMomentumStepCmd;
  delete fGunMomentumCmd;
  delete fGunPhiSmearingCmd;
  delete fGunPhiStepCmd;
  delete fGunThetaSmearingCmd;
  delete fGunThetaStepCmd;
  delete fGunPositionSmearingCmd;
  delete fGunPositionStepCmd;
  delete fGunOptPhotonPolarCmd;
}

void ParticleGunMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
  if (command == fGunPositionStepCmd)          fPrimaryGenerator->SetGunPositionStep(fGunPositionStepCmd->GetNew3VectorValue(newValue));
  else if (command == fGunPositionSmearingCmd) fPrimaryGenerator->SetGunPositionSmearing(fGunPositionSmearingCmd->GetNew3VectorValue(newValue));
  else if (command == fGunThetaStepCmd)        fPrimaryGenerator->SetGunThetaStep(fGunThetaStepCmd->GetNewDoubleValue(newValue));
  else if (command == fGunThetaSmearingCmd)    fPrimaryGenerator->SetGunThetaSmearing(fGunThetaSmearingCmd->GetNewDoubleValue(newValue));
  else if (command == fGunPhiStepCmd)          fPrimaryGenerator->SetGunPhiStep(fGunPhiStepCmd->GetNewDoubleValue(newValue));
  else if (command == fGunPhiSmearingCmd)      fPrimaryGenerator->SetGunPhiSmearing(fGunPhiSmearingCmd->GetNewDoubleValue(newValue));
  else if (command == fGunMomentumCmd)         fPrimaryGenerator->SetGunMomentum(fGunMomentumCmd->GetNewDoubleValue(newValue));
  else if (command == fGunMomentumStepCmd)     fPrimaryGenerator->SetGunMomentumStep(fGunMomentumStepCmd->GetNewDoubleValue(newValue));
  else if (command == fGunMomentumSmearingCmd) fPrimaryGenerator->SetGunMomentumSmearing(fGunMomentumSmearingCmd->GetNewDoubleValue(newValue));

  else if (command == fGunDirectionSmearingModeCmd) {
    if      (newValue == "gaussian")           fPrimaryGenerator->SetGunDirectionSmearingMode(ParticleGunGenerator::kGaussian);
    else if (newValue == "uniform")            fPrimaryGenerator->SetGunDirectionSmearingMode(ParticleGunGenerator::kUniform);
  }
  else if (command == fGunMomentumSmearingModeCmd) {
    if      (newValue == "gaussian")           fPrimaryGenerator->SetGunMomentumSmearingMode(ParticleGunGenerator::kGaussian);
    else if (newValue == "uniform")            fPrimaryGenerator->SetGunMomentumSmearingMode(ParticleGunGenerator::kUniform);
  }
  else if (command == fGunInfoCmd)             fPrimaryGenerator->PrintGeneratorInfo();
  else if (command == fGunOptPhotonPolarCmd)   fPrimaryGenerator->SetOptPhotonPolar(fGunOptPhotonPolarCmd->GetNewDoubleValue(newValue));
}

G4String ParticleGunMessenger::GetCurrentValue(G4UIcommand *command)
{
  if (command == fGunPositionStepCmd)          return fGunPositionStepCmd->ConvertToString(fPrimaryGenerator->GetGunPositionStep());
  else if (command == fGunPositionSmearingCmd) return fGunPositionSmearingCmd->ConvertToString(fPrimaryGenerator->GetGunPositionSmearing());
  else if (command == fGunThetaStepCmd)        return fGunThetaStepCmd->ConvertToString(fPrimaryGenerator->GetGunThetaStep());
  else if (command == fGunThetaSmearingCmd)    return fGunThetaSmearingCmd->ConvertToString(fPrimaryGenerator->GetGunThetaSmearing());
  else if (command == fGunPhiStepCmd)          return fGunPhiStepCmd->ConvertToString(fPrimaryGenerator->GetGunPhiStep());
  else if (command == fGunPhiSmearingCmd)      return fGunPhiSmearingCmd->ConvertToString(fPrimaryGenerator->GetGunPhiSmearing());
  else if (command == fGunMomentumCmd)         return fGunMomentumCmd->ConvertToString(fPrimaryGenerator->GetGunMomentum());
  else if (command == fGunMomentumStepCmd)     return fGunMomentumStepCmd->ConvertToString(fPrimaryGenerator->GetGunMomentumStep());
  else if (command == fGunMomentumSmearingCmd) return fGunMomentumSmearingCmd->ConvertToString(fPrimaryGenerator->GetGunMomentumSmearing());

  else if (command == fGunDirectionSmearingModeCmd) {
    switch (fPrimaryGenerator->GetGunDirectionSmearingMode()) {
      case ParticleGunGenerator::kGaussian:    return "gaussian";
      case ParticleGunGenerator::kUniform:     return "uniform";
      default: return "unknown"; // this should never happen
    }
  }
  else if (command == fGunMomentumSmearingModeCmd) {
    switch (fPrimaryGenerator->GetGunMomentumSmearingMode()) {
      case ParticleGunGenerator::kGaussian:    return "gaussian";
      case ParticleGunGenerator::kUniform:     return "uniform";
      default: return "unknown"; // this should never happen
    }
  }
  else if (command == fGunOptPhotonPolarCmd)   return fGunOptPhotonPolarCmd->ConvertToString(fPrimaryGenerator->GetOptPhotonPolar());
  return G4String(); // nothing
}
