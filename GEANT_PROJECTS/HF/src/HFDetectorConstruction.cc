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
// $Id: HFDetectorConstruction.cc,v 1.9 2013/05/18 19:12:17 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <iostream>

#include "HFDetectorConstruction.hh"
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

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFDetectorConstruction::HFDetectorConstruction()
:m_isConstructed(false), m_nRods(4),m_rRod(2.5*mm),m_nFib(1.457),m_nClad(1.42)
,m_absFib(28.1*m),m_absClad(28.1*m)
,m_nSFib(1.59),m_nSClad(1.49)
,m_absSFib(4.*m),m_absSClad(4.*m)
,m_nGlass(1.517),m_absGlass(.1)
,m_stacking(NULL)
//,m_gun(NULL)
{

  m_expHall_z = 20.0*m;
  m_expHall_x = 5.0*m;
  m_expHall_y = 5.0*m;

  m_length = 50.*cm;

  m_checkOverlaps = false;
  m_messenger = new HFDetectorConstructionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HFDetectorConstruction::~HFDetectorConstruction()
{ 
  delete m_messenger; 
  //delete m_fibLimits;
}

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

  G4NistManager* man = G4NistManager::Instance();
  G4Element* B = man->FindOrBuildElement("B");
  G4Element* Na = man->FindOrBuildElement("Na");
  G4Element* Al = man->FindOrBuildElement("Al");
  G4Element* K = man->FindOrBuildElement("K");
  G4Element* C = man->FindOrBuildElement("C");
  G4Element* H = man->FindOrBuildElement("H");

  m_air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  m_air->AddElement(N, 70.*perCent);
  m_air->AddElement(O, 30.*perCent);

  // Tungsten
  // 
  m_tungsten = new G4Material("Tungsten", z=74, a=183.19*g/mole, density=19.25*g/cm3);

  // Iron
  m_iron = new G4Material("Iron", z=26, a=55.845*g/mole, density=7.874*g/cm3);

  // Quartz
  //
  m_quartz = new G4Material("quartz",2.2*g/cm3,2);
  m_quartz->AddElement(Si,1);
  m_quartz->AddElement(O,2);

  // Quartz cladding material
  m_cladCher = new G4Material("Ccladding",2.2*g/cm3,2);
  m_cladCher->AddElement(Si,1);
  m_cladCher->AddElement(O,2);

  // scsf
  m_scsf78 = new G4Material("scsf78",2.2*g/cm3,2);
  m_scsf78->AddElement(C,1);
  m_scsf78->AddElement(H,1);

  // Quartz cladding material
  m_cladScin = new G4Material("PMMA",2.2*g/cm3,3);
  m_cladScin->AddElement(C,5);
  m_cladScin->AddElement(O,2);
  m_cladScin->AddElement(H,8);

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
    = new G4PVPlacement(0,G4ThreeVector(0, 0, 0),m_expHall_log,"World",0,false,0);

}



