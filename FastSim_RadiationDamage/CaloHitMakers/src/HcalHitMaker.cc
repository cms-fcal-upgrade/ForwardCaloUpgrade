#include "FastSimulation/CaloHitMakers/interface/HcalHitMaker.h"
#include "FastSimulation/CaloGeometryTools/interface/CaloGeometryHelper.h"
#include "FastSimulation/CalorimeterProperties/interface/CalorimeterProperties.h"
#include "FastSimulation/CalorimeterProperties/interface/HCALProperties.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <algorithm>
#include <cmath>

typedef ROOT::Math::Transform3DPJ::Point Point;

HcalHitMaker::HcalHitMaker(EcalHitMaker& grid,unsigned shower)
  :CaloHitMaker(grid.getCalorimeter(),DetId::Hcal,HcalHitMaker::getSubHcalDet(grid.getFSimTrack()),
		grid.getFSimTrack()->onHcal()?grid.getFSimTrack()->onHcal():grid.getFSimTrack()->onVFcal()+1,shower),
   myGrid(grid),  myTrack((grid.getFSimTrack())),darkening(0)
{
  // normalize the direction
  ecalEntrance_=myGrid.ecalEntrance();
  particleDirection=myTrack->ecalEntrance().Vect().Unit();
  radiusFactor_=(EMSHOWER)? moliereRadius:interactionLength;
  mapCalculated_=false;
  //std::cout << " Famos HCAL " << grid.getTrack()->onHcal() << " " <<  grid.getTrack()->onVFcal() << " " << showerType << std::endl;
  if(EMSHOWER&&(abs(grid.getFSimTrack()->type())!=11 && grid.getFSimTrack()->type()!=22))
    {
      std::cout << " FamosHcalHitMaker : Strange. The following shower has EM type" << std::endl <<* grid.getFSimTrack() << std::endl;
    }

  //setup darkening (if enabled)
  isDamaged = ((HCALProperties*)theCaloProperties)->isDamaged();
  lumiDarkening = ((HCALProperties*)theCaloProperties)->lumiDarkening();
  if (isDamaged && lumiDarkening > 0) darkening = new HEDarkening();

  //setup distances for darkening
  double depthECAL  = 0.;
  double depthGAP   = 0.;
  if(myTrack->onEcal() ) {//for HB and HE (not HF)
    if(EMSHOWER){
      depthECAL  = grid.ecalTotalX0();         // ECAL depth segment
      depthGAP = grid.ecalHcalGapTotalX0();  // GAP  depth
	}
	else if(HADSHOWER){
	  depthECAL  = grid.ecalTotalL0();         // ECAL depth segment
      depthGAP = grid.ecalHcalGapTotalL0();  // GAP  depth
	}
  }
  depthToHCAL = depthECAL + depthGAP;
  
  if(EMSHOWER) depthHCAL = grid.hcalTotalX0();
  else if(HADSHOWER) depthHCAL = grid.hcalTotalL0();
  
  //approximate layer depths
  Lzero = 15.0/1511.0;
  Llay = 88.0/1511.0;
  
}

XYZPoint
HcalHitMaker::getPoint(double r, double phi, unsigned layer)
{
  XYZPoint point(r*radiusFactor_*std::cos(phi),r*radiusFactor_*std::sin(phi),0.);
 
 // Watch out !!!! (Point) is a real point in the MathCore terminology (not a redefined a XYZPoint which
  // is actually a XYZVector in the MatchCore terminology). Therefore, the Transform3D is correctly applied
  point = locToGlobal_((Point)point);

  return point;
}

bool 
HcalHitMaker::addHit(double r,double phi,unsigned layer)
{
  return addHit(getPoint(r,phi,layer),layer);
}

HcalDetId HcalHitMaker::getDetId(const XYZPoint& point, unsigned layer)
{
  DetId thecellID(myCalorimeter->getClosestCell(point,false,false));
  
  HcalDetId myDetId(thecellID);

  if ( myDetId.subdetId() == HcalForward ) {
    int mylayer = layer;
    if ( myDetId.depth()==2 ) {
      mylayer = (int)layer;
    } else {
      mylayer = 1;
    }
    myDetId =  HcalDetId((HcalSubdetector)myDetId.subdetId(),myDetId.ieta(),myDetId.iphi(),mylayer);
  }  
  
  return myDetId;
}

