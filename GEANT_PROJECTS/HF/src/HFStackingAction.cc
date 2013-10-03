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
// $Id: HFStackingAction.cc,v 1.7 2013/05/20 21:57:39 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <iostream>
#include "HFStackingAction.hh"
#include "HFDetectorConstruction.hh"

#include "OptFibreTools.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFStackingAction::HFStackingAction(HFDataFormat *df)
: gammaCounter(0)
, m_lCutLow(350.)
, m_fibreNA(-1.)
, m_scsfNA(-1.)
, m_nFibre(1.457)
, m_nClad(1.419)
, m_nScin(1.59)
, m_nScinClad(1.49)
, m_rFibre(0.306*mm)
, m_nGlass(1.517)
, m_nAir(1.)
, m_fibLength(120*cm)
, m_fibreDir(0.,0.,1.)
, m_df(df)
, m_optDef( G4OpticalPhoton::OpticalPhotonDefinition() )
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFStackingAction::~HFStackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
HFStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{

  if ( m_fibreNA < 0. ) m_fibreNA = sqrt(m_nFibre*m_nFibre-m_nClad*m_nClad);
  if ( m_scsfNA < 0. ) m_scsfNA = sqrt(m_nScin*m_nScin-m_nScinClad*m_nScinClad);

  G4ClassificationOfNewTrack classification = fUrgent;

  const G4VPhysicalVolume * volume = aTrack->GetVolume();
  const G4String & vName = volume->GetName();

  if ( !volume ) return classification;

  // kill optical photon
  //if ( aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() ) classification = fKill;

  // check if track is an optical photon in a fibre
  if(aTrack->GetDefinition() == m_optDef )
  { // particle is optical photon
    if(aTrack->GetParentID()>0)
    { // particle is secondary

      // kill all photons	
      classification = fKill;

      const G4DynamicParticle *aParticle = aTrack->GetDynamicParticle();
      const double E = aParticle->GetTotalEnergy();
      const double lambda = hbarc*twopi/E*1.e+6;
      const G4ThreeVector & mom = aTrack->GetMomentumDirection();
      const double px = mom.x();
      const double py = mom.y();
      const double pz = mom.z();
      assert( fabs(mom.mag() - 1.) < 1.e-6);
      
      //////////////////////////////////////////////////
      // Find the fiber's central axis, and the displancement
      // of the photon production point from the that axis in
      // a plane perpendicular to the fiber.
      const G4ThreeVector & trans = volume->GetTranslation();

      const G4ThreeVector & touchTrans = aTrack->GetTouchable()->GetTranslation();
      //assert( trans.x() == touchTrans.x() );

      const G4ThreeVector & pos = aTrack->GetPosition();
      const double x = touchTrans.x();  // record center to fibre rather
      const double y = touchTrans.y();
      const double z = pos.z();
      const double t = aTrack->GetGlobalTime();


      Fiber fib;
      fib.radius = m_rFibre;
      fib.attenuation = 28.1*m;
      fib.lengthTotal = 1.2*m;
      fib.refrIndCore = m_nFibre;
      fib.refrIndClad = m_nClad;
      fib.refrIndAir = 1.;
      fib.refrIndDet = m_nGlass;
      fib.direction.SetX(m_fibreDir.x());
      fib.direction.SetY(m_fibreDir.y());
      fib.direction.SetZ(m_fibreDir.z());
      fib.position.SetX( touchTrans.x() );
      fib.position.SetY( touchTrans.y() );
      fib.position.SetZ( touchTrans.z() );

      Photon ph;
      ph.position.SetX( pos.x() );
      ph.position.SetY( pos.y() );
      ph.position.SetZ( pos.z() );
      ph.direction.SetX( mom.x() );
      ph.direction.SetY( mom.y() );
      ph.direction.SetZ( mom.z() );
      ph.dist = (touchTrans.z()+m_fibLength/2.-pos.z())/m_fibLength;
      assert( ph.dist <= 1. );
      assert( ph.dist >= 0. );      

      Travel trk = GetTimeAndProbability(ph,fib);
      double prob = trk.prob[0];
      double probTime = trk.time[0];
      double sumProb = 0.;
      for (int i=0; i<trk.nmax; i++ )  sumProb += trk.prob[i];
      double rndnm = m_r1.Rndm();
      bool isDetected = 0. < rndnm && rndnm < trk.prob[0];


      //////////////////////
      // This is the old method to check photon acceptance
      const double na = sqrt(px*px+py*py);
      
      //const G4VProcess * proc = aTrack->GetCreatorProcess();
      //const G4String & procName = proc ? proc->GetProcessName() : "no";
     
      if ( lambda <= m_lCutLow ) classification = fKill;

      if ( vName.contains("fib") &&  lambda > m_lCutLow && isDetected ) {
        gammaCounter++;
	StackingStruct st(lambda,E,na,x,y,z,t,probTime);
        m_df->fillStackingAction(st,fCherenkov);
      } else if ( vName.contains("scsf") &&  lambda > m_lCutLow && isDetected ) { 
        gammaCounter++;
	StackingStruct st(lambda,E,na,x,y,z,t,probTime);
        m_df->fillStackingAction(st,fScintillation);
      } else if ( vName.contains("glass")  ) {
	// kill tracks created in the glass
	classification = fKill;
      } 
    }

  } else {

    const G4DynamicParticle * particle = aTrack->GetDynamicParticle();
    const double E = particle->GetTotalEnergy();
    const int pdgId = particle->GetPDGcode();
    const G4ThreeVector & mom = aTrack->GetMomentum();
    const G4ThreeVector & pos = aTrack->GetPosition();


    ParticleStruct pt(pdgId,mom,pos,E);
    m_df->fillParticle(pt);

  }


  return classification;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HFStackingAction::NewStage()
{
  G4cout << "Number of optical photons produced in this event : "
         << gammaCounter << " R = " << m_rFibre << " NA = " << m_fibreNA << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HFStackingAction::PrepareNewEvent()
{ gammaCounter = 0; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