// Setup the logical volumes
void HFDetectorConstruction::SetupGeometry()
{ 


//calculate number of rods

  const unsigned n = m_nRods;
  int num_Rods = n;
  unsigned int  i = 1;
  double adder = 0;
while(i <= n/2){
adder = 2*(n-i);  
num_Rods = num_Rods + adder;
i = i +1;
}




  //
  // ------------- Volumes --------------

  // A tungsten rod
  //
  std::cout << "HF Constructing W rods: r=" << m_rRod << std::endl;
  m_tungRod = new G4Tubs("WRod",0.,m_rRod,m_length,0.,2.*pi);


i = 0;
m_tungRod_log.resize(num_Rods);

while ( i < num_Rods){
  m_tungRod_log[i] = new G4LogicalVolume(m_tungRod,m_tungsten,"WRodLog",0,0,0);
  i = i + 1;
}
  // Quartz fibre
  //
  m_qFibreCher = new G4Tubs("quarzFibre",0.,m_rCFib,m_length,0.,2.*pi);

i = 0;
m_qFibreCher_log.resize(num_Rods);

while( i < num_Rods){

  m_qFibreCher_log[i] = new G4LogicalVolume(m_qFibreCher,m_quartz,"quartzFibreLog",0,0,0);
  i = i +1;
}


  // Cladding on quartz fibre
  //
  m_cladCher_tube = new G4Tubs("Ccladding",m_rCClad,m_rCFib,m_length,0,2.*pi);

i = 0;
m_cladCher_log.resize(3*num_Rods);

while (i < 3*num_Rods){
  m_cladCher_log[i] = new G4LogicalVolume(m_cladCher_tube,m_cladCher,"Ccladding",0,0,0);
  i = i +1;
}


  // Scintillating scsf-78
  //
  m_qFibreScin = new G4Tubs("scsfFibre",0.,m_rSFib,m_length,0.,2.*pi);
  m_qFibreScin_log = new G4LogicalVolume(m_qFibreScin,m_scsf78,"scsf78FibreLog",0,0,0);



  // Cladding on plastic fibre
  //


  m_cladScin_tube = new G4Tubs("cladding",m_rSClad,m_rSFib,m_length,0,2.*pi);
  m_cladScin_log = new G4LogicalVolume(m_cladScin_tube,m_cladScin,"ScladdingScin",0,0,0);


  // Glass 
  //
  m_glass_box = new G4Box("Glass",m_expHall_x,m_expHall_y,1.*cm);
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
void HFDetectorConstruction::SetupDetectors()
{ 

  const G4double zPos = 7.2*m;
  const G4double xPos1 = m_rRod - m_rCFib;
  const G4double xPos2 = -m_del + m_o;
  const G4double xPos3 = xPos2;
  const G4double yPos1 = 0;
  const G4double yPos2 = (-m_x/2) + m_o;
  const G4double yPos3 = -yPos2;

  const unsigned n = m_nRods;
  //const unsigned nRods = 2*(n*(n+1)/2-n*(n/2+1)/4)-n;
  int count=0;

  // create main row
  G4double xStart = m_rRod*(n-1);
  for ( unsigned i=0; i != n; i++ ) {
    G4double xPos = 2*i*m_rRod-xStart;
    char name[6], finame[8], fiiname[9], fiiiname[10], ciname[11], ciiname[12], ciiiname[13];
    sprintf(name,"rod%d",count);
    sprintf(finame,"Cfib1.%d",count);
    sprintf(fiiname,"Cfib2.%d",count);
    sprintf(fiiiname,"Cfib3.%d",count);
    sprintf(ciname,"Cclad1.%d",count);
    sprintf(ciiname,"Cclad2.%d",count);
    sprintf(ciiiname,"Clad3.%d",count);

    m_rods.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,0.,zPos),m_tungRod_log[count],name,m_expHall_log,false,count,m_checkOverlaps));

    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos1,yPos1,0),m_qFibreCher_log[count],finame,m_tungRod_log[count],false,count,m_checkOverlaps));
    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,0),m_cladCher_log[3*count],ciname,m_qFibreCher_log[count],false,count,m_checkOverlaps));

    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos2,yPos2,0),m_qFibreCher_log[count],fiiname,m_tungRod_log[count],false,count,m_checkOverlaps));
    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,0),m_cladCher_log[3*count+1],ciiname,m_qFibreCher_log[count],false,count,m_checkOverlaps));

    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos3,yPos3,0),m_qFibreCher_log[count],fiiiname,m_tungRod_log[count],false,count,m_checkOverlaps));
    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,0),m_cladCher_log[3*count+2],ciiiname,m_qFibreCher_log[count],false,count++,m_checkOverlaps));
  }

  // create upper portion
  const unsigned nd2 = n/2;
  for ( unsigned row = 0; row != nd2; row++ ) {
    G4double xStartr = xStart-(row+1)*m_rRod;
    G4double yPos = m_h*(row+1);
    const unsigned nR = n-row-1;
    for ( unsigned i=0; i != nR; i++ ) {
      G4double xPos = 2*i*m_rRod-xStartr;
    char name[6], finame[8], fiiname[9], fiiiname[10], ciname[11], ciiname[12], ciiiname[13];
    sprintf(name,"rod%d",count);
    sprintf(finame,"Cfib1.%d",count);
    sprintf(fiiname,"Cfib2.%d",count);
    sprintf(fiiiname,"Cfib3.%d",count);
    sprintf(ciname,"Cclad1.%d",count);
    sprintf(ciiname,"Cclad2.%d",count);
    sprintf(ciiiname,"Clad3.%d",count);
      
      m_rods.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_tungRod_log[count],name,m_expHall_log,false,count,m_checkOverlaps));

    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos1,yPos1,0),m_qFibreCher_log[count],finame,m_tungRod_log[count],false,count,m_checkOverlaps));
    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,0),m_cladCher_log[3*count],ciname,m_qFibreCher_log[count],false,count,m_checkOverlaps));

    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos2,yPos2,0),m_qFibreCher_log[count],fiiname,m_tungRod_log[count],false,count,m_checkOverlaps));
    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,0),m_cladCher_log[3*count+1],ciiname,m_qFibreCher_log[count],false,count,m_checkOverlaps));

    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos3,yPos3,0),m_qFibreCher_log[count],fiiiname,m_tungRod_log[count],false,count,m_checkOverlaps));
    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,0),m_cladCher_log[3*count+2],ciiiname,m_qFibreCher_log[count],false,count++,m_checkOverlaps));
    }
  }

  // create lower portion
  for ( unsigned row = 0; row != nd2; row++ ) {
    G4double xStartr = xStart-(row+1)*m_rRod;
    G4double yPos = -m_h*(row+1);
    const unsigned nR = n-row-1;
    for ( unsigned i=0; i != nR; i++ ) {
      G4double xPos = 2*i*m_rRod-xStartr;
      
    char name[6], finame[8], fiiname[9], fiiiname[10], ciname[11], ciiname[12], ciiiname[13];
    sprintf(name,"rod%d",count);
    sprintf(finame,"Cfib1.%d",count);
    sprintf(fiiname,"Cfib2.%d",count);
    sprintf(fiiiname,"Cfib3.%d",count);
    sprintf(ciname,"Cclad1.%d",count);
    sprintf(ciiname,"Cclad2.%d",count);
    sprintf(ciiiname,"Clad3.%d",count);

      m_rods.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_tungRod_log[count],name,m_expHall_log,false,count,m_checkOverlaps));

    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos1,yPos1,0),m_qFibreCher_log[count],finame,m_tungRod_log[count],false,count,m_checkOverlaps));
    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,0),m_cladCher_log[3*count],ciname,m_qFibreCher_log[count],false,count,m_checkOverlaps));

    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos2,yPos2,0),m_qFibreCher_log[count],fiiname,m_tungRod_log[count],false,count,m_checkOverlaps));
    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,0),m_cladCher_log[3*count+1],ciiname,m_qFibreCher_log[count],false,count,m_checkOverlaps));

    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos3,yPos3,0),m_qFibreCher_log[count],fiiiname,m_tungRod_log[count],false,count,m_checkOverlaps));
    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(0,0,0),m_cladCher_log[3*count+2],ciiiname,m_qFibreCher_log[count],false,count++,m_checkOverlaps));
    }
  }

