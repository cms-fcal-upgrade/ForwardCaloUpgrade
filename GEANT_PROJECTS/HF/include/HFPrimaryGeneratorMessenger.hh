// $Id: HFPrimaryGeneratorMessenger.hh,v 1.2 2013/03/28 20:25:13 cowden Exp $

#ifndef HFPrimaryGeneratorMessenger_h
#define HFPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class HFPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class HFPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    HFPrimaryGeneratorMessenger(HFPrimaryGeneratorAction*);
   ~HFPrimaryGeneratorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    G4String GetCurrentValue(G4UIcommand* command);
    
  private:
    HFPrimaryGeneratorAction*    HFAction;
    G4UIdirectory*               dir; 
    G4UIcmdWithAString*          GenTypeCmd;
};

#endif