bool HcalHitMaker::addHit(const XYZPoint& point, unsigned layer)
{
  // Temporary nasty hacks to avoid misbehaviour of not-intended-for-that
  //  getClosestCell in case of large (eta beyond HF ...)  and in EM showers 
  if(fabs(point.Z())>2000 || fabs(point.X())>2000 || fabs(point.Y())>2000) 
    { 
      if(EMSHOWER) 
	edm::LogWarning("HcalHitMaker") << "received a hit very far from the detector " << point << " coming from an electromagnetic shower. - Ignoring it" << std::endl;
      else if(HADSHOWER)
	edm::LogWarning("HcalHitMaker") << "received a hit very far from the detector " << point << " coming from a hadron shower. - Ignoring it" << std::endl;
      else if(MIP)
	edm::LogWarning("HcalHitMaker") << "received a hit very far from the detector " << point << " coming from a muon. - Ignoring it" << std::endl;
      return false; 
    } 

  double pointeta = fabs(point.eta());
  if(pointeta > 5.19) return false; 

  HcalDetId myDetId = getDetId(point,layer);
  DetId thecellID = myDetId;
  
//   if ( myDetId.subdetId() == HcalForward ) {
//     std::cout << "HcalHitMaker : " << point.Z() << " " << myDetId.depth()    << std::endl;
//   }
//   std::cout << "BEFORE" << std::endl;
//   std::cout << "HcalHitMaker : subdetId : " << myDetId.subdetId() << std::endl;
//   std::cout << "HcalHitMaker : depth    : " << myDetId.depth()    << std::endl;
//   std::cout << "HcalHitMaker : ieta     : " << myDetId.ieta()     << std::endl;
//   std::cout << "HcalHitMaker : iphi     : " << myDetId.iphi()     << std::endl;
//   std::cout << "HcalHitMaker : spotE    : " << spotEnergy         << std::endl;
//   std::cout << "HcalHitMaker : point.X  : " << point.X()          << std::endl;
//   std::cout << "HcalHitMaker : point.Y  : " << point.Y()          << std::endl;
//   std::cout << "HcalHitMaker : point.Z  : " << point.Z()          << std::endl;

  if(!thecellID.null())
    {
	  //calculate darkening if in HE & enabled
	  //apply weight only to hit energy (not stored spotenergy because it gets reused)
	  double weight = 1.0;
	  if (darkening !=0 && myDetId.subdet()==HcalEndcap){
	    double r = std::sqrt(point.X()*point.X() + point.Y()*point.Y());
	    double Lhcal = (currentDepth_ - depthToHCAL)/depthHCAL;
	    int lay;
		if( 0 < Lhcal && Lhcal <= Lzero) lay = 0;
		else lay = (int)((Lhcal - Lzero)/Llay)+1;
		//edm::LogInfo("FastCalorimetry") << "HcalHitMaker:LayerAbs >>>  position: "<< currentDepth_ << "    depthToHCAL: " << depthToHCAL << "   depthHCAL: " << depthHCAL;
		//edm::LogInfo("FastCalorimetry") << "HcalHitMaker:LayerFrac >>>  Lhcal: "<< Lhcal << "    Lzero: " << Lzero << "   Llay: " << Llay;
        //edm::LogInfo("FastCalorimetry") << "HcalHitMaker:Darkening >>>  position: "<< currentDepth_ * interactionLength << "    lay: " << lay << "   R: " << r << " cm ";
        
        float normalized_lumi = darkening->int_lumi(lumiDarkening);
        float dose_acquired   = darkening->dose(lay,r);
        weight *= darkening->degradation(normalized_lumi * dose_acquired);
        //edm::LogInfo("FastCalorimetry") << "HcalHitMaker:         >>> norm_Lumi: " << normalized_lumi << "  dose: " << dose_acquired << "    coefficient = " << weight;
	  }
	
      uint32_t cell(thecellID.rawId());
      
      //      std::cout << " FamosHcalHitMaker::addHit - the cell num " << cell
      //      		<< std::endl;

      std::map<uint32_t,float>::iterator cellitr;
      cellitr = hitMap_.find(cell);
      if(cellitr==hitMap_.end())
	{
	  hitMap_.insert(std::pair<uint32_t,float>(cell,weight*spotEnergy));
	}
      else
	{
	  cellitr->second+=weight*spotEnergy;
	}
      return true;
    }
  return false;
}

bool 
HcalHitMaker::setDepth(double depth,bool inCm)
{
  currentDepth_=depth;
  std::vector<CaloSegment>::const_iterator segiterator;
  if(inCm)
    {
      segiterator = find_if(myGrid.getSegments().begin(),myGrid.getSegments().end(),CaloSegment::inSegment(currentDepth_));
    }
  else
    {
      if(EMSHOWER)
	segiterator = find_if(myGrid.getSegments().begin(),myGrid.getSegments().end(),CaloSegment::inX0Segment(currentDepth_));
      
      //Hadron shower 
      if(HADSHOWER)
	segiterator = find_if(myGrid.getSegments().begin(),myGrid.getSegments().end(),CaloSegment::inL0Segment(currentDepth_));
    }
  
  if(segiterator==myGrid.getSegments().end()) 
    {
      // Special trick  - As advised by Salavat, no leakage should be simulated
      if(depth > myGrid.getSegments().back().sL0Exit())
	{
	  segiterator= find_if(myGrid.getSegments().begin(),myGrid.getSegments().end(),CaloSegment::inL0Segment(myGrid.getSegments().back().sL0Exit()-1.));
	  depth=segiterator->sL0Exit()-1.;
	  currentDepth_=depth;
	  if(segiterator==myGrid.getSegments().end())
	    {
	      std::cout << " Could not go at such depth " << EMSHOWER << "  " << currentDepth_ << std::endl;
	      std::cout << " Track " << *(myGrid.getFSimTrack()) << std::endl;
	      return false;
	    }
	}
      else
	{
	  std::cout << " Could not go at such depth " << EMSHOWER << "  " << currentDepth_ << " " << myGrid.getSegments().back().sL0Exit() << std::endl; 
	  std::cout << " Track " << *(myGrid.getFSimTrack()) << std::endl; 
	  return false; 
	}
    }


  XYZPoint origin;
  if(inCm)
    {
      origin=segiterator->positionAtDepthincm(currentDepth_);
    }
  else
    {
      if(EMSHOWER)
	origin=segiterator->positionAtDepthinX0(currentDepth_);
      if(HADSHOWER)
	origin=segiterator->positionAtDepthinL0(currentDepth_);
    }
  XYZVector zaxis(0,0,1);
  XYZVector planeVec1=(zaxis.Cross(particleDirection)).Unit();
  locToGlobal_=Transform3D(Point(0,0,0),
			   Point(0,0,1),
			   Point(1,0,0),
			   (Point)origin,
			   (Point)(origin+particleDirection),
			   (Point)(origin+planeVec1));
  return true;
}
