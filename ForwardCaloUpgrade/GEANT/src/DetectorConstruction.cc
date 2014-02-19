//
// ********************************************************************
// This is description of Hadron Endcap calorimeter of CMS experiment
// with ECAL, SE and all dead material in front of 
// ********************************************************************
//

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction():defaultMaterial(0),
 solidWorld(0),logicWorld(0),physiWorld(0),
 solidCalor(0),logicCalor(0),physiCalor(0),
 solidLayer(0),logicLayer(0),physiLayer(0),
 solidAbsorber(0),logicAbsorber(0),physiAbsorber(0),
 solidWrap(0),logicWrap(0),physiWrap(0),
 solidGap(0),logicGap(0),physiGap(0),
 solidSens(0),logicSens(0),physiSens(0),
 solWrapZero(0),logWrapZero(0),phyWrapZero(0),
 solGapZero(0),logGapZero(0),phyGapZero(0),
 solSensZero(0),logSensZero(0),phySensZero(0),
 solidCables(0),logicCables(0),physiCables(0),
 solidG10plate(0),logicG10plate(0),physiG10plate(0),
 solidEcal(0),logicEcal(0),physiEcal(0),
 solidSupport(0),logicSupport(0),physiSupport(0),
 solidAluSe(0),logicAluSe(0),physiAluSe(0),
 solidLeadSe(0),logicLeadSe(0),physiLeadSe(0)
{

// default parameter values of the calorimeter, Hadron Endcap (HE)

  AbsorberThickness =  79.0*mm;
  AirGapThickness   =   9.0*mm;
  WrapThickness     =   7.0*mm;
  GapThickness      =   6.0*mm;
  SensThickness     =   3.9*mm;
  NbOfLayers        =  17;
  CalorSizeYZ       = 100.0*cm;

  LayerThickness = AbsorberThickness + AirGapThickness;
  CalorThickness = NbOfLayers*LayerThickness;

// parameters for zero scintillator layer in front of HE

  ZeroWrapThick = 15.0*mm;
  ZeroGapThick  = 13.0*mm;
  ZeroSensThick =  9.0*mm;
  
// dead material in front of HE

  CablesThickness   =  1.5*mm;
  G10plateThickness =  2.5*mm;
  EcalThickness     = 22.0*cm;
  SuppThickness     =  4.5*mm;
  AluSeThickness     = 3.0*cm;

// materials

  DefineMaterials();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructCalorimeter();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
 //This function illustrates the possible ways to define materials
 
G4String symbol;             // a=mass of a mole;
G4double a, z, density;      // z=mean number of protons;  

G4int ncomponents, natoms;
G4double fractionmass;

//
// define Elements
//

G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
G4Element* Si = new G4Element("Silicon" ,symbol="Si", z=14., a= 28.08*g/mole);
G4Element* Cl = new G4Element("Chlorine",symbol="Cl", z=17., a= 35.45*g/mole);
G4Element* Cu = new G4Element("Copper"  ,symbol="Cu", z=29., a= 63.54*g/mole);
G4Element* Zn = new G4Element("Zinc"    ,symbol="Zn", z=30., a= 65.38*g/mole);
G4Element* W  = new G4Element("Tungsten",symbol="W" , z=74., a=183.84*g/mole);
G4Element* Pb = new G4Element("Lead"    ,symbol="Pb", z=82., a=207.20*g/mole);

// define simple materials

pAlum =
new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.700*g/cm3);

pLead =
new G4Material("PbSe", z=82., a=207.20*g/mole, density=11.34*g/cm3);

// define a material from elements.   case 1: chemical molecule

// define scintillator (C_9H_10)_n
//---------------------------------
pSci = 
new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
pSci->AddElement(C, natoms=9);
pSci->AddElement(H, natoms=10);

// define a material from elements.   case 2: mixture by fractional mass

// define Air:
//------------
pAir = 
new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
pAir->AddElement(N, fractionmass=0.7);
pAir->AddElement(O, fractionmass=0.3);

// define brass (70% Cu + 30% Zn):
//--------------------------------
pBra =
new G4Material("Brass"  , density= 8.530*g/cm3, ncomponents=2);
pBra->AddElement(Cu, fractionmass=0.7);
pBra->AddElement(Zn, fractionmass=0.3);

