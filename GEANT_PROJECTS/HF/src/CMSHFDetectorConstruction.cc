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
// $Id: CMSHFDetectorConstruction.cc,v 1.5 2013/06/18 16:08:49 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <iostream>

#include "CMSHFDetectorConstruction.hh"
#include "HFStackingAction.hh"
#include "HFPrimaryGeneratorAction.hh"

#include "G4RunManager.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"


#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CMSHFDetectorConstruction::CMSHFDetectorConstruction()
:m_isConstructed(false),m_nFib(1.457),m_nClad(1.42)
,m_absFib(28.1*m),m_absClad(28.1*m)
,m_nSFib(1.59),m_nSClad(1.49)
,m_absSFib(4.*m),m_absSClad(4.*m)
,m_nGlass(1.517),m_absGlass(.1)
,m_Bfield(NULL)
,m_stacking(NULL)
//,m_gun(NULL)
{

  m_expHall_z = 20.0*m;
  m_expHall_x = 5.0*m;
  m_expHall_y = 5.0*m;


  m_length = 60.*cm;  // half length of the calorimeter
  //m_Wdx = 19.8*mm;
  //m_Wdy = m_Wdx;

  // set the position of the detector
  //m_zPos = 7.3*m;
  m_zPos = 0.7*m;
  m_xPos = 0.*m;
  m_yPos = 0.*m;

  // This is the full radius including the cladding
  // since the cladding is a daughter volume of the fiber 
  m_rCFib = 0.366*mm;
  m_rSFib = m_rCFib;

  m_rCClad = 0.333*mm;
  m_rSClad = m_rCClad;

  m_NfibSeg = 32U;
  m_Nseg = 13U;

  m_a = 1.2*mm;

  m_checkOverlaps = false;
  m_messenger = new CMSHFDetectorConstructionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CMSHFDetectorConstruction::~CMSHFDetectorConstruction()
{ 
  delete m_messenger; 
  //delete m_fibLimits;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* CMSHFDetectorConstruction::Construct()
{
  
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  if ( !m_isConstructed ) {
    m_isConstructed = true;
    CalculateConstants();
    DefineMaterials();
    SetupWorld();
    SetupGeometry();
    SetupDetectors();
  }

  // print the calo parameters
  PrintCalorParameters();


//always return the physical World
  return m_expHall_phys;
}


//  Define the materials used in the detector
void CMSHFDetectorConstruction::DefineMaterials()
{ 

  // ------------- Materials -------------

  G4double a, z, density;
  G4int nelements;

  // Air
  // 
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
  G4Element* Si = new G4Element("Silicon", "Si", z=14, a=28.09*g/mole);

  G4NistManager* man = G4NistManager::Instance();
  G4Element* B = man->FindOrBuildElement("B");
  G4Element* Na = man->FindOrBuildElement("Na");
  G4Element* Al = man->FindOrBuildElement("Al");
  G4Element* K = man->FindOrBuildElement("K");
  G4Element* W = man->FindOrBuildElement("W");
  G4Element* Ni = man->FindOrBuildElement("Ni");
  G4Element* Cu = man->FindOrBuildElement("Cu");
  G4Element* C = man->FindOrBuildElement("C");
  G4Element* H = man->FindOrBuildElement("H");

  m_air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  m_air->AddElement(N, 70.*perCent);
  m_air->AddElement(O, 30.*perCent);

  // Tungsten
  // 
  //m_tungsten = new G4Material("Tungsten", z=74, a=183.19*g/mole, density=19.25*g/cm3);
  m_tungsten = new G4Material("Tungsten",17.*g/cm3,3);
  m_tungsten->AddElement(W,90.*perCent);
  m_tungsten->AddElement(Ni,5.*perCent);
  m_tungsten->AddElement(Cu,5.*perCent);

  // Iron
  m_iron = new G4Material("Iron", z=26, a=55.845*g/mole, density=7.874*g/cm3);

  // Quartz
  //
  m_quartz = new G4Material("quartz",2.2*g/cm3,2);
  m_quartz->AddElement(Si,1);
  m_quartz->AddElement(O,2);

  // cladding material
  m_cladCher = new G4Material("claddingCher",2.2*g/cm3,2);
  m_cladCher->AddElement(Si,1);
  m_cladCher->AddElement(O,2);

  // scsf
  m_scsf78 = new G4Material("scsf78",2.2*g/cm3,2);
  m_scsf78->AddElement(Si,1);
  m_scsf78->AddElement(O,2);
  //m_scsf78->AddElement(C,4);
  //m_scsf78->AddElement(H,4);

  // cladding material
  m_cladScin = new G4Material("claddingScin",2.2*g/cm3,2);
  m_cladScin->AddElement(Si,1);
  m_cladScin->AddElement(O,2);
  //m_cladScin->AddElement(C,3);
  //m_cladScin->AddElement(H,5);
  //m_cladScin->AddElement(O,2);

  //
  // ------------ Generate & Add Material Properties Table ------------
  //
  const G4int nEntries = 32;

  G4double PhotonEnergy[nEntries] =
            { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
              2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
              2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
              2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
              2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
              3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
              3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
              3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };


  //
  // Air
  //
  G4double RefractiveIndex2[nEntries] =
            { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
              1.00, 1.00, 1.00, 1.00 };

  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex2, nEntries);
  
  m_air->SetMaterialPropertiesTable(myMPT2);

  // Glass
  //
  m_glass = new G4Material("glass",2.23*g/cm3,6);
  m_glass->AddElement(B,0.040064);
  m_glass->AddElement(O,0.539562);
  m_glass->AddElement(Na,0.028191);
  m_glass->AddElement(Al,0.011644);
  m_glass->AddElement(Si,0.377220);
  m_glass->AddElement(K,0.003321);

  G4MaterialPropertiesTable *glassPT = new G4MaterialPropertiesTable();
  G4double glassE[3] = { 1.625*eV, 5.*eV, 12.4*eV };
  G4double glassN[3] = {m_nGlass,m_nGlass,m_nGlass};
  G4double glassAbs[3] = {m_absGlass,m_absGlass,m_absGlass};
  glassPT->AddProperty("RINDEX",glassE,glassN,3);
  glassPT->AddProperty("ABSLENGTH",glassE,glassAbs,3);


  // ------------- Surfaces and Optical Properties --------------


  const unsigned nEnergies = 3;
  G4double energies[nEnergies] = { 1.625*eV, 5.*eV, 12.4*eV };
  G4double qRindex[nEnergies] = {m_nFib, m_nFib, m_nFib};   
  G4double qAbsLength[nEnergies] = {m_absFib, m_absFib, m_absFib};

  G4MaterialPropertiesTable *qProps = new G4MaterialPropertiesTable();
  qProps->AddProperty("RINDEX",energies,qRindex,nEnergies);
  qProps->AddProperty("ABSLENGTH",energies,qAbsLength,nEnergies);
  m_quartz->SetMaterialPropertiesTable(qProps);

  G4double cRindex[nEnergies] = {m_nClad,m_nClad,m_nClad};
  G4double cAbsLength[nEnergies] = {m_absClad,m_absClad,m_absClad};
  
  G4MaterialPropertiesTable *cProps = new G4MaterialPropertiesTable();
  cProps->AddProperty("RINDEX",energies,cRindex,nEnergies);
  cProps->AddProperty("ABSLENGTH",energies,cAbsLength,nEnergies);
  m_cladCher->SetMaterialPropertiesTable(cProps);


  const unsigned nScinEnergies = 6;
  G4double scinEnergies[nScinEnergies] = {3.1*eV, 2.9*eV, 2.76*eV, 2.48*eV, 2.25*eV, 2.07*eV };
  G4double scinValues[nScinEnergies] = {0.0, 5.0, 10., 5.0, 2.0, 1.0};
  G4double scinRindex[nScinEnergies] = { m_nSFib, m_nSFib, m_nSFib, m_nSFib, m_nSFib, m_nSFib };
  G4double scinAbsLength[nScinEnergies] = { m_absSFib, m_absSFib, m_absSFib, m_absSFib, m_absSFib, m_absSFib };
  G4double scinCladRindex[nScinEnergies] = { m_nSClad, m_nSClad, m_nSClad, m_nSClad, m_nSClad, m_nSClad };
  G4double scinCladAbsLength[nScinEnergies] = { m_absSClad, m_absSClad, m_absSClad, m_absSClad, m_absSClad, m_absSClad };

  G4MaterialPropertiesTable *scsfProps = new G4MaterialPropertiesTable();
  scsfProps->AddProperty("RINDEX",scinEnergies,scinRindex,nScinEnergies);
  scsfProps->AddProperty("ABSLENGTH",scinEnergies,scinAbsLength,nScinEnergies);

  //scsfProps->AddProperty("FASTCOMPONENT",scinEnergies,scinValues,nScinEnergies);
  //scsfProps->AddProperty("SLOWCOMPONENT",scinEnergies,scinValues,nScinEnergies);

  //
  // see http://infoscience.epfl.ch/record/164027/files/EPFL_TH5033.pdf
  // for scintillation yield
  /*scsfProps->AddConstProperty("SCINTILLATIONYIELD", 8300./MeV);
  scsfProps->AddConstProperty("RESOLUTIONSCALE", 2.0);
  scsfProps->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  scsfProps->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
  scsfProps->AddConstProperty("YIELDRATIO", 0.8);*/

  G4MaterialPropertiesTable *scsfCladProps = new G4MaterialPropertiesTable();
  scsfCladProps->AddProperty("RINDEX",scinEnergies,scinCladRindex,nScinEnergies);
  scsfCladProps->AddProperty("ABSLENGTH",scinEnergies,scinCladAbsLength,nScinEnergies);

  //m_scsf78->SetMaterialPropertiesTable(scsfProps);
  m_scsf78->SetMaterialPropertiesTable(scsfProps);
  m_cladScin->SetMaterialPropertiesTable(scsfCladProps);



}

// Setup the world geometry
void CMSHFDetectorConstruction::SetupWorld()
{

  // The experimental Hall
  //
  m_expHall_box = new G4Box("World",m_expHall_x,m_expHall_y,m_expHall_z);

  m_expHall_log
    = new G4LogicalVolume(m_expHall_box,m_air,"World",0,0,0);

  m_expHall_phys
    = new G4PVPlacement(0,G4ThreeVector(0, 0, 0),m_expHall_log,"World",0,false,0);

}



// Setup the logical volumes
void CMSHFDetectorConstruction::SetupGeometry()
{ 

  //
  // ------------- primitives -----------
  G4cout << "CMSHF Constructing W block: " << 2.*m_Wdx << "x" << 2.*m_Wdy << "x" << 2.*m_length << G4endl;
  //m_tungBlock = new G4Box("Wblock",m_Wdx,m_Wdy,m_length);
  m_tungBlock = new G4Box("Wblock",m_segWidth/2.,m_segWidth/2.,m_length);
  m_qFibreCher = new G4Tubs("quarzFibre",0.,m_rCFib,m_length,0.,2.*pi);
  m_cladCher_tube = new G4Tubs("cladding",m_rCClad,m_rCFib,m_length,0,2.*pi);
  m_qFibreScin = new G4Tubs("scsfFibre",0.,m_rSFib,m_length,0.,2.*pi);
  m_cladScin_tube = new G4Tubs("cladding",m_rSClad,m_rSFib,m_length,0,2.*pi);
  m_glass_box = new G4Box("Glass",m_expHall_x,m_expHall_y,1.*cm);

  //
  // ------------- Volumes --------------
  const unsigned segTot = m_Nseg*m_Nseg;
  m_tungBlock_log.resize(segTot);
  m_qFibreCher_log.resize(segTot);
  m_cladCher_log.resize(segTot);
  m_qFibreScin_log.resize(segTot);
  m_cladScin_log.resize(segTot);


  for ( unsigned i=0; i != segTot; i++ ) {
    // A tungsten rod
    m_tungBlock_log[i] = new G4LogicalVolume(m_tungBlock,m_tungsten,"Wblock",0,0,0);
  
    // Quartz fibre
    m_qFibreCher_log[i] = new G4LogicalVolume(m_qFibreCher,m_quartz,"quartzFibreLog",0,0,0);
  
    // Cladding on fibre
    m_cladCher_log[i] = new G4LogicalVolume(m_cladCher_tube,m_cladCher,"claddingCher",0,0,0);
  
    // Scintillating scsf-78
    m_qFibreScin_log[i] = new G4LogicalVolume(m_qFibreScin,m_scsf78,"scsf78FibreLog",0,0,0);
  
    // Cladding on fibre
    m_cladScin_log[i] = new G4LogicalVolume(m_cladScin_tube,m_cladScin,"claddingScin",0,0,0);
  }

  // Glass 
  //
  m_glass_log = new G4LogicalVolume(m_glass_box,m_glass,"glassLog",0,0,0); 

  // set limits on the time to propagate photons
  // needs a G4StepLimiter needs to be added to OpticalPhoton
  // process for the following to do anything.
  /*m_fibLimits  = new G4UserLimits();
  m_fibLimits->SetUserMaxTime(0.0*ns);
  m_qFibre_log->SetUserLimits(m_fibLimits);
  m_clad_log->SetUserLimits(m_fibLimits);
  m_glass_log->SetUserLimits(m_fibLimits);
  m_expHall_log->SetUserLimits(m_fibLimits);*/

}

// Setup the detectors physical volumes
void CMSHFDetectorConstruction::SetupDetectors()
{ 

  int count=0,scsfCount=0;

  const unsigned segTot = m_Nseg*m_Nseg;
  m_tungBlock_phys.resize(segTot);

  // place the tungsten block
  for ( unsigned i=0; i != segTot; i++ ) {
    m_tungBlock_phys[i] = new G4PVPlacement(0,m_segPositions[i],m_tungBlock_log[i],"tungsten",m_expHall_log,false,0,m_checkOverlaps);

  }

  // ------------------------------------------------------
  // insert fibers 
  char name[50];
  unsigned segRow=0,segCol=0;
  unsigned fibCountRow=0; // count of fibres (total) in a row across all segments
  unsigned rowCount=0; // count of the number of rows across all segments
  const unsigned nFibSide = m_Nseg*m_NfibSeg;

  for ( unsigned i=0; i != nFibSide; i++ ) {
  
    // reset segCol to zero
    segCol=0;

    const unsigned iSubIndex = i%m_NfibSeg;
    double yPos = m_a/2. - m_segWidth/2. + iSubIndex*m_a;

    const bool oddRow = i%2==0;
    for ( unsigned j=0; j != nFibSide; j++ ) {
      const bool doQuartz = ( !oddRow && (j+1)%2==0 ) || ( oddRow && j%2==0 );

      const unsigned segNum = segRow*m_Nseg+segCol;
      const unsigned jSubIndex = j%m_NfibSeg;

      double xPos = m_a/2. - m_segWidth/2. + jSubIndex*m_a;

      if ( doQuartz ) {
      // place quartz
      	sprintf(name,"fib%d",count);
      	m_fibresCher.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,0.),m_qFibreCher_log[segNum],name,m_tungBlock_log[segNum],false,count,m_checkOverlaps));
      	sprintf(name,"clad%d",count);
      	m_claddingCher.push_back(new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),m_cladCher_log[segNum],name,m_qFibreCher_log[segNum],false,count,m_checkOverlaps));
      	count++;
      } else {
      // place scsf78
	sprintf(name,"scsf%d",scsfCount);
	m_fibresScin.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,0.),m_qFibreScin_log[segNum],name,m_tungBlock_log[segNum],false,scsfCount,m_checkOverlaps));
	sprintf(name,"cladScin%d",scsfCount);
	m_claddingScin.push_back(new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),m_cladScin_log[segNum],name,m_qFibreScin_log[segNum],false,scsfCount,m_checkOverlaps));
 	scsfCount++;
      }

      if ( jSubIndex == m_NfibSeg-1U ) segCol++;
    }
    assert ( segCol == m_Nseg );
    if ( iSubIndex == m_NfibSeg-1U ) segRow++;
  }

  //
  G4cout << "Total Number of fibers: " << count << " " << scsfCount << G4endl;



  // -------------------- glass face ----------------------------

  // put glass plate at back
  m_glass_phys = new G4PVPlacement(0,G4ThreeVector(0.,0.,m_zPos+m_length+1*cm),m_glass_log,"glass",m_expHall_log,false,0,true); 

}


