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
// $Id: CMSHFDetectorConstruction.hh,v 1.6 2013/05/17 19:19:10 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef CMSHFDetectorConstruction_h
#define CMSHFDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "CMSHFDetectorConstructionMessenger.hh"

#include "G4RotationMatrix.hh"

class G4LogicalVolume;
class G4PhysicalVolume;
class G4Material;
class G4Box;
class G4Tubs;
class G4UserLimits;

class G4UniformMagField;

class HFStackingAction;
class HFPrimaryGeneratorAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CMSHFDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    CMSHFDetectorConstruction();
   ~CMSHFDetectorConstruction();

  public:
    G4VPhysicalVolume* Construct();

    // set the position of the centre of the calorimeter segment
    void SetPositionXYZ(const G4ThreeVector &detPos);

    // set the length of the calorimeter (longitudinal)
    void SetLength(G4double l);

    // set N fibres per segment side.  sqrt(Ntot) in a segment
    void SetNFibSeg(unsigned N);

    // set N segments per side of detector.  sqrt(Ntot) of detector
    void SetNSeg(unsigned N);

    // set fibre spacing
    void SetFibSpacing(G4double a);

    // set the fibre index of refraction
    void SetFibreIndex(G4double n);

    // set the cladding index of refraction
    void SetCladIndex(G4double n);

    // allow access to the user stacking action class CMSHFStackingAction
    void SetStackingAction( HFStackingAction * sa);

    // allow access to the particle gun
    void SetParticleGun( HFPrimaryGeneratorAction *gun);

    // set whether to check for overlaps in the geometry
    void SetOverlapCheck( G4bool check );

    // set the magnetic field
    void SetMagneticField( const G4ThreeVector &vec);

    // set the pitch of the detector
    // pitch is applied first, then yaw
    void SetPitchAndYaw(G4double, G4double);

    // turn on fiber filling
    void SetFibres(const bool);

    // functions to set dead segments around block
    void SetDeadTop(const unsigned );
    void SetDeadBottom(const unsigned );
    void SetDeadRight(const unsigned );
    void SetDeadLeft(const unsigned );

    // functions to set define the sinctillator properties
    void SetScinYield(const G4double);
    void SetScinFastConst(const G4double);
    void SetScinSlowConst(const G4double);
    void SetScinYieldRatio(const G4double);

    // switch for detector material
    // passing true -> build W absorber
    // passing false -> build Brass absorber
    void BuildWorBrass(const bool);

    // refresh geometry and force a rebuild
    void RefreshGeometry();


    // ---- accessor methods -----
    void PrintCalorParameters() { return; }
    
    // 
    G4ThreeVector GetPositionXYZ() const { return G4ThreeVector(m_xPos,m_yPos,m_zPos); }
    G4double GetLength() const { return m_length; }
    G4double GetFibreIndex() const { return m_nFib; }
    G4double GetCladIndex() const { return m_nClad; }
    G4double GetOverlapCheck() const { return m_checkOverlaps; }
    
    // compute the packing fraction
    G4double GetPackingFraction() const;

  private:
   // -------------- private member functions --------
   void DefineMaterials();
   void DefineScintillator();
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

    // position of calorimeter
    G4double m_zPos;
    G4double m_xPos;
    G4double m_yPos;

    // Tungsten block parameters
    G4double m_Wdx;
    G4double m_Wdy;

    unsigned m_NfibSeg;  // number of fibres on segment side
    unsigned m_Nseg;     // number of segments on detector side
    G4double m_segWidth;
    G4double m_segHeight;

    // number of dead segments to put on sides of active area
    unsigned m_deadSeg_bottom;
    unsigned m_deadSeg_top;
    unsigned m_deadSeg_right;
    unsigned m_deadSeg_left;

    unsigned m_nQseg;  // number of Q fibres in a segment
    unsigned m_nSseg;  // number of S fibres in a segment
    bool m_fillFibres;

    std::vector<G4ThreeVector> m_segPositions;
    std::vector<G4ThreeVector> m_deadPositions;

    G4double m_nFib;
    G4double m_nClad;
    G4double m_absFib;
    G4double m_absClad;
    G4double m_nSFib;
    G4double m_nSClad;
    G4double m_absSFib;
    G4double m_absSClad;
    G4double m_length;
    G4double m_nGlass;
    G4double m_absGlass;

    // scintillator properties
    G4double m_scinFastConst;
    G4double m_scinSlowConst;
    G4double m_scinYield;
    G4double m_scinYieldRatio;

    bool m_buildW; // switch for tungsten (true) or brass (false)

    // radii for fibers and cladding ( C for Cherenkov and S for scintillation)
    G4double m_rCFib;
    G4double m_rCClad;
    G4double m_rSFib;
    G4double m_rSClad;
    G4double m_rair; // radius of air gap

    // fiber spacing  (not a grid)
    G4double m_a;
    G4double m_h; // sqrt(3)/2 * m_a

    // rotation matrix for the wedge
    // this describes how a wedge is shifted from a normal position
    double m_pitch; 
    double m_yaw;

    G4bool m_checkOverlaps;

    // materials
    G4Material * m_air;
    G4Material * m_quartz;
    G4Material * m_tungsten;
    G4Material * m_brass;
    G4Material * m_glass;
    G4Material * m_cladCher;
    G4Material * m_scsf78;
    G4Material * m_cladScin;
    G4Material * m_iron;

    // primitives
    G4Box * m_expHall_box;
    G4Box * m_tungBlock;
    G4Tubs * m_qFibreCher;
    G4Tubs * m_qFibreScin;
    G4Box * m_glass_box;
    G4Tubs * m_cladCher_tube;
    G4Tubs * m_cladScin_tube;
    G4Tubs * m_air_gap;

    // logical volumes
    G4LogicalVolume * m_expHall_log;
    std::vector<G4LogicalVolume *> m_tungBlock_log;
    std::vector<G4LogicalVolume *> m_qFibreCher_log;
    std::vector<G4LogicalVolume *> m_cladCher_log;
    std::vector<G4LogicalVolume *> m_qFibreScin_log;
    std::vector<G4LogicalVolume *> m_cladScin_log;
    std::vector<G4LogicalVolume *> m_airGap_log;
    G4LogicalVolume * m_glass_log;
    G4LogicalVolume * m_deadBlock_log;

    // physical volumes
    G4VPhysicalVolume * m_expHall_phys;
    std::vector<G4VPhysicalVolume *> m_tungBlock_phys;
    std::vector<G4VPhysicalVolume *> m_fibresCher;
    std::vector<G4VPhysicalVolume *> m_claddingCher;
    std::vector<G4VPhysicalVolume *> m_fibresScin;
    std::vector<G4VPhysicalVolume *> m_claddingScin;
    std::vector<G4VPhysicalVolume *> m_airGap_phys;
    std::vector<G4VPhysicalVolume *> m_glass_phys;
    std::vector<G4VPhysicalVolume *> m_deadBlocks_phys;

    // uniform magnetic field
    G4UniformMagField *m_Bfield;

    CMSHFDetectorConstructionMessenger *m_messenger;
    HFStackingAction * m_stacking;
    HFPrimaryGeneratorAction *m_gun;

    G4UserLimits *m_fibLimits;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*CMSHFDetectorConstruction_h*/