// define G10 material (22% Si + 27% C + 42% O + 6% H + 3% Cl):
// ------------------------------------------------------------
pG10 =
new G4Material("G10plate", density= 1.70*g/cm3, ncomponents=5);
pG10->AddElement(Si, fractionmass=0.22);
pG10->AddElement(C , fractionmass=0.27);
pG10->AddElement(O , fractionmass=0.42);
pG10->AddElement(H , fractionmass=0.06);
pG10->AddElement(Cl, fractionmass=0.03);

// define cables material (58% Cu + 26% C + 14% O + 2% H):
// -------------------------------------------------------
pCab =
new G4Material("Cables" , density= 2.680*g/cm3, ncomponents=4);
pCab->AddElement(Cu, fractionmass=0.58);
pCab->AddElement(C , fractionmass=0.26);
pCab->AddElement(O , fractionmass=0.14);
pCab->AddElement(H , fractionmass=0.02);

// define ECAL crystals PWO4 (45.5% PB + 40.4% W + 14.1% O):
// ---------------------------------------------------------
pPwo =
new G4Material("Ecal" , density= 8.300*g/cm3, ncomponents=3);
pPwo->AddElement(Pb, fractionmass=0.455);
pPwo->AddElement(W , fractionmass=0.404);
pPwo->AddElement(O , fractionmass=0.141);

// examples of vacuum

G4Material* Vacuum =
new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 2.73*kelvin, 3.e-18*pascal);

G4Material* beam = 
new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,
                       kStateGas, STP_Temperature, 2.e-2*bar);
beam->AddMaterial(pAir, fractionmass=1.);

G4cout << *(G4Material::GetMaterialTable()) << G4endl;

//default materials of the World

defaultMaterial  = Vacuum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructCalorimeter()
{

// World
//=======

  solidWorld = new G4Box("World",                       //its name
                          200.0*cm, 75.0*cm, 75.0*cm);  //its size
                         
  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                   defaultMaterial,	//its material
                                   "World");		//its name
                                   
  physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 logicWorld,		//its logical volume				 
                                 "World",		//its name
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number
  
// Calorimeter, only air space (x = 0.0 cm - 149.6 cm )
//======================================================

  solidCalor=0; logicCalor=0; physiCalor=0;
  
  solidCalor = new G4Box("Calorimeter",		        //its name
    		   CalorThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.); //size
    			     
  logicCalor = new G4LogicalVolume(solidCalor,  	//its solid
      			           defaultMaterial,	//its material
      		     	          "Calorimeter");	//its name
    				       
  physiCalor = new G4PVPlacement(0,			//no rotation
                   G4ThreeVector(CalorThickness/2.,0.0,0.0), //at (74.80,0,0)
                                 logicCalor,	        //its logical volume
                                "Calorimeter",	        //its name
                                 logicWorld,	        //its mother  volume
                                 false,		        //no boolean operation
                                 0);		        //copy number
  
// Layer, only air space (79.0*mm brass + 9.0*mm air gap = 88.0*mm)
//=================================================================

  solidLayer=0; logicLayer=0; physiLayer=0;

  solidLayer = new G4Box("Layer",			//its name
                   LayerThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.); //size
                       
  logicLayer = new G4LogicalVolume(solidLayer,	//its solid
                                   pAir,	        //its material
                                  "Layer");	        //its name

  physiLayer = new G4PVReplica("Layer",		        //its name
    		                logicLayer,	        //its logical volume
      		                logicCalor,	        //its mother
                                kXAxis,		        //axis of replication
                                NbOfLayers,	        //number of replica
                                LayerThickness);	//width of replica

// Replication may occur along:
//-----------------------------
// o Cartesian axes (kXAxis,kYAxis,kZAxis)
//
//   The replications, of specified width have coordinates of
//   form (-width*(nReplicas-1)*0.5+n*width,0,0) where n=0.. nReplicas-1
//   for the case of kXAxis, and are unrotated.
//
// o Radial axis (cylindrical polar) (kRho)
//
//   The replications are cons/tubs sections, centred on the origin
//   and are unrotated.
//   They have radii of width*n+offset to width*(n+1)+offset
//                      where n=0..nReplicas-1
//
// o Phi axis (cylindrical polar) (kPhi)
//   The replications are `phi sections' or wedges, and of cons/tubs form
//   They have phi of offset+n*width to offset+(n+1)*width where
//   n=0..nReplicas-1
  
