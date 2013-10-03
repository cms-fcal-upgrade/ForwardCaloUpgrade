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
// $Id: HFStackingAction.hh,v 1.4 2013/05/17 19:19:10 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HFStackingAction_H
#define HFStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"
#include "HFDataFormat.hh"

#include "G4ParticleDefinition.hh"

#include "TRandom.h"

class HFDetectorConstruction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HFStackingAction : public G4UserStackingAction

{
  public:
    HFStackingAction(HFDataFormat *df);
   ~HFStackingAction();

  public:
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    void NewStage();
    void PrepareNewEvent();

    // set the fibre index of refraction
    void SetFibreIndex(G4double n) 
      { 
	m_nFibre = n; 
	m_fibreNA = -1.; // force recalculation
      }

   //  set the fibre direction
   void SetFibreDirection(const G4ThreeVector & dir) 
      {
	m_fibreDir = dir;
      }

    // set the cladding index of refraction
    void SetCladIndex(G4double n) 
      { 
	m_nClad = n; 
	m_fibreNA = -1.;
      }

    // set scintillating fiber index of refraction
    void SetScinIndex(G4double n) {
      m_nScin = n;
      m_scsfNA = -1.;
    }

    void SetScinCladIndex(G4double n) {
      m_nScinClad = n;
      m_scsfNA = -1.;
    }

    // set index of refraction for photodetector
    void SetGlassIndex(G4double n) { m_nGlass = n; }

    // set air index of refraction
    void SetAirIndex(G4double n) { m_nAir = n; }
  
    // set the low end of the optical photon wavelength cutoff
    void SetOptLowCut(G4double c) { m_lCutLow = c; }

    // set the fiber radius
    inline void SetFiberRadius(double r) { m_rFibre = r; }

    // -- accessor methods
    // get the fibre index of refraction
    G4double GetFibreIndex() const { return m_nFibre; }

    // get the cladding index of refraction
    G4double GetCladIndex() const { return m_nClad; }

    // get the low wavelength cutoff for optical photons
    G4double GetOptLowCut() const { return m_lCutLow; }


  private:
    G4int gammaCounter;

    G4double m_lCutLow; // cut off on low end of wavelength
    G4double m_fibreNA; // quartz fibre numerical aperature
    G4double m_scsfNA; // scsf fibre numerical aperature

    G4double m_nFibre;
    G4double m_nClad;
    G4double m_nScin;
    G4double m_nScinClad;
    G4double m_rFibre; // fibre radius
    G4double m_nGlass;
    G4double m_nAir;
    G4double m_fibLength;

    G4ThreeVector m_fibreDir;

    HFDataFormat * m_df;

    TRandom m_r1;

    const G4ParticleDefinition * m_optDef;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

