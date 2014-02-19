#ifndef HFSteppingAction_hh
#define HFSteppingAction_hh

//////////////////////////////////////////
// C S Cowden    2013 April 12
// HF Tungsten porotype simlation stepping action
// primarily implemented to retrieve optical photons
// crossing end of fibers.
/////////////////////////////////////////

#include "G4UserSteppingAction.hh"

#include "G4ParticleDefinition.hh"

class HFDataFormat;

class HFSteppingAction: public G4UserSteppingAction {

public:
  HFSteppingAction(HFDataFormat *df);
  ~HFSteppingAction();

  virtual void UserSteppingAction(const G4Step *);

private:
  HFDataFormat * m_df; 
  const G4ParticleDefinition * m_optDef;
 
};

#endif
