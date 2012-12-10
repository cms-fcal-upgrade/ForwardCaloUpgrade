#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include <cmath>
#include <iostream>
#include <cassert>
#include <algorithm>
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

const int HcalDetId::maxDepthHB;
const int HcalDetId::maxDepthHE;

static const int IPHI_MAX=72;

HcalTopology::HcalTopology(HcalTopology::Mode mode) :
  excludeHB_(false),
  excludeHE_(false),
  excludeHO_(false),
  excludeHF_(false),
  mode_(mode),
  firstHBRing_(1),
  lastHBRing_(16), 
  firstHERing_(16),
  lastHERing_(29), 
  firstHFRing_(29),
  lastHFRing_(41),
  firstHORing_(1),
  lastHORing_(15),
  firstHEDoublePhiRing_((mode==md_H2 || mode==md_H2HE)?(22):(21)),
  firstHFQuadPhiRing_(40),
  firstHETripleDepthRing_((mode==md_H2 || mode==md_H2HE)?(24):(27)),
  singlePhiBins_(72),
  doublePhiBins_(36)
{
}


bool HcalTopology::valid(const HcalDetId& id) const {
  // check the raw rules
  bool ok=validRaw(id);

  ok=ok && !isExcluded(id);

  return ok;
}

bool HcalTopology::isExcluded(const HcalDetId& id) const {
  bool exed=false;
  // first, check the full detector exclusions...  (fast)
  switch (id.subdet()) {
  case(HcalBarrel): exed=excludeHB_; break;
  case(HcalEndcap): exed=excludeHE_; break;
  case(HcalOuter): exed=excludeHO_; break;
  case(HcalForward): exed=excludeHF_; break;
  default: exed=false;
  }
  // next, check the list (slower)
  if (!exed && !exclusionList_.empty()) {
    std::vector<HcalDetId>::const_iterator i=std::lower_bound(exclusionList_.begin(),exclusionList_.end(),id);
    if (i!=exclusionList_.end() && *i==id) exed=true;
  }
  return exed;
}

void HcalTopology::exclude(const HcalDetId& id) {
  std::vector<HcalDetId>::iterator i=std::lower_bound(exclusionList_.begin(),exclusionList_.end(),id);
  if (i==exclusionList_.end() || *i!=id) {
    exclusionList_.insert(i,id);
  }
}

void HcalTopology::excludeSubdetector(HcalSubdetector subdet) {
  switch (subdet) {
  case(HcalBarrel): excludeHB_=true; break;
  case(HcalEndcap): excludeHE_=true; break;
  case(HcalOuter): excludeHO_=true; break;
  case(HcalForward): excludeHF_=true; break;
  default: break;
  }
}

std::vector<DetId> HcalTopology::east(const DetId& id) const {
  std::vector<DetId> vNeighborsDetId;
  HcalDetId neighbors[2];
  for (int i=0;i<decIEta(HcalDetId(id),neighbors);i++)
    vNeighborsDetId.push_back(DetId(neighbors[i].rawId()));
  return vNeighborsDetId;
}

std::vector<DetId> HcalTopology::west(const DetId& id) const {
  std::vector<DetId> vNeighborsDetId;
  HcalDetId neighbors[2];
  for (int i=0;i<incIEta(HcalDetId(id),neighbors);i++)
    vNeighborsDetId.push_back(DetId(neighbors[i].rawId()));
  return  vNeighborsDetId;
}

std::vector<DetId> HcalTopology::north(const DetId& id) const {
  std::vector<DetId> vNeighborsDetId;
  HcalDetId neighbor;
  if (incIPhi(HcalDetId(id),neighbor))
    vNeighborsDetId.push_back(DetId(neighbor.rawId()));
  return  vNeighborsDetId;
}

std::vector<DetId> HcalTopology::south(const DetId& id) const {
  std::vector<DetId> vNeighborsDetId;
  HcalDetId neighbor;
  if (decIPhi(HcalDetId(id),neighbor))
    vNeighborsDetId.push_back(DetId(neighbor.rawId()));
  return  vNeighborsDetId;
}