// Absorber (79.0*mm brass inside layer)
//======================================

  solidAbsorber=0; logicAbsorber=0; physiAbsorber=0;  
  
  solidAbsorber = new G4Box("Brass",   	                //its name
                      AbsorberThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.); 
                          
  logicAbsorber = new G4LogicalVolume(solidAbsorber,    //its solid
      			              pBra,             //its material
      			             "Brass");          //name
      			                  
  physiAbsorber = new G4PVPlacement(0,		        //no rotation
        	      G4ThreeVector(-AirGapThickness/2.,0.,0.), //its position
                                     logicAbsorber,     //its logical volume
                                    "Brass",            //its name
                                     logicLayer,        //its mother
                                     false,             //no boulean operat
                                     0);                //copy number

// Al Wrapper 7.0*mm for scintillator (0.5*mm wall thickness)
//===========================================================
                           
  solidWrap=0; logicWrap=0; physiWrap=0;
      
  solidWrap = new G4Box("Wrapper",
                         WrapThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);
  
  logicWrap = new G4LogicalVolume(solidWrap,
                                  pAlum,
                                 "Wrapper");
                                   
  physiWrap = new G4PVPlacement(0,                     //no rotation
                  G4ThreeVector(AbsorberThickness/2.,0.,0.), //its position
                                logicWrap,             //its logical volume$
                               "Wrapper",              //its name
                                logicLayer,            //its mother
                                false,                 //no boulean operat
                                0);                    //copy number

// Air gap 6.0*mm inside Al wrapper
//=================================
  
  solidGap=0; logicGap=0; physiGap=0;
                                  
  solidGap = new G4Box("Gap",
                        GapThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);
     
  logicGap = new G4LogicalVolume(solidGap,
                                 pAir,
                                "Gap");

  physiGap = new G4PVPlacement(0,                       //no rotation
                               G4ThreeVector(0.,0.,0.), //its position
                               logicGap,                //its logical volume
                              "Gap",                    //its name
                               logicWrap,               //its mother
                               false,                   //no boulean operat
                               0);                      //copy number
                                   
// Sensitive Scintillator 3.9*mm thickness (inside air gap)
//=========================================================

  solidSens=0; logicSens=0; physiSens=0;

  solidSens = new G4Box("Scintil",
                         SensThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);

  logicSens = new G4LogicalVolume(solidSens,
                                  pSci,
                                 "Scintil");

  physiSens = new G4PVPlacement(0,                       //no rotation
                                G4ThreeVector(0.,0.,0.), //its position
                                logicSens,               //its logical volume$
                               "Scintil",                //its name
                                logicGap,                //its mother
                                false,                   //no boulean operat
                                0);                      //copy number

// Al Wrapper 15.0*mm for zero layer (1.0*mm wall thickness)
//===========================================================
                           
  solWrapZero=0; logWrapZero=0; phyWrapZero=0;
      
  solWrapZero = new G4Box("Zwrapper",
                           WrapThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);
  
  logWrapZero = new G4LogicalVolume(solWrapZero,
                                    pAlum,
                                   "Zwrapper");
                                   
  phyWrapZero = new G4PVPlacement(0,                       //no rotation
                  G4ThreeVector(-ZeroWrapThick/2.,0.,0.),  //position
                                 logWrapZero,              //logical volume
                                "Zwrapper",                //name
                                 logicWorld,               //mother
                                 false,                    //no boulean operat
                                 0);                       //copy number

// Air gap 13.0*mm inside Al wrapper for zero layer
//=================================================
  
  solGapZero=0; logGapZero=0; phyGapZero=0;
                                  
  solGapZero = new G4Box("ZeroGap",
                          ZeroGapThick/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);
     
  logGapZero = new G4LogicalVolume(solGapZero,
                                 pAir,
                                "ZeroGap");

  phyGapZero = new G4PVPlacement(0,                       
                               G4ThreeVector(0.,0.,0.), 
                               logGapZero,               
                              "ZeroGap",                    
                               logWrapZero,              
                               false,                   
                               0);                      

