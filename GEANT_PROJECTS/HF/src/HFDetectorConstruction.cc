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
// $Id: HFDetectorConstruction.cc,v 1.1 2013/03/13 10:34:16 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <iostream>

#include "HFDetectorConstruction.hh"
#include "HFStackingAction.hh"

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

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFDetectorConstruction::HFDetectorConstruction()
:m_isConstructed(false), m_nRods(20),m_rRod(2.5*mm),m_nFib(1.457),m_nClad(1.42)
,m_stacking(NULL)
{

  m_expHall_z = 20.0*cm;
  m_expHall_x = 5.*cm;
  m_expHall_y = 5.*cm;

  m_length = 15.*cm;

  m_messenger = new HFMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFDetectorConstruction::~HFDetectorConstruction()
{ delete m_messenger; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* HFDetectorConstruction::Construct()
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
void HFDetectorConstruction::DefineMaterials()
{ 

  // ------------- Materials -------------

  G4double a, z, density;
  G4int nelements;

  // Air
  // 
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
  G4Element* Si = new G4Element("Silicon", "Si", z=14, a=28.09*g/mole);

  m_air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  m_air->AddElement(N, 70.*perCent);
  m_air->AddElement(O, 30.*perCent);

  // Tungsten
  // 
  m_tungsten = new G4Material("Tungsten", z=74, a=183.19*g/mole, density=19.25*g/cm3);

  // Quartz
  //
  m_quartz = new G4Material("quartz",2.2*g/cm3,2);
  m_quartz->AddElement(Si,1);
  m_quartz->AddElement(O,2);

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


}

// Setup the world geometry
void HFDetectorConstruction::SetupWorld()
{

  // The experimental Hall
  //
  m_expHall_box = new G4Box("World",m_expHall_x,m_expHall_y,m_expHall_z);

  m_expHall_log
    = new G4LogicalVolume(m_expHall_box,m_air,"World",0,0,0);

  m_expHall_phys
    = new G4PVPlacement(0,G4ThreeVector(),m_expHall_log,"World",0,false,0);

}



// Setup the logical volumes
void HFDetectorConstruction::SetupGeometry()
{ 

  //
  // ------------- Volumes --------------

  // A tungsten rod
  //
  std::cout << "HF Constructing W rods: r=" << m_rRod << std::endl;
  m_tungRod = new G4Tubs("WRod",0.,m_rRod,m_length,0.,2.*pi);

  m_tungRod_log = new G4LogicalVolume(m_tungRod,m_tungsten,"WRodLog",0,0,0);

  // Quartz fibre
  //
  m_qFibre = new G4Tubs("quarzFibre",0.,m_rFib,m_length,0.,2.*pi);
  m_qFibre_log = new G4LogicalVolume(m_qFibre,m_quartz,"quartzFibreLog",0,0,0);

}

// Setup the detectors physical volumes
void HFDetectorConstruction::SetupDetectors()
{ 


  const unsigned n = m_nRods;
  //const unsigned nRods = 2*(n*(n+1)/2-n*(n/2+1)/4)-n;
  unsigned count=0U;

  // create main row
  const double xStart = m_rRod*(n-1);
  for ( unsigned i=0; i != n; i++ ) {
    const double xPos = 2*i*m_rRod-xStart;
    char name[6];
    sprintf(name,"rod%d",count);

    m_rods.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,0.,0.),m_tungRod_log,name,m_expHall_log,false,count++,true));
  }

  // create upper portion
  const unsigned nd2 = n/2;
  for ( unsigned row = 0; row != nd2; row++ ) {
    const double xStartr = xStart-(row+1)*m_rRod;
    const double yPos = m_h*(row+1);
    const unsigned nR = n-row-1U;
    for ( unsigned i=0; i != nR; i++ ) {
      const double xPos = 2*i*m_rRod-xStartr;
      char name[6];
      sprintf(name,"rod%d",count);
      
      m_rods.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,0.),m_tungRod_log,name,m_expHall_log,false,count++,true));
    }
  }

  // create lower portion
  for ( unsigned row = 0; row != nd2; row++ ) {
    const double xStartr = xStart-(row+1)*m_rRod;
    const double yPos = -m_h*(row+1U);
    const unsigned nR = n-row-1U;
    for ( unsigned i=0; i != nR; i++ ) {
      const double xPos = 2*i*m_rRod-xStartr;
      char name[6];
      sprintf(name,"rod%d",count);
      
      m_rods.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,0.),m_tungRod_log,name,m_expHall_log,false,count++,true));
    }
  }



  //
  // insert fibers


  //const unsigned nFibres = 2*(n*(n+1)/2-n*(n/2+1)/4);

  count = 0;
  // create upper lattice
  for ( unsigned row = 0; row != nd2; row++ ) {
    const double xStartr = xStart-(row+1)*m_rRod;
    const double yPosLow = m_h*(row+1U)-m_l;
    const double yPosHigh = m_h*(row+1U)-m_y;
    const unsigned nR = n-row-1U;
    const unsigned nF = 2*(nR-1)+1;
    for ( unsigned i=0; i != nF; i++ ) {
      const double xPos = i*m_rRod-xStartr;
      char name[6];
      sprintf(name,"fib%d",count);
     
      if ( i%2 == 1 ) m_fibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPosHigh,0.),m_qFibre_log,name,m_expHall_log,false,count++,true));
      else m_fibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPosLow,0.),m_qFibre_log,name,m_expHall_log,false,count++,true));
    }
  }

  // create lower lattice
  for ( unsigned row = 0; row != nd2; row++ ) {
    const double xStartr = xStart-(row+1)*m_rRod;
    const double yPosLow = -m_h*(row+1U)+m_y;
    const double yPosHigh = -m_h*(row+1U)+m_l;
    const unsigned nR = n-row-1U;
    const unsigned nF = 2*(nR-1)+1;
    for ( unsigned i=0; i != nF; i++ ) {
      const double xPos = i*m_rRod-xStartr;
      char name[6];
      sprintf(name,"fib%d",count);
    
      if ( i%2 == 0 ) m_fibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPosHigh,0.),m_qFibre_log,name,m_expHall_log,false,count++,true));
      else m_fibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPosLow,0.),m_qFibre_log,name,m_expHall_log,false,count++,true));
    }
  }



  // ------------- Surfaces and Optical Properties --------------


  const unsigned nEnergies = 3U;
  G4double energies[nEnergies] = { 1.625*eV, 5.*eV, 12.4*eV };
  G4double qRindex[nEnergies] = {m_nFib, m_nFib, m_nFib};   

  G4MaterialPropertiesTable *qProps = new G4MaterialPropertiesTable();
  qProps->AddProperty("RINDEX",energies,qRindex,nEnergies);
  m_quartz->SetMaterialPropertiesTable(qProps);

}


