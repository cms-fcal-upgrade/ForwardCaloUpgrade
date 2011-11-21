//
// ********************************************************************

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:
    DetectorConstruction* Detector;

    G4UIdirectory*             ecalDir;
    G4UIdirectory*             detDir;
    G4UIcmdWithAnInteger*      NbLayersCmd;
    G4UIcmdWithAString*        AbsMaterCmd;
    G4UIcmdWithAString*        SensMaterCmd;
    G4UIcmdWithADoubleAndUnit* AbsThickCmd;
    G4UIcmdWithADoubleAndUnit* SensThickCmd;
    G4UIcmdWithADoubleAndUnit* MagFieldCmd;
    G4UIcmdWithoutParameter*   UpdateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
