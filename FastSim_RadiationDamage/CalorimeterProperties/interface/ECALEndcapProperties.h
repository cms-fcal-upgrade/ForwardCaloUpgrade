#ifndef ECALEndcapProperties_H
#define ECALEndcapProperties_H

#include "FastSimulation/CalorimeterProperties/interface/ECALProperties.h"

/** 
 * Functions to return atomic properties of the material
 * A_eff and Z_eff are computed as the A-weighted sums 
 * of the A's and the Z's of Pb, W and O
 *
 * \author Patrick Janot
 * \date: 25-Jan-2004
 */ 

#include "FastSimulation/CalorimeterProperties/interface/RadiationDamageProperties.h"

namespace edm { 
  class ParameterSet;
}

class ECALEndcapProperties : public ECALProperties 
{

 public:

  ECALEndcapProperties(const edm::ParameterSet& fastDet) ;

  virtual ~ECALEndcapProperties() { }

 /// Thickness (in cm): 22.0 for Standard ECAL
  double thickness(double eta) const { return thickness_; }

  ///Photostatistics (photons/GeV) in the homegeneous material: 50E3  for Standard ECAL
  inline double photoStatistics() const { return photoStatistics_; }

  ///Light Collection efficiency [Default : 3.0%]
  inline double lightCollectionEfficiency(double eta) const;

  ///Light Collection uniformity 0.003 for Standard ECAL
  inline double lightCollectionUniformity(double eta) const;

  ///Light Collection non-linearity 1.0 for Standard ECAL
  inline double lightCollectionNonLinearity(double eta, double e) const;

 private:

  RadiationDamageProperties radDamage;

};

#endif