void HFDetectorConstruction::CalculateConstants()
{
  
  m_h = sqrt(3.)*m_rRod;
  m_y = sqrt(3.)*m_rRod/3.;
  m_l = 2.*sqrt(3.)*m_rRod/3;
  m_rFib = 0.99*(m_l-m_rRod);

  m_length2 = m_length/2.;

  if ( m_length2 > m_expHall_z ) m_expHall_z = 1.10*m_length2;
  const double width = m_nRods*m_rRod;
  if ( width > m_expHall_x ) { 
    m_expHall_x =  1.10*width;
    m_expHall_y = 1.1 * width;
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void HFDetectorConstruction::SetNRods(unsigned n)
{
  m_nRods = n;
  if ( !m_isConstructed ) return;

  // clear physical volumes
  ClearPhysicalVolumes();
  ClearLogicalVolumes();

  CalculateConstants();
  SetupGeometry();
  SetupDetectors();

  G4RunManager::GetRunManager()->GeometryHasBeenModified(); 

}


void HFDetectorConstruction::SetRRod(double r)
{ 
  m_rRod = r;

  if ( !m_isConstructed ) return;

  ClearPhysicalVolumes();
  ClearLogicalVolumes();

  CalculateConstants();
  SetupGeometry();
  SetupDetectors();

  G4RunManager::GetRunManager()->GeometryHasBeenModified(); 

}

void HFDetectorConstruction::SetLength(double l)
{

  m_length = l;

  if ( !m_isConstructed ) return;

  ClearPhysicalVolumes();
  ClearLogicalVolumes();

  CalculateConstants();
  SetupGeometry();
  SetupDetectors(); 

  G4RunManager::GetRunManager()->GeometryHasBeenModified(); 

}

void HFDetectorConstruction::SetFibreIndex(double n)
{
  m_nFib = n;

  if ( !m_isConstructed ) return;

  const unsigned nEnergies = 3U;
  G4double energies[nEnergies] = { 1.625*eV, 5.*eV, 12.4*eV };
  G4double qRindex[nEnergies] = {m_nFib, m_nFib, m_nFib};   

  G4MaterialPropertiesTable *qProps = new G4MaterialPropertiesTable();
  qProps->AddProperty("RINDEX",energies,qRindex,nEnergies);
  m_quartz->SetMaterialPropertiesTable(qProps);

  if ( m_stacking ) {
    m_stacking->SetFibreIndex(n);
  }
  
  G4RunManager::GetRunManager()->GeometryHasBeenModified(); 

}

void HFDetectorConstruction::SetCladIndex(double n)
{
  m_nClad = n;
  if ( m_stacking ) {
    m_stacking->SetCladIndex(n);
  }
}


void HFDetectorConstruction::ClearPhysicalVolumes()
{

  assert(m_isConstructed);

  const unsigned nRods = m_rods.size();
  for ( unsigned i=0; i != nRods; i++ ) {
    m_tungRod_log->RemoveDaughter(m_rods[i]);
    delete m_rods[i];
  }
  m_rods.clear();

  const unsigned nFibs = m_fibres.size();
  for ( unsigned i=0; i != nFibs; i++ ) {
    m_qFibre_log->RemoveDaughter(m_fibres[i]);
    delete m_fibres[i];
  }
  m_fibres.clear();


}

void HFDetectorConstruction::ClearLogicalVolumes()
{

  assert(m_isConstructed);

  //m_expHall_log->RemoveDaughter(m_expHall_phys);

  delete m_tungRod;
  delete m_qFibre_log;

  //m_expHall_log->RemoveDaughter(m_expHall_phys);
  //delete m_expHall_phys;
  
  //delete m_expHall_log;
  //delete m_expHall_box;

}


void HFDetectorConstruction::SetStackingAction( HFStackingAction *sa )
{
  m_stacking = sa;
}


