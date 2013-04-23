#include "HFPrimaryGeneratorAction.hh"
#include "HFPrimaryGeneratorMessenger.hh"
#include "HFDataFormat.hh"

#include "ParticleGunGenerator.hh"
#include "HepMCG4AsciiReader.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"


HFPrimaryGeneratorAction::HFPrimaryGeneratorAction(void)
{
  fMessenger = new HFPrimaryGeneratorMessenger(this);
  fPrimaryGenerator = new ParticleGunGenerator();
}

HFPrimaryGeneratorAction::~HFPrimaryGeneratorAction(void)
{
  if (fPrimaryGenerator) delete fPrimaryGenerator;
  delete fMessenger;
}

void HFPrimaryGeneratorAction::SetGenerator(G4String genname)
{

  fGenName = genname;

  if (fPrimaryGenerator) {
    // First dispose of the old generator
    delete fPrimaryGenerator;
  }

  if (fGenName == "particleGun") {
    fPrimaryGenerator = new ParticleGunGenerator();
  }
  else if (fGenName == "hepmcAscii") {
    fPrimaryGenerator = new HepMCG4AsciiReader();
  }
  else {
    G4cout << "Abort: " << fGenName << "is not available. Please use particleGun and hepmcAscii." << G4endl;
    abort();
  }

}

void HFPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if (fPrimaryGenerator) {
    fPrimaryGenerator->GeneratePrimaryVertex(anEvent);

    if ( fGenName == "particleGun") {
      if ( m_df ) {
        double x = dynamic_cast<ParticleGunGenerator*>(fPrimaryGenerator)->GetGunPosition()[0];
        double y = dynamic_cast<ParticleGunGenerator*>(fPrimaryGenerator)->GetGunPosition()[1];
        GeneratorStruct gs(x,y);
        m_df->fillGenerator(gs);
      }
    }
  }
  else{
    G4cout << "generator is not instantiated." << G4endl;
    abort();
  }
}

