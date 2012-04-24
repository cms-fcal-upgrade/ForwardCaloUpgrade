//
// ********************************************************************
// ********************************************************************
//

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"

//class G4ParticleGun;
class G4Event;
class DetectorConstruction;
class PrimaryGeneratorActionMessenger;
//

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction(DetectorConstruction*);    
  virtual ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*);
  G4ParticleGun* GetParticleGun() {return particleGun;};

  void SetVxSmearing(G4ThreeVector);
  void SetVxPosition(G4ThreeVector);

private:
  PrimaryGeneratorActionMessenger* actMessenger;
  G4ParticleGun*                particleGun;	  //pointer a to G4  class
  DetectorConstruction*         Detector;         //pointer to the geometry

  G4int                         smearFlag, smearErea;
  G4double                      xVertex, yVertex, zVertex;
  G4double                      dRwidth;
  G4bool                        vertexDefined; 
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


