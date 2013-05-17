#ifndef HF_HFDetectorConstructionMessenger_hh
#define HF_HFDetectorConstructionMessenger_hh

#include "G4UImessenger.hh"
#include "globals.hh"

class HFDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class HFDetectorConstructionMessenger: public G4UImessenger
{
  public:
   HFDetectorConstructionMessenger(HFDetectorConstruction*);
   ~HFDetectorConstructionMessenger();

   void SetNewValue(G4UIcommand*, G4String);

  private:
   HFDetectorConstruction * m_detector;
   G4UIdirectory * m_dir;

   // the commands
   G4UIcmdWithAnInteger * m_nRodCmd;
   G4UIcmdWithADoubleAndUnit * m_rRodCmd;
   G4UIcmdWithADoubleAndUnit * m_lengthCmd;
   G4UIcmdWithADouble * m_fibreIndexCmd;  // fibre index of refraction
   G4UIcmdWithADouble * m_cladIndexCmd;
   G4UIcmdWithABool * m_overlapCheckCmd;

};

#endif
