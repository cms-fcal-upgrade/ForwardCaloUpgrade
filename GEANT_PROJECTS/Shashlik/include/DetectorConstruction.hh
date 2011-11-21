//
// ********************************************************************
// ********************************************************************
//

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4UniformMagField;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

  public:

     void SetEcalAbsMaterial (G4String);     
     void SetEcalAbsThickness(G4double);     

     void SetEcalSensMaterial (G4String);     
     void SetEcalSensThickness(G4double);
     
     void SetNbOfEcalLayers (G4int);   
     void ComputeEcalParameters();   
     
     void SetMagField(G4double);

     G4VPhysicalVolume* Construct();

     void UpdateGeometry();

  public:

     void PrintCalorParameters(); 
                    
     const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;};           
//     const G4VPhysicalVolume* GetEcal()       {return physiEcal;};
     const G4VPhysicalVolume* GetEcal()       {return phyEcalSens;};
     const G4VPhysicalVolume* GetHcal()       {return physiSens;};
     const G4VPhysicalVolume* GetZero()       {return phySensZero;};

     G4int GetNbOfEcalLayers()         {return NbOfEcalLayers;}; 
     
     G4Material* GetEcalAbsMaterial()  {return EcalAbsMaterial;};
     G4double    GetEcalAbsThickness() {return EcalAbsThickness;};      
     
     G4Material* GetEcalSensMaterial()  {return EcalSensMaterial;};
     G4double    GetEcalSensThickness() {return EcalSensThickness;};
     
  private:

     G4Material*        EcalAbsMaterial;
     G4double           EcalAbsThickness;
     G4Material*        EcalSensMaterial;
     G4double           EcalSensThickness;
     G4int              NbOfEcalLayers;
     G4double           EcalLayerThickness;
     G4double           EcalCalorThickness;
     
     G4double           LayerThickness;
     G4double           AbsorberThickness;
     G4double           WrapThickness;
     G4double           AirGapThickness;
     G4double           GapThickness;
     G4double           SensThickness;
     G4int              NbOfHcalLayers;
     G4double           CalorSizeYZ;
     G4double           CalorThickness;
     
     G4Material*        defaultMaterial;
     G4Material*        pAlum; 
     G4Material*        pLead;
     G4Material*        pSci; 
     G4Material*        pAir;
     G4Material*        pBra;
     G4Material*        pG10; 
     G4Material*        pCab;
     G4Material*        pPwo;
     G4Material*        pTung;

     G4Material*        pSens1;
     G4Material*        pSens2;
     G4Material*        pSens3;
     G4Material*        pSens4 ;
          
     G4double           ZeroWrapThick;
     G4double           ZeroGapThick;
     G4double           ZeroSensThick;

     G4double           CablesThickness;
     G4double           G10plateThickness;
     G4double           EcalThickness;
     G4double           SuppThickness;
     G4double           AluSeThickness;
     G4double           LeadSeThickness;

            
     G4Box*             solidWorld;    //pointer to the solid World 
     G4LogicalVolume*   logicWorld;    //pointer to the logical World
     G4VPhysicalVolume* physiWorld;    //pointer to the physical World

     G4Box*             solidCalor;    //pointer to the solid Calor 
     G4LogicalVolume*   logicCalor;    //pointer to the logical Calor
     G4VPhysicalVolume* physiCalor;    //pointer to the physical Calor
     
     G4Box*             solidLayer;    //pointer to the solid Layer 
     G4LogicalVolume*   logicLayer;    //pointer to the logical Layer
     G4VPhysicalVolume* physiLayer;    //pointer to the physical Layer
         
     G4Box*             solidAbsorber;  //pointer to the solid Absorber
     G4LogicalVolume*   logicAbsorber;  //pointer to the logical Absorber
     G4VPhysicalVolume* physiAbsorber;  //pointer to the physical Absorber

     G4Box*             solidWrap;      //pointer to the solid Wrapper
     G4LogicalVolume*   logicWrap;      //pointer to the logical Wrapper
     G4VPhysicalVolume* physiWrap;      //pointer to the physical Wrapper
     
     G4Box*             solidGap;       //pointer to the solid Gap
     G4LogicalVolume*   logicGap;       //pointer to the logical Gap
     G4VPhysicalVolume* physiGap;       //pointer to the physical Gap

     G4Box*             solidSens;      //pointer to the solid Sensitive
     G4LogicalVolume*   logicSens;      //pointer to the logical Sensitive
     G4VPhysicalVolume* physiSens;      //pointer to the physical Sensitive

     G4Box*             solWrapZero;    //pointer to Zero layer
     G4LogicalVolume*   logWrapZero;    //pointer to Zero layer
     G4VPhysicalVolume* phyWrapZero;    //pointer to Zero layer

     G4Box*             solGapZero;     //pointer to Zero layer
     G4LogicalVolume*   logGapZero;     //pointer to Zero layer
     G4VPhysicalVolume* phyGapZero;     //pointer to Zero layer

     G4Box*             solSensZero;    //pointer to Zero layer
     G4LogicalVolume*   logSensZero;    //pointer to Zero layer
     G4VPhysicalVolume* phySensZero;    //pointer to Zero layer

     G4Box*             solidCables;    //pointer to cables
     G4LogicalVolume*   logicCables;    //pointer to cables
     G4VPhysicalVolume* physiCables;    //pointer to cables

     G4Box*             solidG10plate;  //pointer to G10 plate
     G4LogicalVolume*   logicG10plate;  //pointer to G10 plate
     G4VPhysicalVolume* physiG10plate;  //pointer to G10 plate

     G4Box*             solidEcal;         //pointer to ECAL
     G4LogicalVolume*   logicEcal;         //pointer to ECAL
     G4VPhysicalVolume* physiEcal;         //pointer to ECAL

     G4Box*             solEcalLayer;      //pointer to ECAL layer
     G4LogicalVolume*   logEcalLayer;         
     G4VPhysicalVolume* phyEcalLayer;         

     G4Box*             solEcalAbs;      //pointer to ECAL absorber   
     G4LogicalVolume*   logEcalAbs;
     G4VPhysicalVolume* phyEcalAbs;

     G4Box*             solEcalSens;     //pointer to ECAL sensitive
     G4LogicalVolume*   logEcalSens;
     G4VPhysicalVolume* phyEcalSens;

     G4Box*             solidSupport;      //pointer to Al support
     G4LogicalVolume*   logicSupport;      //pointer to Al support
     G4VPhysicalVolume* physiSupport;      //pointer to Al support

     G4Box*             solidAluSe;        //pointer to Al part of SE
     G4LogicalVolume*   logicAluSe;        //pointer to Al part of SE
     G4VPhysicalVolume* physiAluSe;        //pointer to Al part of SE

     G4Box*             solidLeadSe;       //pointer to Pb part of SE
     G4LogicalVolume*   logicLeadSe;       //pointer to Pb part of SE  
     G4VPhysicalVolume* physiLeadSe;       //pointer to Pb part of SE

     G4UniformMagField* magField;          //pointer to the magnetic field

     DetectorMessenger* detectorMessenger;  //pointer to the Messenger
     
  private:
    
     void DefineMaterials();
     G4VPhysicalVolume* ConstructCalorimeter();     
};

// Compute derived parameters of the ECAL calorimeter:
//----------------------------------------------------
inline void DetectorConstruction::ComputeEcalParameters()
{
   EcalLayerThickness = EcalAbsThickness + EcalSensThickness;
   EcalCalorThickness = NbOfEcalLayers*EcalLayerThickness;

   if( EcalCalorThickness > 220.0 ) {
       G4cout << "\n ===> Stop from DetectorConstruction::ComputeEcalParameters(): "
              << "\n EcalCalorThickness = " << EcalCalorThickness  
              << " mm, greater than 220.0*mm. "
              << "\n Check input parameters: NbOfEcalLayers, EcalAbsThickness"
              << " and EcalSensThickness "
              << G4endl;
//       return;
       exit(1);
     }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