std::vector<DetId> HcalTopology::up(const DetId& id) const {
  HcalDetId neighbor = id;
  //A.N.
  //  incrementDepth(neighbor);
  std::vector<DetId> vNeighborsDetId;
  if(incrementDepth(neighbor)) 
  {
    vNeighborsDetId.push_back(neighbor);
  }
  return  vNeighborsDetId;
}

std::vector<DetId> HcalTopology::down(const DetId& id) const {
  std::cout << "HcalTopology::down() not yet implemented" << std::endl; 
  std::vector<DetId> vNeighborsDetId;
  return  vNeighborsDetId;
}

int HcalTopology::exclude(HcalSubdetector subdet, int ieta1, int ieta2, int iphi1, int iphi2, int depth1, int depth2) {

  bool exed=false;
  // first, check the full detector exclusions...  (fast)
  switch (subdet) {
  case(HcalBarrel): exed=excludeHB_; break;
  case(HcalEndcap): exed=excludeHE_; break;
  case(HcalOuter): exed=excludeHO_; break;
  case(HcalForward): exed=excludeHF_; break;
  default: exed=false;
  }
  if (exed) return 0; // if the whole detector is excluded...

  int ieta_l=std::min(ieta1,ieta2);
  int ieta_h=std::max(ieta1,ieta2);
  int iphi_l=std::min(iphi1,iphi2);
  int iphi_h=std::max(iphi1,iphi2);
  int depth_l=std::min(depth1,depth2);
  int depth_h=std::max(depth1,depth2);

  int n=0;
  for (int ieta=ieta_l; ieta<=ieta_h; ieta++) 
    for (int iphi=iphi_l; iphi<=iphi_h; iphi++) 
      for (int depth=depth_l; depth<=depth_h; depth++) {
	HcalDetId id(subdet,ieta,iphi,depth);
	if (validRaw(id)) { // use 'validRaw' to include check validity in "uncut" detector
	  exclude(id);  
	  n++;
	}
      }
  return n;
}

  /** Basic rules used to derive this code:
      
  HB has 72 towers in iphi.  Ieta 1-14 have depth=1, Ieta 15-16 have depth=1 or 2.

  HE ieta=16-20 have 72 towers in iphi
     ieta=21-29 have 36 towers in iphi
     ieta=16 is depth 3 only
     ieta=17 is depth 1 only
     ieta=18-26 & 29 have depth 1 and 2
     ieta=27-28 has depth 1-3

  HF ieta=29-39 have 36 in iphi
     ieta=40-41 have 18 in iphi (71,3,7,11...)
     all have two depths


  HO has 15 towers in ieta and 72 in iphi and depth = 4 (one value)

  At H2:

  HE ieta 17 is two depths
  HE ieta 22- have 36 towers in iphi (starts one higher)
  HE ieta 24- has three depths

  */

  /** Is this a valid cell id? */
