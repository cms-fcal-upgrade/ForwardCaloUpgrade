//
// ********************************************************************
// ********************************************************************
//

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

//

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* det)
:Detector(det)
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
// default particle kinematic
//============================

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="pi-");
  particleGun->SetParticleDefinition(particle);

// set particle energy and position
//==================================

  particleGun->SetParticleMomentumDirection(G4ThreeVector(1.0,0.0,0.0));
  particleGun->SetParticleEnergy(10.*GeV);
  particleGun->SetParticlePosition(G4ThreeVector(-10.0*cm,0.0*cm,0.0*cm));

}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
// this function is called at the begining of event

//  G4double x0 = -10.0*cm, y0 = 0.0*cm, z0 = 0.0*cm;
//
//  if (rndmFlag == "on")
//     {y0 = (ExN03Detector->GetCalorSizeYZ())*(G4UniformRand()-0.5);
//      z0 = (ExN03Detector->GetCalorSizeYZ())*(G4UniformRand()-0.5);
//     } 

//  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  particleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

