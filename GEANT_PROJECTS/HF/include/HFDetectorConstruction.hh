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
// $Id: HFDetectorConstruction.hh,v 1.2 2013/03/15 11:23:31 cowden Exp $
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
    void SetNRods(unsigned n);

    // set the radius of the tungsten rod (the fibre radius is selected to fill the gap)
    void SetRRod(double r);

    // set the length of the calorimeter (longitudinal)
    void SetLength(double l);

    // set the fibre index of refraction
    void SetFibreIndex(double n);

    // set the cladding index of refraction
    void SetCladIndex(double n);

    // allow access to the user stacking action class HFStackingAction
    void SetStackingAction( HFStackingAction * sa);

    // allow access to the particle gun
    void SetParticleGun( HFPrimaryGeneratorAction *gun);

    // ---- accessor methods -----
    inline void PrintCalorParameters() const { return; }
    
    // 
    inline double GetNRods() const { return m_nRods; }
    inline double GetRRod() const { return m_rRod; }
    inline double GetLength() const { return m_length; }
    inline double GetFibreIndex() const { return m_nFib; }
    inline double GetCladIndex() const { return m_nClad; }

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
   bool m_isConstructed;

    // parameters
    G4double m_expHall_x;
    G4double m_expHall_y;
    G4double m_expHall_z;

    unsigned m_nRods;
    double m_rRod;
    double m_nFib;
    double m_nClad;
    double m_length;
    double m_length2;

    double m_h;
    double m_l;
    double m_y;
    double m_rFib;

    // materials
    G4Material * m_air;
    G4Material * m_quartz;
    G4Material * m_tungsten;

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