bool HcalTopology::validRaw(const HcalDetId& id) const {
  bool ok=true;
  int ieta=id.ieta();
  int aieta=id.ietaAbs();
  int depth=id.depth();
  int iphi=id.iphi();

  if ((ieta==0 || iphi<=0 || iphi>IPHI_MAX) || aieta>lastHFRing()) return false; // outer limits
    
  if (ok) {
    HcalSubdetector subdet=id.subdet();
    if (subdet==HcalBarrel) {
      if (mode_==md_SLHC || mode_==md_H2HE) {
	if ((aieta>lastHBRing() || depth>HcalDetId::maxDepthHB || (aieta==lastHBRing() && depth > 2))) ok=false;
      } else {
	if (aieta>lastHBRing() || depth>2 || (aieta<=14 && depth>1)) ok=false;
      }
    } else if (subdet==HcalEndcap) {
      if (mode_==md_H2HE) {
	if (depth>HcalDetId::maxDepthHE || aieta<firstHERing() || aieta>lastHERing() || (aieta==firstHERing() && depth<3) || (aieta>=firstHEDoublePhiRing() && (iphi%2)==0)) ok=false;
      } else if (mode_==md_SLHC) {
        if (depth>getMaxLayersDepth(aieta) || aieta<firstHERing() || aieta>lastHERing() ||
           (aieta>=firstHEDoublePhiRing() && (iphi%2)==0)) ok=false;
      } else {
	if (depth>3 || aieta<firstHERing() || aieta>lastHERing() || (aieta==firstHERing() && depth!=3) || (aieta==17 && depth!=1 && mode_!=md_H2) || // special case at H2
	    (((aieta>=17 && aieta<firstHETripleDepthRing()) || aieta==lastHERing()) && depth>2) ||
	    (aieta>=firstHEDoublePhiRing() && (iphi%2)==0)) ok=false;
      }
    } else if (subdet==HcalOuter) {
      if (aieta>lastHORing() || iphi>IPHI_MAX || depth!=4) ok=false;
    } else if (subdet==HcalForward) {
      if (aieta<firstHFRing() || aieta>lastHFRing() || ((iphi%2)==0) || (depth>2) || (aieta>=firstHFQuadPhiRing() && ((iphi+1)%4)!=0)) ok=false;
    } else ok=false;
  }
    
  return ok;
}

  
bool HcalTopology::incIPhi(const HcalDetId& id, HcalDetId &neighbor) const {
  bool ok=valid(id);
  if (ok) {
    switch (id.subdet()) {
    case (HcalBarrel):
    case (HcalOuter):
      if (id.iphi()==IPHI_MAX) neighbor=HcalDetId(id.subdet(),id.ieta(),1,id.depth()); 
      else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()+1,id.depth()); 
      break;
    case (HcalEndcap):
      if (id.ietaAbs()>=firstHEDoublePhiRing()) {
	if (id.iphi()==IPHI_MAX-1) neighbor=HcalDetId(id.subdet(),id.ieta(),1,id.depth()); 
	else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()+2,id.depth()); 
      } else {
	if (id.iphi()==IPHI_MAX) neighbor=HcalDetId(id.subdet(),id.ieta(),1,id.depth()); 
	else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()+1,id.depth()); 
      }	
      break;
    case (HcalForward):
      if (id.ietaAbs()>=firstHFQuadPhiRing()) {
	if (id.iphi()==IPHI_MAX-1) neighbor=HcalDetId(id.subdet(),id.ieta(),3,id.depth()); 
	else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()+4,id.depth()); 
      } else {
	if (id.iphi()==IPHI_MAX-1) neighbor=HcalDetId(id.subdet(),id.ieta(),1,id.depth()); 
	else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()+2,id.depth()); 
      }
      break;
    default: ok=false;
    }
  } 
  return ok;
}

/** Get the neighbor (if present) of the given cell with lower iphi */
bool HcalTopology::decIPhi(const HcalDetId& id, HcalDetId &neighbor) const {
  bool ok=valid(id);
  if (ok) {
    switch (id.subdet()) {
    case (HcalBarrel):
    case (HcalOuter):
      if (id.iphi()==1) neighbor=HcalDetId(id.subdet(),id.ieta(),IPHI_MAX,id.depth()); 
      else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()-1,id.depth()); 
      break;
    case (HcalEndcap):
      if (id.ietaAbs()>=firstHEDoublePhiRing()) {
	if (id.iphi()==1) neighbor=HcalDetId(id.subdet(),id.ieta(),IPHI_MAX-1,id.depth()); 
	else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()-2,id.depth()); 
      } else {
	if (id.iphi()==1) neighbor=HcalDetId(id.subdet(),id.ieta(),IPHI_MAX,id.depth()); 
	else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()-1,id.depth()); 
      }
      break;
    case (HcalForward):
      if (id.ietaAbs()>=firstHFQuadPhiRing()) {
	if (id.iphi()==3) neighbor=HcalDetId(id.subdet(),id.ieta(),IPHI_MAX-1,id.depth()); 
	else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()-4,id.depth()); 
      } else {
	if (id.iphi()==1) neighbor=HcalDetId(id.subdet(),id.ieta(),IPHI_MAX-1,id.depth()); 
	else neighbor=HcalDetId(id.subdet(),id.ieta(),id.iphi()-2,id.depth()); 
      }
      break;
    default: ok=false;
    }
  } 
  return ok;
}

