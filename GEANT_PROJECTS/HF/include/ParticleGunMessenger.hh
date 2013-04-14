// copied over from Mokka.
// *********************************************************
// *                         Mokka                         *
// *    -- A detailed Geant 4 simulation for the ILC --    *
// *                                                       *
// *  polywww.in2p3.fr/geant4/tesla/www/mokka/mokka.html   *
// *********************************************************
//
#ifndef ParticleGunMessenger_hh
#define ParticleGunMessenger_hh 1

#include "G4UImessenger.hh"
#include "globals.hh"

class ParticleGunGenerator;

class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;

class ParticleGunMessenger: public G4UImessenger
{
public:
  ParticleGunMessenger(ParticleGunGenerator *primaryGenerator);
  ~ParticleGunMessenger(void);
    
  void SetNewValue(G4UIcommand *command, G4String newValues);
  G4String GetCurrentValue(G4UIcommand *command);

private:
  ParticleGunGenerator      *fPrimaryGenerator;
    
  G4UIcmdWith3VectorAndUnit *fGunPositionStepCmd;
  G4UIcmdWith3VectorAndUnit *fGunPositionSmearingCmd;
  G4UIcmdWithADoubleAndUnit *fGunThetaStepCmd;
  G4UIcmdWithADoubleAndUnit *fGunThetaSmearingCmd;
  G4UIcmdWithADoubleAndUnit *fGunPhiStepCmd;
  G4UIcmdWithADoubleAndUnit *fGunPhiSmearingCmd;
  G4UIcmdWithADoubleAndUnit *fGunMomentumCmd;
  G4UIcmdWithADoubleAndUnit *fGunMomentumStepCmd;
  G4UIcmdWithADoubleAndUnit *fGunMomentumSmearingCmd;
  G4UIcmdWithAString        *fGunDirectionSmearingModeCmd;
  G4UIcmdWithAString        *fGunMomentumSmearingModeCmd;
  G4UIcmdWithoutParameter   *fGunInfoCmd;
  G4UIcmdWithADoubleAndUnit *fGunOptPhotonPolarCmd;
};

#endif
