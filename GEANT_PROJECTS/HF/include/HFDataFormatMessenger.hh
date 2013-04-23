#ifndef HF_HFDataFormatMessenger_hh
#define HF_HFDataFormatMessenger_hh

#include "G4UImessenger.hh"
#include "globals.hh"

class HFDataFormat;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;

class HFDataFormatMessenger: public G4UImessenger
{
  public:
   HFDataFormatMessenger(HFDataFormat*);
   ~HFDataFormatMessenger();

   void SetNewValue(G4UIcommand*, G4String);

  private:
   HFDataFormat * m_dataformat;
   G4UIdirectory * m_dir;

   // the commands
   G4UIcmdWithABool * m_storeOpticalInfoCmd;   
   G4UIcmdWithABool * m_storeParticleInfoCmd;   
   G4UIcmdWithABool * m_storeGeneratorInfoCmd;  
   G4UIcmdWithoutParameter * m_genTreeCmd; 

};

#endif
