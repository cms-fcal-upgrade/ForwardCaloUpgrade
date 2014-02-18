#ifndef HFPrimaryGeneratorAction_h
#define HFPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4Event;
class HFPrimaryGeneratorMessenger;
class G4VPrimaryGenerator;
class HFDataFormat;

class HFPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    HFPrimaryGeneratorAction(void);
   ~HFPrimaryGeneratorAction(void);

  public:
    void GeneratePrimaries(G4Event*);
    void SetGenerator(G4String);
    G4String GetGeneratorName(void)    { return fGenName; }

    void SetDataFormat(HFDataFormat *df) { m_df = df; }

  private:

    HFPrimaryGeneratorMessenger *fMessenger;
    G4VPrimaryGenerator *fPrimaryGenerator;

    G4String fGenName;
    HFDataFormat * m_df;
};


#endif