int HcalTopology::incIEta(const HcalDetId& id, HcalDetId neighbors[2]) const {
  if (id.zside()==1) return incAIEta(id,neighbors);
  else return decAIEta(id,neighbors);
}

int HcalTopology::decIEta(const HcalDetId& id, HcalDetId neighbors[2]) const {
  if (id.zside()==1) return decAIEta(id,neighbors);
  else return incAIEta(id,neighbors);
}

/** Increasing in |ieta|, there is always at most one neighbor */
int HcalTopology::incAIEta(const HcalDetId& id, HcalDetId neighbors[2]) const {
  int n=1;
  int aieta=id.ietaAbs();

  if (aieta==firstHEDoublePhiRing()-1 && (id.iphi()%2)==0) 
    neighbors[0]=HcalDetId(id.subdet(),(aieta+1)*id.zside(),id.iphi()-1,id.depth());
  else if (aieta==firstHFQuadPhiRing()-1 && ((id.iphi()+1)%4)!=0) 
    neighbors[0]=HcalDetId(id.subdet(),(aieta+1)*id.zside(),((id.iphi()==1)?(71):(id.iphi()-2)),id.depth());
  else if (aieta==lastHBRing()) 
    neighbors[0]=HcalDetId(HcalEndcap,(aieta+1)*id.zside(),id.iphi(),1);
  else if (aieta==lastHERing()) 
    neighbors[0]=HcalDetId(HcalForward,(aieta+1)*id.zside(),id.iphi(),1);
  else
    neighbors[0]=HcalDetId(id.subdet(),(aieta+1)*id.zside(),id.iphi(),id.depth());
    
  if (!valid(neighbors[0])) n=0;
  return n;
}

/** Decreasing in |ieta|, there are be two neighbors of 40 and 21*/
int HcalTopology::decAIEta(const HcalDetId& id, HcalDetId neighbors[2]) const {
  int n=1;
  int aieta=id.ietaAbs();

  if (aieta==firstHEDoublePhiRing()) { 
    n=2;
    neighbors[0]=HcalDetId(id.subdet(),(aieta-1)*id.zside(),id.iphi(),id.depth());
    neighbors[1]=HcalDetId(id.subdet(),(aieta-1)*id.zside(),id.iphi()+1,id.depth());
  } else if (aieta==firstHFQuadPhiRing()) {
    n=2;
    neighbors[0]=HcalDetId(id.subdet(),(aieta-1)*id.zside(),id.iphi(),id.depth());
    if (id.iphi()==IPHI_MAX-1) neighbors[1]=HcalDetId(id.subdet(),(aieta-1)*id.zside(),1,id.depth());
    else neighbors[1]=HcalDetId(id.subdet(),(aieta-1)*id.zside(),id.iphi()+2,id.depth());
  } else if (aieta==1) {
    neighbors[0]=HcalDetId(id.subdet(),-aieta*id.zside(),id.iphi(),id.depth());
  } else if (aieta==lastHBRing()+1) {
    neighbors[0]=HcalDetId(HcalBarrel,(aieta-1)*id.zside(),id.iphi(),id.depth());
  } else if (aieta==lastHERing()+1) {
    neighbors[0]=HcalDetId(HcalEndcap,(aieta-1)*id.zside(),id.iphi(),id.depth());
  } else
    neighbors[0]=HcalDetId(id.subdet(),(aieta-1)*id.zside(),id.iphi(),id.depth());
  
  if (!valid(neighbors[0]) && n==2) {
    if (!valid(neighbors[1])) n=0;
    else {
      n=1;
      neighbors[0]=neighbors[1];
    }
  }
  if (n==2 && !valid(neighbors[1])) n=1;
  if (n==1 && !valid(neighbors[0])) n=0;

  return n;
}


