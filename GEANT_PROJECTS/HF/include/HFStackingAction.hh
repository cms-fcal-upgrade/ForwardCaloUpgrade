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
// $Id: HFStackingAction.hh,v 1.1 2013/03/13 10:34:15 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HFStackingAction_H
#define HFStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"
#include "HFDataFormat.hh"

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
    inline void SetFibreIndex(double n) 
      { 
	m_nFibre = n; 
	m_fibreNA = -1.; // force recalculation
      }

    // set the cladding index of refraction
    inline void SetCladIndex(double n) 
      { 
	m_nClad = n; 
	m_fibreNA = -1.;
      }
  
    // set the low end of the optical photon wavelength cutoff
    inline void SetOptLowCut(double c) { m_lCutLow = c; }

    // -- accessor methods
    // get the fibre index of refraction
    inline double GetFibreIndex() const { return m_nFibre; }

    // get the cladding index of refraction
    inline double GetCladIndex() const { return m_nClad; }

    // get the low wavelength cutoff for optical photons
    inline double GetOptLowCut() const { return m_lCutLow; }


  private:
    G4int gammaCounter;

    double m_lCutLow; // cut off on low end of wavelength
    double m_fibreNA; // quartz fibre numerical aperature

    double m_nFibre;
    double m_nClad;

    HFDataFormat * m_df;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

