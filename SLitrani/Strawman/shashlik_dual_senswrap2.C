//arguments:
//nphot : number of photons
//draw : turns SLitrani display on or off
//layer : chooses the layer in which to generate photons
//nlayers : sets the number of absorbing layers (there is one extra sensitive layer)
//so, for nlayers = 28 (default) there are sensitive layers with numbers 1 to 29

//the code outputs a few values related to photon efficiency into a root tree
//so examinations of multiple layers can be put together into a tree by using hadd on the generated files

//how to run:
//SLitrani 'shashlik_dual_senswrap.C(1e5,0,15)'

////////Original creator of this code: Kevin Pedro
//////Modified by Shilpi Jain

void shashlik_dual_senswrap2(int nphot, bool draw = false, unsigned layer = 1, unsigned nlayers = 28, double lyso_att_length = 100){
	if(layer > nlayers+1){
		cout << "You requested layer " << layer << ", but only " << nlayers+1 << "total layers" << endl;
		return;
	}
	else if(layer <= 0){
		cout << "There is no layer 0" << endl;
		return;
	}

	//bool debug = true;
	bool debug = false;
	
	/////SJ//////////////
	bool applyFCAL_param = true;
	//bool applyFCAL_param = false;

	////////reflector is on one side and the fibres also touch the base of the last layer and PD is on the other end
	string det_type = "onesided_refl_fiberTouchAtEnd"; 

	////////PD is on both the sides - DEFAULY by Kevin
	//string det_type = "bothsided";

	////////There is no reflector on the other side. PD is there on the other end
	//string det_type = "noreflector";

	////////There is  reflector on the other side and fibres DONOT touch the last layer, 
	//////instead they come out of the last layer (larger lenght) . PD is there on the other end
	//string det_type = "onesided_refl_fiberNotTouchAtEnd";
	
	/////END SJ//////////////
	
	int mediumindex = 1;
	
	//char *name = "shashlik";
	//char *listing = "shashlik";
	gROOT->ProcessLine(".L InitSLitrani.C");
	InitSLitrani(5,"shashlik","shashlik");
	TGeoManager *geom = new TGeoManager("setup","shashlik");
	
	//--------------
	//Materials List
	
	//Vacuum
	TGeoMaterial *vacuum_mat = new TGeoMaterial("Vacuum",0,0,0);
	TLitMedium *vacuum = new TLitMedium("Vacuum",mediumindex++,vacuum_mat,kFALSE,1.0,1e+4);
	vacuum->IsIsotropic(1.0);
	
	//Reflective Wrapping
	TGeoMaterial *wrap_mat = new TGeoMaterial("Wrap",0,0,0);
	TLitMedium *refwrap = new TLitMedium("RefWrap",mediumindex++,wrap_mat,-1.0);
	refwrap->SetAsWrapping(0.0,0.0,1.0);
	refwrap->IsIsotropic(1.0);
	
	//Diffusive Wrapping
	TLitMedium *difwrap = new TLitMedium("DifWrap",mediumindex++,wrap_mat,-1.0);
	difwrap->SetAsWrapping(1.0,0.0,1.0);
	difwrap->IsIsotropic(1.0);
	
	//Scintillator
	//define ceramic scintillator LYSO:Ce (Lu2(1-x)Y2xSiO5)(Ce) (x=0.1) 7.40 g/cm3
	TGeoElementTable *table = geom->GetElementTable();
	TGeoElement *Lu = table->FindElement("LUTETIUM");
	TGeoElement *Y = table->FindElement("YTTRIUM");
	TGeoElement *Si = table->FindElement("SILICON");
	TGeoElement *O = table->FindElement("OXYGEN");
	TGeoElement *Ce = table->FindElement("CERIUM");
	TGeoMixture *LYSO_mix = new TGeoMixture("LYSO",5,7.40);
	LYSO_mix->AddElement(Lu,0.7144);
	LYSO_mix->AddElement(Y,0.0403);
	LYSO_mix->AddElement(Si,0.0637);
	LYSO_mix->AddElement(O,0.1814);
	LYSO_mix->AddElement(Ce,0.0002);
	//rad len, int len (neg. rad len to prevent recomputation) in cm
	LYSO_mix->SetRadLen(-1.14,20.9);
	//define Litrani medium
	TLitMedium *LYSO = new TLitMedium("LYSO",mediumindex++,LYSO_mix,kFALSE,1.0,lyso_att_length);
	LYSO->SetCascades(2.07);
	//Sellmeier values from CERN-THESIS-2008-043, p. 92
	const Double_t ns0 = 2.464;   //index 0 for the Sellmeier law for LYSO
	const Double_t ns1 = 0.73098;   //index 1 for the Sellmeier law for LYSO
	const Double_t ls1 = 204.34;   //1st resonant wavelength for the Sellmeier law for LYSO
	LYSO->IsIsotropic(1,ns0,ns1,ls1);
	//LYSO->FindSpectrum("Spectrum_PbWO4La"); //no idea what to do for this...
	LYSO->SetDiffusionLength(60.,true); //copied from PbWO4...
	
	//WLS fiber
	double wls_spect_factor = 1.0;
	double wls_att_length = 1000.0;
	double wls_index = 1.59;
	double emission_wl = 420;
	TGeoMaterial *wls_mat = new TGeoMaterial("wls",0,0,0); //A,Z,rho irrelevant
	TLitMedium *wls = new TLitMedium("wls",mediumindex++,wls_mat,kFALSE,1.0,wls_att_length);

	if(debug) cout<<"Shilpi:1.  WLS attenuation length:"<<wls->GetWLSL()<<endl;

	wls->IsIsotropic(wls_index);

	if(debug) cout<<"Shilpi:2.  WLS attenuation length:"<<wls->GetWLSL()<<endl;

	gROOT->ProcessLine(".L BCF91A_Emission.C");
	gROOT->ProcessLine(".L BCF91A_Abs.C");
	BCF91A_Abs(wls_spect_factor);
	//BCF91A_Abs(wls_spect_factor, kTRUE);

	if(debug) cout<<"Shilpi:3.  WLS attenuation length:"<<wls->GetWLSL()<<endl;

	wls->SetWLS(BCF91A_Emission(),"AbsLengthWLS_Abs");

	
	if(debug) cout<<"Shilpi:4.  WLS attenuation length:"<<wls->GetWLSL()<<endl;

	//Total absorber
	TGeoMaterial *totabs_mat = new TGeoMaterial("totabs",0,0,0);
	//negative value of mu (4th argument in constructor): no slice between wrapping
	//only allowed if it is a total absorber, or if the wrapped medium is isotropic
	TLitMedium *totabs = new TLitMedium("totabs",mediumindex++,totabs_mat,-1.0);
	totabs->SetAsWrapping(0.0,0.0,1.0,1.0,1.0);
	totabs->IsIsotropic(1.0);  // Only useful if last arg of constructor changed to +1.0
	
	//Lead absorber
	TGeoElement *Pb = table->FindElement("LEAD");
	TGeoMaterial *leadabs_mat = new TGeoMaterial("leadabs",Pb,11.4);
	//TLitMedium *leadabs = new TLitMedium("leadabs",mediumindex++,leadabs_mat,kFALSE,1.0,0); //0.0001
	//leadabs->IsIsotropic(2.6);
	TLitMedium *leadabs = new TLitMedium("leadabs",mediumindex++,leadabs_mat,1.0);
	leadabs->SetAsWrapping(0.0,0.0,1.0,1.0,1.0);
	leadabs->IsIsotropic(2.6);
	
	//Detector
	TGeoMaterial *wdetector_mat = new TGeoMaterial("wdetector",0,0,0);
	TLitMedium *wdetector = new TLitMedium("wdetector",mediumindex++,wdetector_mat,kFALSE,1.0,1e+7);
	wdetector->IsIsotropic(wls_index); //perfect match to WLS

	//--------------
	//Geometry Setup
	
	//lengths (in cm)
	double WorldXY = 50;
	double WorldZ = 100;
	double WrapThick = 0.1;
	double DetThick = 0.1;
	
	double CalorSizeXY = 1.9;
	double SensThick = 0.2; //thickness of LYSO tiles
	double AbsThick = 0.4; //thickness of absorber
	double CalorThick = AbsThick*nlayers + (SensThick+WrapThick)*(nlayers+1);
	
	double WLSRadius = 0.05; //radius of WLS fibers (1mm diameter)
	double WLSGap = 0.01;
	double WLSLength = 25;
	
	if(det_type == "onesided_refl_fiberTouchAtEnd")
	  WLSLength = 22.45;

	
	double WLSFrac = 0.75; //placement of WLS fibers, value must be in (0,1)
	

	///////SJ/////////////////
	if( applyFCAL_param ){
	  CalorSizeXY = 2.5;
	  SensThick = 0.2; //thickness of LYSO tiles
	  AbsThick = 0.4; //thickness of absorber
	  CalorThick = AbsThick*nlayers + (SensThick+WrapThick)*(nlayers+1);
	  
	  ////actual is 0.065 - leave it from the time being
	  WLSRadius = 0.05; //radius of WLS fibers (1mm diameter)
	  WLSGap = 0.01;
	  WLSFrac = 0.5; //placement of WLS fibers, value must be in (0,1)
	}
	///////END SJ/////////////////

	if (CalorThick > WLSLength){
		cout << "The calorimeter is too large. The maximum number of layers is " << (int)((WLSLength-SensThick-WrapThick)/(AbsThick+SensThick+WrapThick)) << endl;
		return;
	}
	
	//build world
	TGeoVolume *world = geom->MakeBox("WORLD",vacuum,WorldXY/2,WorldXY/2,WorldZ/2);
	geom->SetTopVolume(world);
	
	//build total absorbing wrap around world
	TGeoBBox *worldwrap_out = new TGeoBBox("WrapO",WorldXY/2,WorldXY/2,WorldZ/2);
	TGeoBBox *worldwrap_in  = new TGeoBBox("WrapI",WorldXY/2-WrapThick,WorldXY/2-WrapThick,WorldZ/2-WrapThick);
	TGeoCompositeShape *worldwrap_shape = new TGeoCompositeShape("WorldWrap","WrapO-WrapI");
	TGeoVolume *worldwrap = new TGeoVolume("WorldWrap",worldwrap_shape,totabs);
	world->AddNode(worldwrap,1,0);
	
	//build fiber shapes
	TGeoTube *fiber_shape = new TGeoTube("FiberTube",0,WLSRadius,WLSLength/2);
	//shapes for subtracting from tiles have air gaps
	TGeoTube *fiber_gap_shape = new TGeoTube("FiberGapTube",0,WLSRadius+WLSGap,WLSLength/2);
	TGeoTube *fiber_shape_sens = new TGeoTube("FiberTubeSens",0,WLSRadius+WLSGap,SensThick/2);
	TGeoTube *fiber_shape_sens_wrap = new TGeoTube("FiberTubeSensWrap",0,WLSRadius+WLSGap,(SensThick+WrapThick)/2);
	TGeoTube *fiber_shape_abs = new TGeoTube("FiberTubeAbs",0,WLSRadius+WLSGap,AbsThick/2);
	
	//some translations for fibers
	/////////////////////SJ///////////////////////////
	TGeoTranslation *tf[5];

	double xdisp_refl[4];
	double ydisp_refl[4];
	
	double xydispl_refl = 0.;

	xydispl_refl = CalorSizeXY*WLSFrac/2;
	
	///(x,y) == (00, 11, 22, 33) == (++, +-, --, -+) -> x is -tive for 2 and 3; y is -tive for 1 and 2
	for(int i=0; i<4; i++){
	  
	  int xsign = +1;
	  int ysign = +1;
	  
	  if(i==2 || i==3) xsign = -1;
	  
	  if(i==1 || i==2) ysign = -1;
	  
	  xdisp_refl[i] =  xsign*xydispl_refl;
	  ydisp_refl[i] =  ysign*xydispl_refl;
	  
	};

	
	

	////////monitoring
	///////for the case of both sided or one sided with fibre coming out from the other end
	double zdisp_fiber = -CalorThick/2;
	
	if( det_type == "onesided_refl_fiberTouchAtEnd")
	  zdisp_fiber = -(CalorThick - WLSLength/2);
	  //zdisp_fiber = WLSLength/2;
	
	if( det_type == "onesided_refl_fiberNotTouchAtEnd" || det_type == "bothsided" || det_type == "noreflector")
	  zdisp_fiber = -CalorThick/2;
	
	
	tf[0] = new TGeoTranslation("tf0",0,0,zdisp_fiber);
	
	for(int it=1; it<=4; it++){
	  
	  char cit[100];
	  sprintf(cit,"%d",it);
	  string sit(cit);
	  tf[it] = new TGeoTranslation( ("tf"+sit).c_str(),xdisp_refl[it-1],ydisp_refl[it-1],zdisp_fiber);
	}

	//////////////////////SJ///////////////////////////

	//build fibers
	TGeoVolume *wls_fiber_gap = new TGeoVolume("WLSFiberGap",fiber_gap_shape,vacuum);
	TGeoVolume *wls_fiber = new TGeoVolume("WLSFiber",fiber_shape,wls);
	wls_fiber_gap->AddNode(wls_fiber,1,0); 
	
	for(int it=1; it<=4; it++){
	  
	  world->AddNode(wls_fiber_gap,it,tf[it]);
	}
	
	TLitVolume* wls_gap_lit = new TLitVolume(wls_fiber_gap);
	TLitVolume* wls_lit = new TLitVolume(wls_fiber);
	
	
	TGeoTranslation *tft[5];
	
	double zdisp_fiber_abs = 0;
	
	if( det_type == "onesided_refl_fiberTouchAtEnd")
	  zdisp_fiber_abs = (WLSLength-CalorThick)/2;
	
	if( det_type == "onesided_refl_fiberNotTouchAtEnd" || det_type == "bothsided" || det_type == "noreflector")
	  zdisp_fiber_abs = 0;
	
	tft[0] = new TGeoTranslation("tft0",0,0,zdisp_fiber_abs);
	tft[0]->RegisterYourself();
	

	double fiber_abs_halflength = (WLSLength-CalorThick)/4;
	
	if( det_type == "onesided_refl_fiberTouchAtEnd")
	  fiber_abs_halflength = zdisp_fiber_abs;
	
	if( det_type == "onesided_refl_fiberNotTouchAtEnd" || det_type == "bothsided"|| det_type == "noreflector" )
	  fiber_abs_halflength = (WLSLength-CalorThick)/4;
	
	for(int it=1; it<=4; it++){
	  
	  char cit[100];
	  sprintf(cit,"%d",it);
	  string sit(cit);
	  tft[it] = new TGeoTranslation( ("tft"+sit).c_str(),xdisp_refl[it-1],ydisp_refl[it-1],zdisp_fiber_abs);
	  tft[it]->RegisterYourself();
	}

	//////////////////////////SJ//////////////////////////////

	//absorbing wrapping for fibers outside of calorimeter
	
	////////////////////////////////SJ/////////////////
	TGeoTube *wls_fiber_abs_tube = new TGeoTube("WLSFiberAbsTube",WLSRadius+WLSGap,WLSRadius+WLSGap+WrapThick,fiber_abs_halflength);
	////////////////////////////////SJ/////////////////
	
	TGeoTranslation *tfa[2];
	TGeoCompositeShape *wls_fiber_abs_shape;
	////used only for both sided or one sided with reflector with fibres not touching the base of the detector
	if( det_type == "onesided_refl_fiberNotTouchAtEnd" || det_type == "bothsided" || det_type == "noreflector" ){
	  tfa[0] = new TGeoTranslation("tfa0",0,0,(WLSLength-CalorThick)/4);
	  tfa[0]->RegisterYourself();
	  tfa[1] = new TGeoTranslation("tfa1",0,0,-CalorThick-(WLSLength-CalorThick)/4);
	  tfa[1]->RegisterYourself();
	  wls_fiber_abs_shape = new TGeoCompositeShape("WLSFiberAbsShape","WLSFiberAbsTube:tfa0 + WLSFiberAbsTube:tfa1");
	}


	
	TGeoVolume* wls_fiber_abs;

	if( det_type == "onesided_refl_fiberTouchAtEnd")
	  wls_fiber_abs = new TGeoVolume("WLSFiberAbs",wls_fiber_abs_tube,leadabs);
	
	if( det_type == "onesided_refl_fiberNotTouchAtEnd" || det_type == "bothsided" || det_type == "noreflector")
	  wls_fiber_abs = new TGeoVolume("WLSFiberAbs",wls_fiber_abs_shape,leadabs);
	


	for(int it=1; it<=4; it++){
	  world->AddNode(wls_fiber_abs,it,tft[it]);
	}
	
	//////////////////////////////////////SJ - 17th Feb//////////////////////////////////////////////////////
	/////////////////////////////Reflectors on one side///////////////////////////////////////////////
	TGeoTube *refl_wrap_tube = new TGeoTube("reflWrapTube",0.,WLSRadius+WLSGap,WrapThick/2);
	TGeoVolume* refl_wrap_layer = new TGeoVolume("reflWrapLayer",refl_wrap_tube,refwrap);
	
	

	double zdisp_refl = -CalorThick/2.-WLSLength/2-WrapThick/2;

	if( det_type == "onesided_refl_fiberTouchAtEnd")
	  zdisp_refl = -CalorThick-WrapThick/2;
	    
	/////absent for the double sided PD
	if( det_type == "onesided_refl_fiberNotTouchAtEnd")
	  zdisp_refl = -CalorThick/2.-WLSLength/2-WrapThick/2;

	
	
	TGeoTranslation *tref[4];
	
	/////absent for the double sided PD
	if( det_type == "onesided_refl_fiberTouchAtEnd" || det_type == "onesided_refl_fiberNotTouchAtEnd" ){
	  for(int it=0; it<4; it++)
	    {
	      char cit[100];
	      sprintf(cit,"%d",it);
	      string sit(cit);
	      tref[it] = new TGeoTranslation( ("tfref"+sit).c_str(),xdisp_refl[it], ydisp_refl[it], zdisp_refl);
	    
	      world->AddNode(refl_wrap_layer,it+1,tref[it]); //sens layer added inside sens wrap
	    }
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	TGeoTranslation *tft_tiles[5];
	
	
	tft_tiles[0] = new TGeoTranslation("tft0",0,0,0);
	tft_tiles[0]->RegisterYourself();
	

	for(int it=1; it<=4; it++){
	  
	  char cit[100];
	  sprintf(cit,"%d",it);
	  string sit(cit);
	  tft_tiles[it] = new TGeoTranslation( ("tft_tiles"+sit).c_str(),xdisp_refl[it-1],ydisp_refl[it-1],0);
	  tft_tiles[it]->RegisterYourself();
	}

	//build scintillator tile shape
	TGeoBBox *sens_tile = new TGeoBBox("SensTile",CalorSizeXY/2,CalorSizeXY/2,SensThick/2);
	//TGeoCompositeShape *sens_shape = new TGeoCompositeShape("SensShape","SensTile-(FiberTubeSens:tft1+FiberTubeSens:tft2+FiberTubeSens:tft3+FiberTubeSens:tft4)");	

	TGeoCompositeShape *sens_shape = new TGeoCompositeShape("SensShape","SensTile-(FiberTubeSens:tft_tiles1+FiberTubeSens:tft_tiles2+FiberTubeSens:tft_tiles3+FiberTubeSens:tft_tiles4)");	
	TGeoVolume* sens_layer = new TGeoVolume("SensLayer",sens_shape,LYSO);
	TLitVolume* sens_lit = new TLitVolume(sens_layer);
	double emission_x = 0;
	TVector3 emisdir(0.0,0.0,1.0);
	TVector3 emispoint(emission_x,0,0.0);
	sens_lit->SetEmission(flat,180.0,emisdir,"",true,emispoint);

	//build scintillator wrapping
	TGeoBBox *sens_wrap_box = new TGeoBBox("SensWrapBox",CalorSizeXY/2+WrapThick/2,CalorSizeXY/2+WrapThick/2,SensThick/2+WrapThick/2);
	//TGeoCompositeShape *sens_wrap_shape = new TGeoCompositeShape("SensWrapShape","SensWrapBox-(FiberTubeSensWrap:tft1+FiberTubeSensWrap:tft2+FiberTubeSensWrap:tft3+FiberTubeSensWrap:tft4)");	
	TGeoCompositeShape *sens_wrap_shape = new TGeoCompositeShape("SensWrapShape","SensWrapBox-(FiberTubeSensWrap:tft_tiles1+FiberTubeSensWrap:tft_tiles2+FiberTubeSensWrap:tft_tiles3+FiberTubeSensWrap:tft_tiles4)");	
	TGeoVolume* sens_wrap_layer = new TGeoVolume("SensWrapLayer",sens_wrap_shape,difwrap);
	sens_wrap_layer->AddNode(sens_layer,1,0); //sens layer added inside sens wrap
	
	//build absorber tile shape
	TGeoBBox *abs_tile = new TGeoBBox("AbsTile",CalorSizeXY/2,CalorSizeXY/2,AbsThick/2);
	//TGeoCompositeShape *abs_shape = new TGeoCompositeShape("AbsShape","AbsTile-(FiberTubeAbs:tft1+FiberTubeAbs:tft2+FiberTubeAbs:tft3+FiberTubeAbs:tft4)");
	TGeoCompositeShape *abs_shape = new TGeoCompositeShape("AbsShape","AbsTile-(FiberTubeAbs:tft_tiles1+FiberTubeAbs:tft_tiles2+FiberTubeAbs:tft_tiles3+FiberTubeAbs:tft_tiles4)");
	TGeoVolume* abs_layer = new TGeoVolume("AbsLayer",abs_shape,leadabs);
	
	//build calorimeter
	TGeoTranslation** ts = new TGeoTranslation*[nlayers+1]; //one extra sens layer
	TGeoTranslation** ta = new TGeoTranslation*[nlayers];
	double* SensOffset = new double[nlayers+1];
	double* AbsOffset = new double[nlayers];
	for(unsigned n = 0; n <= nlayers; n++){
		//add wrapped scintillator tile
		SensOffset[n] = -(((SensThick+WrapThick)+AbsThick)*n + (SensThick+WrapThick)/2);
		std::stringstream tsname;
		tsname << "ts" << n;
		ts[n] = new TGeoTranslation((tsname.str()).c_str(),0,0,SensOffset[n]);

		
		world->AddNode(sens_wrap_layer,nlayers-n+1,ts[n]);
		

		///////try to add a reflective wrapping on the last layer
		

		if(n==nlayers) break; //no absorber after last scintillator
		
		//add absorber tile
		AbsOffset[n] = -(((SensThick+WrapThick)+AbsThick)*n + (SensThick+WrapThick) + AbsThick/2);
		std::stringstream taname;
		taname << "ta" << n;
		ta[n] = new TGeoTranslation((taname.str()).c_str(),0,0,AbsOffset[n]);
		world->AddNode(abs_layer,nlayers-n,ta[n]);
	}

	
	
	//build detector
	double DetOffset[2];
	TGeoVolume *det = geom->MakeBox("Detector",wdetector,CalorSizeXY/2,CalorSizeXY/2,DetThick/2);
	if( det_type == "onesided_refl_fiberTouchAtEnd")
	  DetOffset[0] = WLSLength-CalorThick+DetThick/2;
	    
	/////absent for the double sided PD
	if( det_type == "onesided_refl_fiberNotTouchAtEnd" || det_type == "bothsided"|| det_type == "noreflector" )
	  DetOffset[0] = WLSLength/2-CalorThick/2+DetThick/2;
	
	
	TGeoTranslation *td = new TGeoTranslation("td",0,0,DetOffset[0]);
	TLitVolume *det_lit = new TLitVolume(det);
	det_lit->SetDetector(true,"none",91.0); //angle > 90 -> accept all incident photons
	world->AddNode(det,1,td);
	
	DetOffset[1] = -WLSLength/2-CalorThick/2-DetThick/2;
	TGeoTranslation *td2 = new TGeoTranslation("td2",0,0,DetOffset[1]);
	TGeoRotation *rd = new TGeoRotation("rd",0,180,0); //flip around x-axis
	TGeoCombiTrans *cd = new TGeoCombiTrans(*td2,*rd);
	
	if( det_type == "bothsided"|| det_type == "noreflector" )
	  world->AddNode(det,2,cd); //second detector at other end of WLS
	
	//build detector wrap (absorber)
	TGeoBBox *detwrap_out = new TGeoBBox("DetWrapO",CalorSizeXY/2+WrapThick,CalorSizeXY/2+WrapThick,DetThick/2+WrapThick/2);
	TGeoBBox *detwrap_in  = new TGeoBBox("DetWrapI",CalorSizeXY/2,CalorSizeXY/2,DetThick/2);
	TGeoTranslation *tdw = new TGeoTranslation("tdw",0,0,WrapThick/2);
	tdw->RegisterYourself();
	TGeoCompositeShape *detwrap_shape = new TGeoCompositeShape("DetWrap","DetWrapO:tdw-DetWrapI");
	TGeoVolume *detwrap = new TGeoVolume("DetWrap",detwrap_shape,totabs);  
	world->AddNode(detwrap,1,td);
	
	if( det_type == "bothsided" || det_type == "noreflector" )
	  world->AddNode(detwrap,2,cd);
	
	//final checks
	geom->CloseGeometry();
	geom->CheckOverlaps(0.01);
	
	//----------------
	//Display Settings
	
	world->SetVisibility(kFALSE);
	world->SetVisContainers();
	world->SetLineColor(1);
	world->SetLineWidth(1);
	
	worldwrap->SetVisibility(kFALSE);
	worldwrap->SetLineColor(4);
	worldwrap->SetLineWidth(1);
	
	wls_fiber->SetVisibility(kTRUE);
	wls_fiber->SetLineColor(3);
	wls_fiber->SetLineWidth(1);

	sens_layer->SetVisibility(kTRUE);
	sens_layer->SetLineColor(7);
	sens_layer->SetLineWidth(1);

	sens_wrap_layer->SetVisibility(kTRUE);
	sens_wrap_layer->SetLineColor(4);
	//sens_wrap_layer->SetLineColor(2);
	sens_wrap_layer->SetLineWidth(1);
	sens_wrap_layer->SetLineStyle(2);	
	
	abs_layer->SetVisibility(kTRUE);
	abs_layer->SetLineColor(12);
	abs_layer->SetLineWidth(1);
	
	det->SetVisibility(kTRUE);
	det->SetLineColor(6);
	det->SetLineWidth(1);
	
	detwrap->SetVisibility(kTRUE);
	detwrap->SetLineColor(12);
	detwrap->SetLineWidth(1);
	detwrap->SetLineStyle(2);
	
	wls_fiber_abs->SetVisibility(kTRUE);
	wls_fiber_abs->SetLineColor(12);
	wls_fiber_abs->SetLineWidth(1);
	wls_fiber_abs->SetLineStyle(3);
	
	if( det_type == "onesided_refl_fiberTouchAtEnd" || det_type == "onesided_refl_fiberNotTouchAtEnd" ){
	  refl_wrap_layer->SetVisibility(kTRUE);
	  refl_wrap_layer->SetLineColor(2);
	  refl_wrap_layer->SetFillColor(2);
	  refl_wrap_layer->SetLineWidth(10);
	}

	wls_fiber_gap->SetVisibility(kFALSE);
	
	////Shilpi - turned off
	world->Draw();
	/////END SHILPI
	
	//------------
	//Run Settings
	
	//kills run if this many "killthis" error messages are given
	gCleanOut->fMaxKill = 10000;
	
	//generates photons
	std::stringstream spname;
	spname << "/WORLD_1/SensWrapLayer_" << layer << "/SensLayer_1";
	TLitSpontan *sp = new TLitSpontan("spontan","spontan",(spname.str()).c_str(),emission_wl);
	TLitPhoton::fgNPointsMax = 50000; //max # of interactions on photon trajectory
	TLitPhys::Get()->SetLate(100);
	
	if(draw){
		TLit::Get()->BookCanvas(5);
		gTwoPad->SetStateOfPads(Pad1Only);
		world->Draw();
		sp->TrackToDraw(1);
	}
	
	sp->Gen(1,nphot);
	
	//retrieve detector
	TLitDetector *active_det = TLitDetector::FindDetector("G/WORLD_1/Detector_1",true);
	int nseen = (active_det) ? active_det->fNpSeen : 0;
	TLitDetector *active_det2 = TLitDetector::FindDetector("G/WORLD_1/Detector_2",true);

	if( det_type == "bothsided" ){
	  nseen += (active_det2) ? active_det2->fNpSeen : 0;
	}
	
	int nlost = gLitCs->fNpTooLate;
	cout << "Photons seen: " << nseen << endl;
	cout << "Photons emitted: " << nphot << endl;
	cout << "Photons lost: " << nlost << endl;
	///Shilpi
	cout<< "photons efficiecy:"<< (double)nseen*100/(double)nphot << endl;
	///ENDShilpi

	if(draw){
		gLitGp->Summary();
		gTwoPad->SetStateOfPads(Pad1AndPad2);
		gTwoPad->ChangePad();
		gLitGs->DoStat();
		TLit::Get()->CloseFiles();
	}
	
	//-----------
	//ROOT output
	
	//output file
	stringstream oname;
	oname << "tree_shashlik_dual_senswrap_phot" << nphot << "_att" << lyso_att_length << "_nlay" << nlayers << "_lay" << layer << ".root";
	int s_nphot, s_nseen, s_nlost, s_layer, s_nlayers;
	double s_z_layer, s_z_det1, s_z_det2, s_lyso_att_length;
	TFile* out_file = new TFile((oname.str()).c_str(),"RECREATE");
	
	//setup tree
	TTree* s_tree = new TTree("tree","photon efficiency");
	s_tree->Branch("nphot",&s_nphot,"s_nphot/I");
	s_tree->Branch("nseen",&s_nseen,"s_nseen/I");
	s_tree->Branch("nlost",&s_nlost,"s_nlost/I");
	s_tree->Branch("layer",&s_layer,"s_layer/I");
	s_tree->Branch("nlayers",&s_nlayers,"s_nlayers/I");
	s_tree->Branch("z_layer",&s_z_layer,"s_z_layer/D");
	s_tree->Branch("z_det1",&s_z_det1,"s_z_det1/D");
	s_tree->Branch("z_det2",&s_z_det2,"s_z_det2/D");
	s_tree->Branch("lyso_att_length",&s_lyso_att_length,"s_lyso_att_length/D");
	
	//fill tree
	s_nphot = nphot;
	s_nseen = nseen;
	s_nlost = nlost;
	s_layer = layer;
	s_nlayers = nlayers;
	s_z_layer = SensOffset[nlayers-layer+1];
	s_z_det1 = DetOffset[0];
	s_z_det2 = DetOffset[1];
	s_lyso_att_length = lyso_att_length;
	s_tree->Fill();
	
	//write output
	s_tree->Write();
	out_file->Close();
	

	cout<<"Shilpi END: WLS attenuation length:"<<wls-> GetWLSL()<<endl;
}