// Scintillator 9.0*mm thickness of zero layer 
//=============================================

  solSensZero=0; logSensZero=0; phySensZero=0;

  solSensZero = new G4Box("ZeroScin",
                           ZeroSensThick/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);

  logSensZero = new G4LogicalVolume(solSensZero,
                                    pSci,
                                   "ZeroScin");

  phySensZero = new G4PVPlacement(0,                       
                                  G4ThreeVector(0.,0.,0.), 
                                  logSensZero,               
                                 "ZeroScin",                
                                  logGapZero,                
                                  false,                  
                                  0);                      

// Cables 1.5*mm (from ECAL) in front of HE (at -35.8*cm)
//=======================================================

  solidCables=0; logicCables=0; physiCables=0;
      
  solidCables = new G4Box("Cables",
                           CablesThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);
  
  logicCables = new G4LogicalVolume(solidCables,
                                    pCab,
                                   "Cables");
                                   
  physiCables = new G4PVPlacement(0,                     
                    G4ThreeVector(-35.8*cm, 0.0, 0.0), 
                                   logicCables,             
                                  "Cables",             
                                   logicWorld,            
                                   false,                 
                                   0);                   

// G10 plate 2.5*mm behind of ECAL and in front of HE (at -59.80*cm)
//===================================================================

  solidG10plate=0; logicG10plate=0; physiG10plate=0;

  solidG10plate = new G4Box("G10plate",
                      G10plateThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);

  logicG10plate = new G4LogicalVolume(solidG10plate,
                                    pG10,
                                   "G10plate");
                                     
  physiG10plate = new G4PVPlacement(0,
                      G4ThreeVector(-59.8*cm, 0.0, 0.0),
                                     logicG10plate,  
                                    "G10plate",
                                     logicWorld,
                                     false,
                                     1);

// G10 plate 2.5*mm (copy) in front of SE (at -101.425*cm)
//=================================================

  physiG10plate = new G4PVPlacement(0,
                      G4ThreeVector(-101.425*cm, 0.0, 0.0),
                                     logicG10plate,
                                    "G10plate",
                                     logicWorld,
                                     false,
                                     2);

// ECAL (PWO4) 22.0*cm in front of HE (at -70.80*cm)
//===================================================

  solidEcal=0; logicEcal=0; physiEcal=0;
  
  solidEcal = new G4Box("Ecal",  
                         EcalThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);
  
  logicEcal = new G4LogicalVolume(solidEcal,
                                  pPwo,
                                 "Ecal");
                                  
  physiEcal = new G4PVPlacement(0,
                  G4ThreeVector(-70.8*cm, 0.0, 0.0),
                                 logicEcal,
                                "Ecal",
                                 logicWorld,
                                 false,
                                 0);

// Al support 4.5*mm in front of ECAL (at -82.025*cm)
//===================================================

  solidSupport=0; logicSupport=0; physiSupport=0;
    
  solidSupport = new G4Box("Support",
                            SuppThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);

  logicSupport = new G4LogicalVolume(solidSupport,
                                     pAlum,
                                    "Support");

  physiSupport = new G4PVPlacement(0,
                     G4ThreeVector(-82.025*cm, 0.0, 0.0),
                                    logicSupport,
                                   "Support",
                                    logicWorld,
                                    false,
                                    0);

// Alum. part of SE 3.0*cm in front of ECAL (at -98.3*cm)
//=======================================================

  solidAluSe=0; logicAluSe=0; physiAluSe=0;

  solidAluSe = new G4Box("AlumSe",
                          AluSeThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);
 
  logicAluSe = new G4LogicalVolume(solidAluSe,
                                   pAlum,
                                  "AlumSe");

  physiAluSe = new G4PVPlacement(0,
                   G4ThreeVector(-98.3*cm, 0.0, 0.0),
                                  logicAluSe,
                                 "AlumSe",
                                  logicWorld,
                                  false,
                                  0);

