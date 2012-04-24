//
// ********************************************************************

#ifndef PrimaryGeneratorActionMessenger_h
#define PrimaryGeneratorActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWith3Vector;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorActionMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorActionMessenger(PrimaryGeneratorAction* );
   ~PrimaryGeneratorActionMessenger();

    void SetNewValue(G4UIcommand*, G4String);

  private:

    
    PrimaryGeneratorAction*    PrimAction;

    G4UIcmdWith3Vector*        SetVxSmearCmd;
    G4UIcmdWith3Vector*        SetVxPositionCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

