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
//
// $Id: HFDetectorConstruction.hh,v 1.3 2013/03/20 16:38:48 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HFDetectorConstruction_h
#define HFDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "HFMessenger.hh"

class G4LogicalVolume;
class G4PhysicalVolume;
class G4Material;
class G4Box;
class G4Tubs;

class HFStackingAction;
class HFPrimaryGeneratorAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HFDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    HFDetectorConstruction();
   ~HFDetectorConstruction();

  public:
    G4VPhysicalVolume* Construct();

    //  set the number of W rods across the main row
    void SetNRods(G4int n);

    // set the radius of the tungsten rod (the fibre radius is selected to fill the gap)
    void SetRRod(G4double r);

    // set the length of the calorimeter (longitudinal)
    void SetLength(G4double l);

    // set the fibre index of refraction
    void SetFibreIndex(G4double n);

    // set the cladding index of refraction
    void SetCladIndex(G4double n);

    // allow access to the user stacking action class HFStackingAction
    void SetStackingAction( HFStackingAction * sa);

    // allow access to the particle gun
    void SetParticleGun( HFPrimaryGeneratorAction *gun);

    // ---- accessor methods -----
    void PrintCalorParameters() { return; }
    
    // 
    G4int GetNRods() { return m_nRods; }
    G4double GetRRod() { return m_rRod; }
    G4double GetLength() { return m_length; }
    G4double GetFibreIndex() { return m_nFib; }
    G4double GetCladIndex() { return m_nClad; }

  private:
   // -------------- private member functions --------
   void DefineMaterials();
   void SetupWorld();
   void SetupGeometry();
   void SetupDetectors(); 
   void CalculateConstants();

   void ClearPhysicalVolumes();
   void ClearLogicalVolumes();


   //  ------------- private data -------------------
   G4bool m_isConstructed;

    // parameters
    G4double m_expHall_x;
    G4double m_expHall_y;
    G4double m_expHall_z;

    G4int m_nRods;
    G4double m_rRod;
    G4double m_nFib;
    G4double m_nClad;
    G4double m_length;
    G4double m_length2;

    G4double m_h;
    G4double m_l;
    G4double m_y;
    G4double m_rFib;

    // materials
    G4Material * m_air;
    G4Material * m_quartz;
    G4Material * m_tungsten;
    G4Material * m_iron;

    // primitives
    G4Box * m_expHall_box;
    G4Tubs * m_tungRod;
    G4Tubs * m_qFibre;

    // logical volumes
    G4LogicalVolume * m_expHall_log;
    G4LogicalVolume * m_tungRod_log;
    G4LogicalVolume * m_qFibre_log;

    // physical volumes
    G4VPhysicalVolume * m_expHall_phys;
    std::vector<G4VPhysicalVolume *> m_rods;
    std::vector<G4VPhysicalVolume *> m_fibres;

    HFMessenger *m_messenger;
    HFStackingAction * m_stacking;
    HFPrimaryGeneratorAction *m_gun;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*HFDetectorConstruction_h*/