const int num_qFibres = 3*count; 
  // 
  G4cout << "Total Number of rods: " << count << G4endl;
  
  G4cout << "Total Number of cherenkov fibers: " << num_qFibres << G4endl;
  //
  // insert scintillating fibers


  //const unsigned nFibres = 2*(n*(n+1)/2-n*(n/2+1)/4);

  count = 0;
  // create upper lattice
  for ( unsigned row = 0; row != nd2; row++ ) {
    G4double xStartr = xStart-(row+1)*m_rRod;
    G4double yPosLow = m_h*(row+1)-m_l;
    G4double yPosHigh = m_h*(row+1)-m_y;
    const unsigned nR = n-row-1;
    const unsigned nF = 2*(nR-1)+1;
    for ( unsigned i=0; i != nF; i++ ) {
      const G4double xPos = i*m_rRod-xStartr;
      char name[6],cname[7];
      sprintf(name,"Sfib%d",count);
      sprintf(cname,"Sclad%d",count);
     
      if ( i%2 == 1 ) {
      	m_Sfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPosHigh,zPos),m_qFibreScin_log,name,m_expHall_log,false,count,m_checkOverlaps));
      	m_Scladding.push_back(new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),m_cladScin_log,cname,m_qFibreScin_log,false,count++,m_checkOverlaps));
      }
      else {
	m_Sfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPosLow,zPos),m_qFibreScin_log,name,m_expHall_log,false,count,m_checkOverlaps));
	m_Scladding.push_back(new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),m_cladScin_log,cname,m_qFibreScin_log,false,count++,m_checkOverlaps));
      }
    }
  }

  // create lower lattice
  for ( unsigned row = 0; row != nd2; row++ ) {
    G4double xStartr = xStart-(row+1)*m_rRod;
    G4double yPosLow = -m_h*(row+1)+m_y;
    G4double yPosHigh = -m_h*(row+1)+m_l;
    const unsigned nR = n-row-1;
    const unsigned nF = 2*(nR-1)+1;
    for ( unsigned i=0; i != nF; i++ ) {
      const G4double xPos = i*m_rRod-xStartr;
      char name[6], cname[7];
      sprintf(name,"Sfib%d",count);
      sprintf(cname,"Sclad%d",count);
    
      if ( i%2 == 0 ) {
	m_Sfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPosHigh,zPos),m_qFibreScin_log,name,m_expHall_log,false,count,m_checkOverlaps));
	m_Scladding.push_back(new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),m_cladScin_log,cname,m_qFibreScin_log,false,count++,m_checkOverlaps));
      }
      else {
	m_Sfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPosLow,zPos),m_qFibreScin_log,name,m_expHall_log,false,count,m_checkOverlaps));
	m_Scladding.push_back(new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),m_cladScin_log,cname,m_qFibreScin_log,false,count++,m_checkOverlaps));
      }
    }
  }

  //
  G4cout << "Total Number of scintillating fibers: " << count << G4endl;