void HcalTopology::depthBinInformation(HcalSubdetector subdet, int etaRing,
                                       int & nDepthBins, int & startingBin) const {

  if(subdet == HcalBarrel) {
    if (mode_==md_SLHC || mode_==md_H2HE) {
      startingBin = 1;
      if (etaRing==lastHBRing()) {
	nDepthBins = 2;
      } else {
	nDepthBins = HcalDetId::maxDepthHB;
      }
    } else {
      if (etaRing<=14) {
	nDepthBins = 1;
	startingBin = 1;
      } else {
	nDepthBins = 2;
	startingBin = 1;
      }
    }
  } else if(subdet == HcalEndcap) {
    if (mode_==md_SLHC) {
      nDepthBins  = getMaxLayersDepth(etaRing);
      startingBin = 1;
      if (etaRing==firstHERing()) startingBin = 3;
    } else if (mode_==md_H2HE) {
      if (etaRing==firstHERing()) {
	nDepthBins  = HcalDetId::maxDepthHE - 2;
	startingBin = 3;
      } else {
	nDepthBins  = HcalDetId::maxDepthHE;
	startingBin = 1;
      }
    } else {
      if (etaRing==firstHERing()) {
	nDepthBins = 1;
	startingBin = 3;
      } else if (etaRing==17) {
	nDepthBins = 1;
	startingBin = 1;
      } else if (etaRing==lastHERing()) {
	nDepthBins = 2;
	startingBin = 1;
      }	else {
	nDepthBins = (etaRing >= firstHETripleDepthRing()) ? 3 : 2;
	startingBin = 1;
      }
    }
  } else if(subdet == HcalForward) {
    nDepthBins  = 2;
    startingBin = 1;
  } else if(subdet == HcalOuter) {
    nDepthBins = 1;
    startingBin = 4;
  } else {
    std::cerr << "Bad HCAL subdetector " << subdet << std::endl;
  }
}


bool HcalTopology::incrementDepth(HcalDetId & detId) const
{
  HcalSubdetector subdet = detId.subdet();
  int ieta = detId.ieta();
  int etaRing = detId.ietaAbs();
  int depth = detId.depth();
  int nDepthBins, startingBin;
  depthBinInformation(subdet, etaRing, nDepthBins, startingBin);

  // see if the new depth bin exists
  ++depth;
  if(depth > nDepthBins) {
    // handle on a case-by-case basis
    if(subdet == HcalBarrel && etaRing < lastHORing())  {
      // HO
      subdet = HcalOuter;
      depth = 4;
    } else if(subdet == HcalBarrel && etaRing == lastHBRing()) {
      // overlap
      subdet = HcalEndcap;
    } else if(subdet == HcalEndcap && etaRing ==  lastHERing()-1) {
      // guard ring HF29 is behind HE 28
      subdet = HcalForward;
      (ieta > 0) ? ++ieta : --ieta;
      depth = 1;
    } else if(subdet == HcalEndcap && etaRing == lastHERing() && mode_ != md_SLHC) {
      // split cells go to bigger granularity.  Ring 29 -> 28
      (ieta > 0) ? --ieta : ++ieta;
    } else {
      // no more chances
      detId = HcalDetId();
      return false;
    }
  }
  detId = HcalDetId(subdet, ieta, detId.iphi(), depth);
  //A.N.  
  // assert(validRaw(detId));
  return validRaw(detId);
  //A.N.  return true;
}


int HcalTopology::nPhiBins(int etaRing) const {
  int lastPhiBin=singlePhiBins_;
  if (etaRing>= firstHFQuadPhiRing()) lastPhiBin=doublePhiBins_/2;
  else if (etaRing>= firstHEDoublePhiRing()) lastPhiBin=doublePhiBins_;
  return lastPhiBin;
}

void HcalTopology::getDepthSegmentation(unsigned ring, std::vector<int> & readoutDepths) const
{
  // if it doesn't exist, return the first entry with a lower index.  So if we only
  // have entries for 1 and 17, any input from 1-16 should return the entry for ring 1
  SegmentationMap::const_iterator pos = depthSegmentation_.upper_bound(ring);
  if(pos == depthSegmentation_.begin()) {
    throw cms::Exception("HcalTopology") << "No depth segmentation found for ring" << ring;
  }
  --pos;
    // pos now refers to the last element with key <= ring.
  readoutDepths = pos->second;
}

