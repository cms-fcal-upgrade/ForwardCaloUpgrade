// Class containing radiation damage calculations

#ifndef RadiationDamageProperties_H
#define RadiationDamageProperties_H


namespace edm {
  class ParameterSet;
}

class RadiationDamageProperties
{

 public:

  RadiationDamageProperties() { ;}
  virtual ~RadiationDamageProperties() { ;}

  void setLumi(const edm::ParameterSet& rad);

  double doseProfile(double eta) const;
  double relLY_EM(double eta) const;
  double relLY_Had(double eta, double L) const;

  double relativeLightYield(double eta, double L) const;

  double relativeLYLoss(double eta) const;
  double constantTermLYLoss(double eta) const;
  double nonlinearityLYLoss(double eta, double e) const;
  double LightCollectionEfficiency(double, double) const;
  double InducedAbsorptionHadronic(double, double) const;
  double InducedAbsorptionEM(double, double) const;
  double AgingVPT(double, double) const;
  double DegradationMeanEM50GeV(double) const;
  double DegradationNonLinearityEM50GeV(double, double) const;
  double ResolutionConstantTermEM50GeV(double) const;



 private:

  double lumiTotal_;
  double lumiInstant_;
  bool isDamaged_;

};

#endif
