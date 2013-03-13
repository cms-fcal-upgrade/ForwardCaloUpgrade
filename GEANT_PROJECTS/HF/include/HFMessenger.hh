#ifndef HF_HFMessenger_hh
#define HF_HFMessenger_hh

#include "G4UImessenger.hh"
#include "globals.hh"

class HFDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;

class HFMessenger: public G4UImessenger
{
  public:
   HFMessenger(HFDetectorConstruction*);
   ~HFMessenger();

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
   

};

#endif