//
//  // insert cherenkov fibers
//
//
//  //const unsigned nFibres = 2*(n*(n+1)/2-n*(n/2+1)/4);
//
//  count = 0;
//  // create middle lattices
//
//  for ( unsigned row = 0; row != (nd2+1); row++ ) {
//    G4double xStartr = xStart-(row+1)*m_rRod;
//    const unsigned nR = n-row;
//    const unsigned nF = nR;
//  for ( unsigned i=0; i != nF; i++ ) {
//    G4double xPos = 2*i*m_rRod-xStartr-m_rCFib;
//    G4double yPos = row*m_h;
//    char name[8], cname[9];
//    sprintf(name,"Cfib%d",count);
//    sprintf(cname,"Cclad%d",count);
//    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_qFibreCher_log,name,m_expHall_log,false,count,m_checkOverlaps));
//    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_cladCher_log,cname,m_expHall_log,false,count++,m_checkOverlaps));
// }
//}
//
//
//  for ( unsigned row = 1; row != (nd2+1); row++ ) {
//    G4double xStartr = xStart-(row+1)*m_rRod;
//    const unsigned nR = n-row;
//    const unsigned nF = nR;
//  for ( unsigned i=0; i != nF; i++ ) {
//    G4double xPos = 2*i*m_rRod-xStartr-m_rCFib;
//    G4double yPos = row*-m_h;
//    char name[8], cname[9];
//    sprintf(name,"Cfib%d",count);
//    sprintf(cname,"Cclad%d",count);
//    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_qFibreCher_log,name,m_expHall_log,false,count,m_checkOverlaps));
//    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_cladCher_log,cname,m_expHall_log,false,count++,m_checkOverlaps));
// }
//}
//// create upper off-center lattice
//  for ( unsigned row = 0; row != (nd2+1); row++ ) {
//    G4double xStartr = xStart-(row)*m_rRod;
//    const unsigned nR = n-row;
//    const unsigned nF = nR;
//  for ( unsigned i=0; i != nF; i++ ) {
//    G4double xPos = 2*i*m_rRod-xStartr-m_del+m_o;
//    G4double yPos = row*m_h+(m_x/2.)-m_o;
//    char name[8], cname[9];
//    sprintf(name,"Cfib%d",count);
//    sprintf(cname,"Cclad%d",count);
//    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_qFibreCher_log,name,m_expHall_log,false,count,m_checkOverlaps));
//    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_cladCher_log,cname,m_expHall_log,false,count++,m_checkOverlaps));
// }
//}
//
//
//  for ( unsigned row = 1; row != (nd2+1); row++ ) {
//    G4double xStartr = xStart-(row)*m_rRod;
//    const unsigned nR = n-row;
//    const unsigned nF = nR;
//  for ( unsigned i=0; i != nF; i++ ) {
//    G4double xPos = 2*i*m_rRod-xStartr-m_del+m_o;
//    G4double yPos = row*m_h-(m_x/2.)+m_o;
//    char name[8], cname[9];
//    sprintf(name,"Cfib%d",count);
//    sprintf(cname,"Cclad%d",count);
//    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_qFibreCher_log,name,m_expHall_log,false,count,m_checkOverlaps));
//    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_cladCher_log,cname,m_expHall_log,false,count++,m_checkOverlaps));
// }
//}
//
//
////create lower off-center lattice
//  for ( unsigned row = 0; row != (nd2+1); row++ ) {
//    G4double xStartr = xStart-(row)*m_rRod;
//    const unsigned nR = n-row;
//    const unsigned nF = nR;
//  for ( unsigned i=0; i != nF; i++ ) {
//    G4double xPos = 2*i*m_rRod-xStartr-m_del+m_o;
//    G4double yPos = row*-m_h-(m_x/2.)+m_o;
//    char name[8], cname[9];
//    sprintf(name,"Cfib%d",count);
//    sprintf(cname,"Cclad%d",count);
//    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_qFibreCher_log,name,m_expHall_log,false,count,m_checkOverlaps));
//    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_cladCher_log,cname,m_expHall_log,false,count++,m_checkOverlaps));
// }
//}
//
//
//  for ( unsigned row = 1; row != (nd2+1); row++ ) {
//    G4double xStartr = xStart-(row)*m_rRod;
//    const unsigned nR = n-row;
//    const unsigned nF = nR;
//  for ( unsigned i=0; i != nF; i++ ) {
//    G4double xPos = 2*i*m_rRod-xStartr-m_del+m_o;
//    G4double yPos = row*-m_h+(m_x/2.)-m_o;
//    char name[8], cname[9];
//    sprintf(name,"Cfib%d",count);
//    sprintf(cname,"Cclad%d",count);
//    m_Cfibres.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_qFibreCher_log,name,m_expHall_log,false,count,m_checkOverlaps));
//    m_Ccladding.push_back(new G4PVPlacement(0,G4ThreeVector(xPos,yPos,zPos),m_cladCher_log,cname,m_expHall_log,false,count++,m_checkOverlaps));
// }
//}
//
//  G4cout << "Total Number of cherenkov fibers: " << count << G4endl;
//
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

  scsfProps->AddProperty("FASTCOMPONENT",scinEnergies,scinValues,nScinEnergies);
  scsfProps->AddProperty("SLOWCOMPONENT",scinEnergies,scinValues,nScinEnergies);

  //
  // see http://infoscience.epfl.ch/record/164027/files/EPFL_TH5033.pdf
  // for scintillation yield
  scsfProps->AddConstProperty("SCINTILLATIONYIELD", 8300./MeV);
  scsfProps->AddConstProperty("RESOLUTIONSCALE", 2.0);
  scsfProps->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  scsfProps->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
  scsfProps->AddConstProperty("YIELDRATIO", 0.8);

  G4MaterialPropertiesTable *scsfCladProps = new G4MaterialPropertiesTable();
  scsfCladProps->AddProperty("RINDEX",scinEnergies,scinCladRindex,nScinEnergies);
  scsfCladProps->AddProperty("ABSLENGTH",scinEnergies,scinCladAbsLength,nScinEnergies);

  m_scsf78->SetMaterialPropertiesTable(scsfProps);
  //m_scsf78->SetMaterialPropertiesTable(scsfCladProps);
  m_cladScin->SetMaterialPropertiesTable(scsfCladProps);

  // -------------------- glass face ----------------------------

  // put glass plate at back
  m_glass_phys = new G4PVPlacement(0,G4ThreeVector(0.,0.,zPos+m_length+0.5*cm),m_glass_log,"glass",m_expHall_log,false,0,true); 

}


