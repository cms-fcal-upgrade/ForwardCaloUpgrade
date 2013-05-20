#ifndef CMSHF_CMSHFDetectorConstructionMessenger_hh
#define CMSHF_CMSHFDetectorConstructionMessenger_hh

#include "G4UImessenger.hh"
#include "globals.hh"

class CMSHFDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class CMSHFDetectorConstructionMessenger: public G4UImessenger
{
  public:
   CMSHFDetectorConstructionMessenger(CMSHFDetectorConstruction*);
   ~CMSHFDetectorConstructionMessenger();

   void SetNewValue(G4UIcommand*, G4String);

  private:
   CMSHFDetectorConstruction * m_detector;
   G4UIdirectory * m_dir;

   // the commands
   G4UIcmdWithADoubleAndUnit * m_widthCmd;
   G4UIcmdWithADoubleAndUnit * m_lengthCmd;
   G4UIcmdWithADouble * m_fibreIndexCmd;  // fibre index of refraction
   G4UIcmdWithADouble * m_cladIndexCmd;
   G4UIcmdWithABool * m_overlapCheckCmd;

};

#endif
