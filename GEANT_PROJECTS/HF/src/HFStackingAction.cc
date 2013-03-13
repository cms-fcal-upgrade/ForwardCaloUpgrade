//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HFStackingAction.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFStackingAction::HFStackingAction(HFDataFormat *df)
: gammaCounter(0)
, m_lCutLow(350.)
, m_fibreNA(-1.)
, m_nFibre(1.457)
, m_nClad(1.419)
, m_df(df)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFStackingAction::~HFStackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
HFStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{

  if ( m_fibreNA < 0. ) m_fibreNA = sqrt(m_nFibre*m_nFibre-m_nClad*m_nClad);

  G4ClassificationOfNewTrack classification = fUrgent;

  const G4VPhysicalVolume * volume = aTrack->GetVolume();
  const G4String & vName = volume->GetName();

  // check if track is an optical photon in a fibre
  if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() && vName.contains("fib"))
  { // particle is optical photon
    if(aTrack->GetParentID()>0)
    { // particle is secondary

      classification = fKill;

      const G4DynamicParticle *aParticle = aTrack->GetDynamicParticle();
      const double E = aParticle->GetTotalEnergy();
      const double lambda = hbarc*twopi/E*1.e+6;
      const G4ThreeVector & mom = aTrack->GetMomentumDirection();
      const double x = mom.x();
      const double y = mom.y();
      const double na = sqrt(x*x+y*y);
     

      if ( lambda > m_lCutLow && na < m_fibreNA ) { 
        gammaCounter++;
	StackingStruct st(lambda,E,na);
        m_df->fillStackingAction(st);
      } 
    }

  }
  return classification;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HFStackingAction::NewStage()
{
  G4cout << "Number of optical photons produced in this event : "
         << gammaCounter << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HFStackingAction::PrepareNewEvent()
{ gammaCounter = 0; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