void HFDetectorConstruction::CalculateConstants()
{
  
  m_h = sqrt(3.)*m_rRod;
  m_y = sqrt(3.)*m_rRod/3.;
  m_l = 2.*sqrt(3.)*m_rRod/3;
  m_del = sqrt((m_rRod/2.)*(m_rRod-0.5));
  m_x = sqrt(m_rRod*(2.*m_rRod + 1));
  m_rCFib = 0.99*(m_l-m_rRod);
  m_rCClad = 0.8*m_rCFib;  // radius to core clad interface from fiber axis
  m_rSFib = m_rCFib;
  m_rSClad = m_rCClad;
  m_o = m_rCFib/sqrt(2.);
  if ( m_stacking ) m_stacking->SetFiberRadius(m_rCClad);

  //if ( m_length > m_expHall_z ) m_expHall_z = 1.10*m_length;
  //G4double width = m_nRods*m_rRod;
  //if ( width > m_expHall_x ) { 
  //  m_expHall_x =  1.10*width;
  //  m_expHall_y = 1.1 * width;
  //}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void HFDetectorConstruction::SetNRods(G4int n)
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


void HFDetectorConstruction::SetRRod(G4double r)
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

void HFDetectorConstruction::SetLength(G4double l)
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

void HFDetectorConstruction::SetFibreIndex(G4double n)
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

void HFDetectorConstruction::SetCladIndex(G4double n)
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


void HFDetectorConstruction::SetOverlapCheck(G4bool check)
{
  m_checkOverlaps = check;

  if ( !m_isConstructed ) return;

  G4RunManager::GetRunManager()->GeometryHasBeenModified(); 
  
}


void HFDetectorConstruction::ClearPhysicalVolumes()
{

  assert(m_isConstructed);

  const unsigned nRods = m_rods.size();
  for ( unsigned i=0; i != nRods; i++ ) {
    m_tungRod_log[i]->RemoveDaughter(m_rods[i]);
    delete m_rods[i];
  }
  m_rods.clear();

  const unsigned nSFibs = m_Sfibres.size();
  for ( unsigned i=0; i != nSFibs; i++ ) {
    m_qFibreScin_log->RemoveDaughter(m_Sfibres[i]);
    delete m_Sfibres[i];
  }
  m_Sfibres.clear();

  const unsigned nSClads = m_Scladding.size();
  for ( unsigned i=0; i != nSClads; i++ ) {
    m_cladScin_log->RemoveDaughter(m_Scladding[i]);
    delete m_Scladding[i];
  }
  m_Scladding.clear();

  const unsigned nCFibs = m_qFibreCher_log.size();
  for ( unsigned i=0; i != nCFibs; i++ ) {
    m_qFibreCher_log[i]->RemoveDaughter(m_Cfibres[3*i]);
    m_qFibreCher_log[i]->RemoveDaughter(m_Cfibres[3*i+1]);
    m_qFibreCher_log[i]->RemoveDaughter(m_Cfibres[3*i+2]);
    delete m_Cfibres[3*i];
    delete m_Cfibres[3*i+1];
    delete m_Cfibres[3*i+2];
  }
  m_Cfibres.clear();

  const unsigned nCClads = m_Ccladding.size();
  for ( unsigned i=0; i != nCClads; i++ ) {
    m_cladCher_log[i]->RemoveDaughter(m_Ccladding[i]);
    delete m_Ccladding[i];
  }
  m_Ccladding.clear();

}

void HFDetectorConstruction::ClearLogicalVolumes()
{

  assert(m_isConstructed);

  //m_expHall_log->RemoveDaughter(m_expHall_phys);
const unsigned n_rodfibs = m_tungRod_log.size();
unsigned int i = 0;

while(i < n_rodfibs){
  delete m_tungRod_log[i];
  delete m_qFibreCher_log[i];
  delete m_cladCher_log[3*i];
  delete m_cladCher_log[3*i+1];
  delete m_cladCher_log[3*i+2];
  i = i + 1;
}
  delete m_qFibreScin_log;
  //m_expHall_log->RemoveDaughter(m_expHall_phys);
  //delete m_expHall_phys;
  
  //delete m_expHall_log;
  //delete m_expHall_box;

}


void HFDetectorConstruction::SetStackingAction( HFStackingAction *sa )
{
  m_stacking = sa;
}


//void HFDetectorConstruction::SetParticleGun( HFPrimaryGeneratorAction *gun )
//{
//  m_gun = gun;
//}


