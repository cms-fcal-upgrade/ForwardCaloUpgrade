#ifndef HFEventAction_hh
#define HFEventAction_hh

#include "globals.hh"
#include "G4UserEventAction.hh"
#include "HFDataFormat.hh"

class G4Timer;

class HFEventAction : public G4UserEventAction
{
  public:
    HFEventAction(HFDataFormat *df);
    ~HFEventAction();

    void BeginOfEventAction(const G4Event *);
    void EndOfEventAction(const G4Event *);

  private:
    HFDataFormat *m_df;
    G4Timer* timer;
};
    



#endif