// Lead part of SE 1.5*cm in front of ECAL (at -100.55*cm)
//========================================================

  solidLeadSe=0; logicLeadSe=0; physiLeadSe=0;
                                
  solidLeadSe = new G4Box("LeadSe",
                           LeadSeThickness/2.,CalorSizeYZ/2.,CalorSizeYZ/2.);
                                
  logicLeadSe = new G4LogicalVolume(solidLeadSe,
                                    pLead,
                                   "LeadSe");

  physiLeadSe = new G4PVPlacement(0,
                    G4ThreeVector(-100.55*cm, 0.0, 0.0),
                                   logicLeadSe,
                                  "LeadSe",
                                   logicWorld,
                                   false,
                                   0);

  PrintCalorParameters();     
  
// example how to change cuts in different medias through
// the input cards

// Determine region "Brass"

    G4Region* aRegion = new G4Region("Brass");
    logicAbsorber->SetRegion(aRegion);
    aRegion->AddRootLogicalVolume(logicAbsorber);

// And put the production cuts

    G4Region* reg1 = G4RegionStore::GetInstance()->GetRegion("Brass");
    G4ProductionCuts* cut1 = new G4ProductionCuts;
    cut1->SetProductionCut(0.5*mm);
    reg1->SetProductionCuts(cut1);

// Determine region "Scintillator"

    G4Region* bRegion = new G4Region("Scintillator");
    logicSens ->SetRegion(bRegion);
    bRegion->AddRootLogicalVolume(logicSens);

// And put the production cuts

    G4Region* reg2 = G4RegionStore::GetInstance()->GetRegion("Scintillator");
    G4ProductionCuts* cut2 = new G4ProductionCuts;
    cut2->SetProductionCut(0.1*mm);
    reg2->SetProductionCuts(cut2);

//                                        
// Visualization attributes
//
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

//  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
//  simpleBoxVisAtt->SetVisibility(true);
//  logicCalor->SetVisAttributes(simpleBoxVisAtt);

//
// Below are vis attributes that permits someone to test / play 
// with the interactive expansion / contraction geometry system of the
// vis/OpenInventor driver :

// {G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
//  simpleBoxVisAtt->SetVisibility(true);
//  delete logicCalor->GetVisAttributes();
//  logicCalor->SetVisAttributes(simpleBoxVisAtt);}


 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  atb->SetForceSolid(true);
//  logicLayer->SetVisAttributes(atb);}
//  logicAbsorber->SetVisAttributes(atb);}
//  logicWrap->SetVisAttributes(atb);}
//  logicGap->SetVisAttributes(atb);}
  logicSens->SetVisAttributes(atb);}
  
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  atb->SetForceSolid(true);
  logSensZero->SetVisAttributes(atb);}

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  atb->SetForceSolid(true);
  logicCables->SetVisAttributes(atb);}

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  atb->SetForceSolid(true);
  logicG10plate->SetVisAttributes(atb);}

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));
  atb->SetForceSolid(true);  
  logicEcal->SetVisAttributes(atb);} 

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  atb->SetForceSolid(true);  
  logicSupport->SetVisAttributes(atb);} 

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  atb->SetForceSolid(true);
  logicAluSe->SetVisAttributes(atb);}

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  atb->SetForceSolid(true);
  logicLeadSe->SetVisAttributes(atb);}

  
// {//Set opacity = 0.2 then transparency = 1 - 0.2 = 0.8
//  G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.2));
//  atb->SetForceSolid(true);
//  logicGap->SetVisAttributes(atb);}

// {//Set opacity = 0.2 then transparency = 1 - 0.2 = 0.8
//  G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,1.0));
//  atb->SetForceSolid(true);
//  logicGap->SetVisAttributes(atb);}

// {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
//  atb->SetForceSolid(true);
//  logicWrap->SetVisAttributes(atb);}

//
// always return the physical World
//
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintCalorParameters()
{
  G4cout << "\n------------------------------------------------------------"
         << "\n---> The calorimeter is " << NbOfLayers << " layers of: [ "
         << AbsorberThickness/mm << "mm of " << "Brass" 
         << " + "
         << SensThickness/mm << "mm of " << "Scintillator" << " ] " 
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