void CMSHFDetectorConstruction::CalculateConstants()
{
  
  if ( m_stacking ) {
     m_stacking->SetFiberRadius(m_rCClad);
  }

  m_segWidth = m_a*m_NfibSeg; 
  m_Wdx = m_Nseg*m_segWidth/2.;
  m_Wdy = m_Wdx;

  m_nQseg = (unsigned)(m_NfibSeg/2.+0.5);
  m_nSseg = m_NfibSeg - m_nQseg;

  // calculate segment positions
  const double xOffset = m_Wdx-m_segWidth/2.;
  const double yOffset = m_Wdy-m_segWidth/2.;

  m_segPositions.resize(m_Nseg*m_Nseg);

  for ( unsigned i=0; i != m_Nseg; i++ ) {
    const double yPos = m_yPos-yOffset+i*m_segWidth;
    for ( unsigned j=0; j != m_Nseg; j++ ) {
      const double xPos = m_xPos-xOffset+j*m_segWidth;
      unsigned p = i*m_Nseg+j;
      m_segPositions[p] = G4ThreeVector(xPos,yPos,m_zPos);
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void CMSHFDetectorConstruction::SetPositionXYZ(const G4ThreeVector &detPos)
{
  m_xPos = detPos.x();
  m_yPos = detPos.y();
  m_zPos = detPos.z();

  if ( !m_isConstructed ) return;

  ClearPhysicalVolumes();
  //ClearLogicalVolumes();

  CalculateConstants();
  //SetupGeometry();
  SetupDetectors();

  G4RunManager::GetRunManager()->GeometryHasBeenModified();

}


void CMSHFDetectorConstruction::SetLength(G4double l)
{

  m_length = l;

  if ( !m_isConstructed ) return;

  ClearPhysicalVolumes();
  ClearLogicalVolumes();

  CalculateConstants();
  SetupGeometry();
  SetupDetectors(); 

  //if ( m_gun ) {
  //  m_gun->SetInitDist(m_length);
  //}

  G4RunManager::GetRunManager()->GeometryHasBeenModified(); 

}

void CMSHFDetectorConstruction::SetNFibSeg(unsigned N)
{

  if ( m_isConstructed ) {

    ClearPhysicalVolumes();
    ClearLogicalVolumes();
    
    m_NfibSeg = N;

    CalculateConstants();
    SetupGeometry();
    SetupDetectors();

    G4RunManager::GetRunManager()->GeometryHasBeenModified();

  }

  m_NfibSeg = N;

}

void CMSHFDetectorConstruction::SetNSeg(unsigned N)
{

  if ( m_isConstructed ) {
    ClearPhysicalVolumes();
    ClearLogicalVolumes();
  
    m_Nseg = N;

    CalculateConstants();
    SetupGeometry();
    SetupDetectors();
  
    G4RunManager::GetRunManager()->GeometryHasBeenModified();

  }

  m_Nseg = N;
  
}


void CMSHFDetectorConstruction::SetFibreIndex(G4double n)
{
  m_nFib = n;

  if ( !m_isConstructed ) return;

  const unsigned nEnergies = 3;
  G4double energies[nEnergies] = { 1.625*eV, 5.*eV, 12.4*eV };
  G4double qRindex[nEnergies] = {m_nFib, m_nFib, m_nFib};   
  G4double qAbsLength[nEnergies] = {m_absFib, m_absFib, m_absFib};

  G4MaterialPropertiesTable *qProps = new G4MaterialPropertiesTable();
  qProps->AddProperty("RINDEX",energies,qRindex,nEnergies);
  qProps->AddProperty("ABSLENGTH",energies,qAbsLength,nEnergies);
  m_quartz->SetMaterialPropertiesTable(qProps);

  if ( m_stacking ) {
    m_stacking->SetFibreIndex(n);
  }
  
  G4RunManager::GetRunManager()->GeometryHasBeenModified(); 

}

void CMSHFDetectorConstruction::SetCladIndex(G4double n)
{
  m_nClad = n;

  if ( !m_isConstructed ) return;

  const unsigned nEnergies = 3U;
  G4double energies[nEnergies] = { 1.625*eV, 5.*eV, 12.4*eV };
  G4double cRindex[nEnergies] = {m_nClad,m_nClad,m_nClad};
  G4double cAbsLength[nEnergies] = {m_absClad,m_absClad,m_absClad};
  
  G4MaterialPropertiesTable *cProps = new G4MaterialPropertiesTable();
  cProps->AddProperty("RINDEX",energies,cRindex,nEnergies);
  cProps->AddProperty("ABSLENGTH",energies,cAbsLength,nEnergies);
  m_cladCher->SetMaterialPropertiesTable(cProps);

  if ( m_stacking ) {
    m_stacking->SetCladIndex(n);
  }

  G4RunManager::GetRunManager()->GeometryHasBeenModified(); 
}


void CMSHFDetectorConstruction::SetOverlapCheck(G4bool check)
{
  m_checkOverlaps = check;

  if ( !m_isConstructed ) return;

  G4RunManager::GetRunManager()->GeometryHasBeenModified(); 
  
}


void CMSHFDetectorConstruction::ClearPhysicalVolumes()
{

  assert(m_isConstructed);

  const unsigned segTot = m_Nseg*m_Nseg;
  for ( unsigned i=0; i != segTot; i++ )  {
    m_tungBlock_log[i]->RemoveDaughter(m_tungBlock_phys[i]);
    delete m_tungBlock_phys[i];
  }

  m_tungBlock_phys.clear();

  const unsigned nFibs = m_fibresCher.size();
  for ( unsigned i=0; i != nFibs; i++ ) {
    m_fibresCher[i]->GetLogicalVolume()->RemoveDaughter(m_fibresCher[i]);
    m_claddingCher[i]->GetLogicalVolume()->RemoveDaughter(m_claddingCher[i]);
    delete m_fibresCher[i];
    delete m_claddingCher[i];
  }
  m_fibresCher.clear();

  const unsigned nScin = m_fibresScin.size();
  for ( unsigned i=0; i != nScin; i++ ) {
    m_fibresScin[i]->GetLogicalVolume()->RemoveDaughter(m_fibresScin[i]);
    m_claddingScin[i]->GetLogicalVolume()->RemoveDaughter(m_claddingScin[i]);
    delete m_fibresScin[i];
    delete m_claddingScin[i];
  }
  m_claddingCher.clear();

  m_glass_log->RemoveDaughter(m_glass_phys);
  delete m_glass_phys;

  // clear daughters of logical volumes
  for ( unsigned i=0; i != segTot; i++ ) {
    m_cladScin_log[i]->ClearDaughters();  
    m_cladCher_log[i]->ClearDaughters();
    m_qFibreScin_log[i]->ClearDaughters();
    m_qFibreCher_log[i]->ClearDaughters();
    m_tungBlock_log[i]->ClearDaughters();
  }


}

void CMSHFDetectorConstruction::ClearLogicalVolumes()
{

  assert(m_isConstructed);

  //m_expHall_log->RemoveDaughter(m_expHall_phys);

  const unsigned nSeg = m_tungBlock_log.size();

  for ( unsigned i=0; i != nSeg; i++ )  {
    delete m_tungBlock_log[i];
    delete m_qFibreCher_log[i];
    delete m_cladCher_log[i];
    delete m_qFibreScin_log[i];
    delete m_cladScin_log[i];
  }
  m_tungBlock_log.clear();
  m_qFibreCher_log.clear();
  m_cladCher_log.clear();
  m_qFibreScin_log.clear();
  m_cladScin_log.clear();

  delete m_glass_log;

  //m_expHall_log->RemoveDaughter(m_expHall_phys);
  //delete m_expHall_phys;
  
  //delete m_expHall_log;
  //delete m_expHall_box;

}

void CMSHFDetectorConstruction::SetMagneticField(const G4ThreeVector &vec)
{

  if ( m_Bfield ) delete m_Bfield;

  m_Bfield = new G4UniformMagField(vec);

  G4FieldManager *fldMngr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  fldMngr->SetDetectorField(m_Bfield);
  fldMngr->CreateChordFinder(m_Bfield);

  G4cout << "Creating Magnetic Field: " << vec << G4endl;

}


void CMSHFDetectorConstruction::SetStackingAction( HFStackingAction *sa )
{
  m_stacking = sa;
}


//void CMSHFDetectorConstruction::SetParticleGun( HFPrimaryGeneratorAction *gun )
//{
//  m_gun = gun;
//}


G4double CMSHFDetectorConstruction::GetPackingFraction() const
{
  return 0.;
}