void HcalTopology::setDepthSegmentation(unsigned ring, const std::vector<int> & readoutDepths)
{
  depthSegmentation_[ring] = readoutDepths;
}

std::pair<int, int> HcalTopology::segmentBoundaries(unsigned ring, unsigned depth) const {
  std::vector<int> readoutDepths;
  getDepthSegmentation(ring, readoutDepths);
  int d1 = std::lower_bound(readoutDepths.begin(), readoutDepths.end(), depth) - readoutDepths.begin();
  int d2 = std::upper_bound(readoutDepths.begin(), readoutDepths.end(), depth) - readoutDepths.begin();
  return std::pair<int, int>(d1, d2);
}

int HcalTopology::getMaxLayersDepth(unsigned ring) const
{
 return maxLayersDepth[ring-16];
}

void HcalTopology::setMaxLayersDepth()
{
  float slhcDepths[5]       = {400.458, 418.768, 436.168, 493., 549.268};
  float ring16slhcDepths[3] = {418.768, 450., 470.968};
  float ring17slhcDepths[5] = {409.698, 435., 460., 495., 514.468};
  float ring18slhcDepths[5] = {391.883, 439., 467., 504. , 540.568};
  float layerDepths[19] = {400.458, 408.718, 416.978, 425.248, 433.508, 441.768,
                           450.038, 458.298, 466.558, 474.828, 483.088, 491.348,
                           499.618, 507.878, 516.138, 524.398, 532.668, 540.928,
                           549.268};
  float ringZstart[14]  = {418.768, 409.698, 391.883, 400.458, 400.458, 400.458,
                           400.458, 400.458, 400.458, 400.458, 400.458, 400.458,
                           400.458, 400.458};
  float ringZend  [14]  = {470.968, 514.468, 540.568, 549.268, 549.268, 549.268,
                           549.268, 549.268, 549.268, 549.268, 549.268, 549.268,
                           549.268, 549.268};
  int   layerShift [3]  = {2, 1, -1};

// loop over HE iring
//-------------------
  for( int iring=16; iring<=29; ++iring ) {
    if(mode_!=md_SLHC && iring <=29) { 
      int maxDepth = HcalDetId::maxDepthHE;
      maxLayersDepth.push_back(maxDepth);
      continue;
    } 

    std::vector<float> depths;
    std::vector<int> readoutDepths;
    getDepthSegmentation(iring, readoutDepths);

    if( readoutDepths.size() > 0 ) {

      int depth = readoutDepths[0];
      depths.push_back(ringZstart[iring-16]);   
      int layer = 1;
      float lastDepth = depths[0];
      for( unsigned int i=1; i<readoutDepths.size(); ++i ) {
        layer = i;
        if( iring <= 18) layer += layerShift[iring-16];
        if( layerDepths[layer] > ringZend[iring-16]) 
        {
          depths.push_back(ringZend[iring-16]);
          break;
        }

        if( depth != readoutDepths[i] ) 
        {
          depth = readoutDepths[i];
          if( (iring == 16 && depth > (HcalDetId::maxDepthHE)-2) ||
                              depth > (HcalDetId::maxDepthHE) ) break;
          if( layerDepths[layer] > lastDepth ) 
          {
            depths.push_back(layerDepths[layer]);
            lastDepth = layerDepths[layer];
          }
        }
      }
      if (depths.back() < ringZend[iring-16]) depths.push_back(ringZend[iring-16]);  

    } else {
        if (iring == 16)      for (int i=0; i<3; ++i) depths.push_back(ring16slhcDepths[i]);
        else if (iring == 17) for (int i=0; i<5; ++i) depths.push_back(ring17slhcDepths[i]);
        else if (iring == 18) for (int i=0; i<5; ++i) depths.push_back(ring18slhcDepths[i]);
        else                  for (int i=0; i<5; ++i) depths.push_back(slhcDepths[i]);
    }

    int idepth = depths.size()-1; 
    maxLayersDepth.push_back(idepth);
  }

}    


