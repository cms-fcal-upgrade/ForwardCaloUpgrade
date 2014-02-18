#include "Geometry/CaloGeometry/interface/CaloGenericDetId.h"
#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <CLHEP/Geometry/Point3D.h>
#include <CLHEP/Geometry/Plane3D.h>

#include <Math/Transform3D.h>
#include <Math/EulerAngles.h>

typedef CaloCellGeometry::CCGFloat CCGFloat ;
typedef CaloCellGeometry::Pt3D     Pt3D     ;
typedef CaloCellGeometry::Pt3DVec  Pt3DVec  ;
typedef HepGeom::Plane3D<CCGFloat> Pl3D     ;

typedef CaloCellGeometry::Tr3D     Tr3D     ;

typedef HepGeom::Vector3D<double> DVec3D ;
typedef HepGeom::Plane3D<double>  DPlane3D ;
typedef HepGeom::Point3D<double>  DPt3D ;


void
EcalEndcapGeometry::getSummary( CaloSubdetectorGeometry::TrVec&  tVec ,
				CaloSubdetectorGeometry::IVec&   iVec ,   
				CaloSubdetectorGeometry::DimVec& dVec   )  const
{
//hack for new SLHC version with crystals in 2 longitudinal pieces
//but in the same shape as the existing endcap crystals, perhaps longer
//or shorter depending on the need

   dVec.reserve( 2*numberOfShapes()*numberOfParametersPerShape() ) ;

// Need to make new arrays of shape parameters. These are
// same as GEANT TRAP. For a shape like the existing endcap
// crystal (square faces, front and back, only 2 adjacent tapered faces)
// a = front side length
// A = rear  side length
// L = length of crystal
// the 11 parameters are
// 0: L/2
// 1: atan( (A-a)/(sqrt(2)*L) )
// 2: pi/4
// 3: a/2
// 4: a/2
// 5: a/2
// 6: 0
// 7: A/2
// 8: A/2
// 9: A/2
//10: 0
//
// Let the original have dimensions a, A, L as above
// Let the front xtal have dimensions a1, A1, L1
// Let the rear xtal have dimensions a2, A2, L2
// Let there be a gap G. User specifies G, L1, L2
// Then a1 = a, A1 = a + (A-a)*L1/L, 
//      a2 = a + (A-a)*(L1+G)/L, A2 = a + (A-a)*(L1+L2+G)/L

   const double L  ( 2*parVecVec().front()[0] ) ;
   const double a  ( 2*parVecVec().front()[3] ) ;
   const double A  ( 2*parVecVec().front()[7] ) ;
   const double L1 ( 24.*L/50. ) ;
   const double L2 ( 25.*L/50. ) ;
   const double G  ( L-L1-L2 ) ;   //JC make sure L1+L2+G ==L
   const double a1 ( a ) ;
   const double A1 ( a + (A-a)*L1/L ) ;
   const double a2 ( a + (A-a)*(L1+G)/L ) ;
   const double A2 ( a + (A-a)*(L1+L2+G)/L ) ;

   dVec.push_back( L1/2. ) ;
   dVec.push_back( atan( (A-a)/(sqrt(2.)*L) ) ) ;
   dVec.push_back( M_PI/4. ) ;
   dVec.push_back( a1/2.   ) ;
   dVec.push_back( a1/2.   ) ;
   dVec.push_back( a1/2.   ) ;
   dVec.push_back( 0.      ) ;
   dVec.push_back( A1/2.   ) ;
   dVec.push_back( A1/2.   ) ;
   dVec.push_back( A1/2.   ) ;
   dVec.push_back( 0.      ) ;

   dVec.push_back( L2/2. ) ;
   dVec.push_back( atan( (A2-a2)/(sqrt(2.)*L2) ) ) ;
   dVec.push_back( M_PI/4. ) ;
   dVec.push_back( a2/2.   ) ;
   dVec.push_back( a2/2.   ) ;
   dVec.push_back( a2/2.   ) ;
   dVec.push_back( 0.      ) ;
   dVec.push_back( A2/2.   ) ;
   dVec.push_back( A2/2.   ) ;
   dVec.push_back( A2/2.   ) ;
   dVec.push_back( 0.      ) ;

   const unsigned int nC ( m_validIds.size() ) ; //#of crystals. 

   tVec = TrVec( 2*nC*6, 0. ) ;  //#position pars 3 for Translation, 3 for rotation
   iVec = IVec(  2*nC  , 0  ) ;  //# DetIDs
   unsigned int it ( 0 ) ;
   for( uint32_t i ( 0 ) ; i != nC ; ++i, it+=6 )
   {
      iVec[ i      ] = 0 ;
      iVec[ i + nC ] = 1 ;  

      // get help from TruncatedPyramid::getTransform() and localCorners()
      // use the "3 point" constructor for Tr3D
      // requires 3 points in local and transformed (global) coordinates
      // must do in double prec to avoid problems with CLHEP internal checks

      // first we get the existing crystal cell geometry object
      const CaloCellGeometry* ptr ( cellGeomPtr( i ) ) ;
      assert( 0 != ptr ) ;
      const TruncatedPyramid* tptr ( dynamic_cast<const TruncatedPyramid*>(ptr) ) ;

      // get local coord of shape 1
      Pt3DVec lc     ( 8, Pt3D(0.,0.,0.) ) ; //8 of 3D points specified below, initialize to 0
      Pt3D    lFront ( 0,0,0 ) ;
      TruncatedPyramid::localCorners( lc, &dVec.front(), lFront ) ; //local corners for pyramaid shape from first element of dVec, lFront point is changed to center of front. 
      const Pt3D  lBack   ( 0.25*(lc[4]+lc[5]+lc[6]+lc[7]) ) ;
      const DPt3D dlCorn  ( lc[0].x(), lc[0].y(), lc[0].z() ) ;
      const DPt3D dlFront ( lFront.x(), lFront.y(), lFront.z() ) ;
      const DPt3D dlBack  ( lBack.x(), lBack.y(), lBack.z() ) ;

      const double dFtoC ( ( dlCorn-dlFront ).mag() ) ;
      const double dFtoB ( ( dlBack-dlFront ).mag() ) ;
      const double aCtoB ( ( dlCorn-dlFront ).angle( dlBack-dlFront ) ) ;

      // the front face center
      const GlobalPoint& p ( ptr->getPosition() ) ;
      const DPt3D        dgFront ( p.x(), p.y(), p.z() ) ;
      
      // the corners of the existing crystals in global coordinates
      const CaloCellGeometry::CornersVec& co ( ptr->getCorners() ) ;
      const DPt3D gCorn ( co[0].x(), co[0].y(), co[0].z() ) ;
      DPt3D dgCorn ( dgFront + dFtoC*( gCorn-dgFront ).unit() ) ;

      // the centers of all faces must lie along the axis of existing crystal
      const double fr1ToBk1 ( sqrt( L1*L1 + (A1-a1)*(A1-a1)/2. ) ) ;
      const GlobalPoint pBack ( p + fr1ToBk1*tptr->axis() ) ;
      const DPt3D gBack ( pBack.x(), pBack.y(), pBack.z() ) ;
      const DPt3D dgBack ( dgFront + dFtoB*( gBack-dgFront ).unit() ) ;

      //-----------------------------
      // this section is to deal with a precision check in Tr3D's constructor
      // that has to do with precision. It prints an error if the angle
      // between two vectors defined by the initial 3 points is preserved
      // to within 1 micro-radian. Hence this angle is checked and
      // a rotation made to correct it before passing the points
      // to the constructor.

      double aCtoBg ( ( dgCorn-dgFront).angle( dgBack-dgFront ) ) ;
      double dangle ( aCtoB - aCtoBg ) ;

      if( 1.e-6 < fabs(dangle) )//guard against precision problems
      {
	 const DPlane3D dgPl ( dgFront, dgCorn, dgBack ) ;
	 const DPt3D    dp2  ( dgFront + dgPl.normal().unit() ) ;

	 dgCorn = ( dgFront + HepGeom::Rotate3D( -dangle, dgFront, dp2 )*
		    DVec3D( dgCorn - dgFront ) ) ;

	 aCtoBg = (dgCorn-dgFront).angle( dgBack-dgFront ) ;
	 dangle = ( aCtoB - aCtoBg ) ;
	 if( 1.e-6<dangle) std::cout<<"After Fix Dangle="<<dangle<<std::endl;
      }
      //-----------------------------

      // the transform
      const Tr3D tr ( dlFront, dlBack, dlCorn,
		      dgFront, dgBack, dgCorn ) ;

      // translation components
      const CLHEP::Hep3Vector  tt ( tr.getTranslation() ) ;
      tVec[ it + 0 ] = tt.x() ;
      tVec[ it + 1 ] = tt.y() ;
      tVec[ it + 2 ] = tt.z() ;
//      std::cout<<"i="<<i<<", Front translation = "<<tt<<std::endl;

      // rotation components
      const CLHEP::HepRotation rr ( tr.getRotation() ) ;
      const ROOT::Math::Transform3D rot ( rr.xx(), rr.xy(), rr.xz(), tt.x(),
					  rr.yx(), rr.yy(), rr.yz(), tt.y(),
					  rr.zx(), rr.zy(), rr.zz(), tt.z()  ) ;

      ROOT::Math::EulerAngles ea ;
      rot.GetRotation( ea ) ;
//      std::cout<<"Front rotation = "<<ea<<std::endl;
      tVec[ it + 3 ] = ea.Phi() ;
      tVec[ it + 4 ] = ea.Theta() ;
      tVec[ it + 5 ] = ea.Psi() ;

      //================ Now do rear shape, shape #2, similar techniques

      // local coords
      Pt3DVec rlc     ( 8, Pt3D(0.,0.,0.) ) ;
      Pt3D    rlFront ( 0,0,0 ) ;
      TruncatedPyramid::localCorners( rlc, &dVec.front()+11, rlFront ) ;
      const Pt3D  rlBack   ( 0.25*(rlc[4]+rlc[5]+rlc[6]+rlc[7]) ) ;
      const DPt3D rdlCorn  ( rlc[0].x(), rlc[0].y(), rlc[0].z() ) ;
      const DPt3D rdlBack  ( rlBack.x(), rlBack.y(), rlBack.z() ) ;
      const DPt3D rdlFront ( rlFront.x(), rlFront.y(), rlFront.z() ) ;
      const double raCtoB ( ( rdlCorn-rdlFront ).angle( rdlBack-rdlFront ) ) ;

      // global coords: first the front of shape 2, global
      const double fr1ToFr2 ( sqrt( (L1+G)*(L1+G) + (a2-a1)*(a2-a1)/2. ) ) ;
      const GlobalPoint rp  ( p + fr1ToFr2*tptr->axis() ) ;
      const DPt3D       rdgFront ( rp.x(), rp.y(), rp.z() ) ;

      // now the back of shape 2, global
      const double fr1ToBk2 ( sqrt( (L1+L2+G)*(L1+L2+G) + (A2-a1)*(A2-a1)/2. ) ) ;
      const GlobalPoint rb  ( p + fr1ToBk2*tptr->axis() ) ;
      const DPt3D       rdgBack ( rb.x(), rb.y(), rb.z() ) ;

      // corner of shape 2, global
      const GlobalVector rgv ( (co[4]-co[0]).unit() ) ;
      const GlobalPoint  rgC  ( co[0] + ( L1+G )*rgv ) ;
      DPt3D rdgCorn ( rgC.x(), rgC.y(), rgC.z() ) ;

      //-------------------------
      double raCtoBg ( ( rdgCorn-rdgFront).angle( rdgBack-rdgFront ) ) ;
      double rdangle ( raCtoB - raCtoBg ) ;

//      std::cout<<"rdangle="<<rdangle<<std::endl;

      if( 1.e-6 < fabs(rdangle) )//guard against precision problems
      {
	 const DPlane3D rdgPl ( rdgFront, rdgCorn, rdgBack ) ;
	 const DPt3D    rdp2  ( rdgFront + rdgPl.normal().unit() ) ;

	 rdgCorn = ( rdgFront + HepGeom::Rotate3D( -rdangle, rdgFront, rdp2 )*
		     DVec3D( rdgCorn - rdgFront ) ) ;

	 raCtoBg = (rdgCorn-rdgFront).angle( rdgBack-rdgFront ) ;
	 rdangle = ( aCtoB - aCtoBg ) ;
	 if( 1.e-6<rdangle) std::cout<<"After Fix Dangle="<<rdangle<<std::endl;
      }
      //-----------------------------

      // the transform
      const Tr3D rtr ( rdlFront, rdlBack, rdlCorn,
		       rdgFront, rdgBack, rdgCorn ) ;

      // translation component
      const CLHEP::Hep3Vector  rtt ( rtr.getTranslation() ) ;
      tVec[ it + 0 + 6*nC ] = rtt.x() ;
      tVec[ it + 1 + 6*nC ] = rtt.y() ;
      tVec[ it + 2 + 6*nC ] = rtt.z() ;
//      std::cout<<"i="<<i<<", Rear translation = "<<rtt<<std::endl;

      // rotation component
      const CLHEP::HepRotation rrr ( rtr.getRotation() ) ;
      const ROOT::Math::Transform3D rotr ( rrr.xx(), rrr.xy(), rrr.xz(), rtt.x(),
					   rrr.yx(), rrr.yy(), rrr.yz(), rtt.y(),
					   rrr.zx(), rrr.zy(), rrr.zz(), rtt.z()  ) ;

      ROOT::Math::EulerAngles rea ;
      rotr.GetRotation( rea ) ;
//      std::cout<<"Rear rotation = "<<rea<<std::endl;
      tVec[ it + 3 + 6*nC ] = rea.Phi() ;
      tVec[ it + 4 + 6*nC ] = rea.Theta() ;
      tVec[ it + 5 + 6*nC ] = rea.Psi() ;
   }
}
