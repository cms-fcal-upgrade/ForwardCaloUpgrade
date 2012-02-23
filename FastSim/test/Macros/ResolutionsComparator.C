#include <iostream>
#include <fstream>

TObjArray* aMu = new TObjArray(6);
TObjArray* aSigma = new TObjArray(6);

void ResolutionsComparator(){

  double* Mass = new double[7];
  double* mu = new double[7];
  double* err_mu = new double[7];
  double* sigma = new double[7];
  double* err_sigma = new double[7];
  double err_mass[7] = {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};



  for (int j = 0; j < 6; j++){

	string sIn("");

	switch(j){
	case 0: sIn = "ELECTRON_FORWARD/calorimeter_histograms_LSO_FEB2012_PERFECT_LIGHT_COLLECTION_ETA23_ELECTRON_FORWARD5x5.txt"; break;
	case 1: sIn = "ELECTRON_FORWARD/calorimeter_histograms_LSO_FEB2012_REALISTIC_LIGHT_COLLECTION_ETA23_ELECTRON_FORWARD5x5.txt"; break;
	case 2: sIn = "ELECTRON_FORWARD/LSO_FEB2012_FULLSIM_PERFECT_LIGHT_COLLECTION_ELECTRON.txt"; break;
	case 3: sIn = "ELECTRON_FORWARD/calorimeter_histograms_PBWO_FEB2012_PERFECT_LIGHT_COLLECTION_ETA23_ELECTRON_FORWARD5x5.txt"; break;
	case 4: sIn = "ELECTRON_FORWARD/calorimeter_histograms_PBWO_FEB2012_REALISTIC_LIGHT_COLLECTION_ETA23_ELECTRON_FORWARD5x5.txt"; break;
	case 5: sIn = "ELECTRON_FORWARD/PBWO_FEB2012_FULLSIM_PERFECT_LIGHT_COLLECTION_ELECTRON.txt"; break;

	default: break;
	}

  	ifstream myfile (sIn.c_str());

	cout << sIn.c_str() << endl;

	for (int i = 0; i < 7; i++) {
	  myfile >> Mass[i] >> mu[i] >> err_mu[i] >> sigma[i] >> err_sigma[i];
	  //	  mu[i] = mu[i]/Mass[i]; err_mu[i] = err_mu[i]/Mass[i];
	  cout <<  Mass[i] << " " <<  mu[i] << " " << err_mu[i] << " " << sigma[i] << " " << err_sigma[i] << endl;
	  mu[i] = mu[i]/Mass[i]; err_mu[i] = err_mu[i]/Mass[i];
	  cout << mu[i] << " " << err_mu[i] << endl;
	}

  	myfile.close();

  	TGraphErrors* gMu = new TGraphErrors(7, Mass, mu, err_mass, err_mu);
  	TGraphErrors* gSigma = new TGraphErrors(7, Mass, sigma, err_sigma, err_mu);

	gMu->SetMarkerSize(1);
	gMu->SetMarkerColor(j%3+1);
	gMu->SetMarkerStyle(20+j);
	gMu->SetLineColor(j%3+1);
	//	gMu->Draw("AP");
	gSigma->SetMarkerSize(1);
	gSigma->SetMarkerColor(j%3+1);
	gSigma->SetMarkerStyle(20+j);
	gSigma->SetLineColor(j%3+1);

	aMu->AddAt(gMu, j);
	aSigma->AddAt(gSigma, j);
	
  }

  //  TGraphErrors* gSigma = (TGraphErrors*) aSigma->At(0);
  //  gSigma->Draw("AP");

  TCanvas* C = new TCanvas("C", "canvas", 1000., 500.);
  C->Divide(2,1);  

  for (int iDet = 0; iDet < 2; iDet++){

    C->cd(1);
  
    TH1F *vFrame = gPad->DrawFrame(5.0,0.85,700.,1.1);
    vFrame->SetTitle("Average reconstructed energy in ECAL");
    vFrame->SetTitleSize(0.2);
    vFrame->SetXTitle("Electron Energy (GeV)");
    vFrame->GetYaxis()->SetLabelSize(0.03);
    vFrame->GetYaxis()->SetTitleSize(0.04);
    vFrame->GetYaxis()->SetTitleOffset(1.2);
    vFrame->GetXaxis()->SetLabelSize(0.03);
    vFrame->GetXaxis()->SetTitleSize(0.04);
    vFrame->GetXaxis()->SetTitleOffset(0.95);
    vFrame->SetYTitle("#mu / Ee");
  
    vFrame->Draw();

    gPad->SetLogx(1);

    for (int j = iDet*3; j < (iDet+1)*3; j++){
    
      TGraphErrors* gMu = (TGraphErrors*) aMu->At(j);
      if (j==0) gMu->DrawClone("SAMEP");
      else gMu->DrawClone("SAMEP");
    }
  
    C->cd(2);
  
    TH1F *vFrame2 = gPad->DrawFrame(5.0,0.0,700.,0.03);
    vFrame2->SetTitle("Resolution in ECAL (Cball fit)");
    vFrame2->SetTitleSize(0.2);
    vFrame2->SetXTitle("Electron Energy (GeV)");
    vFrame2->GetYaxis()->SetLabelSize(0.03);
    vFrame2->GetYaxis()->SetTitleSize(0.04);
    vFrame2->GetYaxis()->SetTitleOffset(1.2);
    vFrame2->GetXaxis()->SetLabelSize(0.03);
    vFrame2->GetXaxis()->SetTitleSize(0.04);
    vFrame2->GetXaxis()->SetTitleOffset(0.95);

    vFrame2->SetYTitle("#sigma/#mu");
  
    vFrame2->Draw();

    gPad->SetLogx(1);

    for (int j = iDet*3; j < (iDet+1)*3; j++){
    
      TGraphErrors* gSigma = (TGraphErrors*) aSigma->At(j);
      if (j==0) gSigma->DrawClone("SAMEP");
      else gSigma->DrawClone("SAMEP");

      TLegend* Legend = new TLegend(0.15, 0.6, 0.6., 0.8);

    }

    if (iDet == 0) {
      Legend->AddEntry((TGraphErrors*) aSigma->At(0 + 3*iDet), "LSO Fast Sim perfect light coll", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(1 + 3*iDet), "LSO Fast Sim realistic", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(2 + 3*iDet), "LSO Full Sim perfect light coll", "p");
    } else if (iDet == 1) {
      Legend->AddEntry((TGraphErrors*) aSigma->At(0 + 3*iDet), "PbWO Fast Sim perfect light coll", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(1 + 3*iDet), "PbWO Fast Sim realistic", "p");
      Legend->AddEntry((TGraphErrors*) aSigma->At(2 + 3*iDet), "PbWO Full Sim perfect light coll", "p");
    }
    

   
    Legend->Draw();

    if (iDet == 0) C->SaveAs("ELECTRON_FORWARD/LSO_FEB2012_ELECTRON_ENERGY_RESOLUTION.png");
    if (iDet == 1) C->SaveAs("ELECTRON_FORWARD/PBWO_FEB2012_ELECTRON_ENERGY_RESOLUTION.png");
  }

    C->cd(1);
  
    TH1F *vFrame = gPad->DrawFrame(5.0,0.85,700.,1.1);
    vFrame->SetTitle("Average reconstructed energy in ECAL");
    vFrame->SetTitleSize(0.2);
    vFrame->SetXTitle("Electron Energy (GeV)");
    vFrame->GetYaxis()->SetLabelSize(0.03);
    vFrame->GetYaxis()->SetTitleSize(0.04);
    vFrame->GetYaxis()->SetTitleOffset(1.2);
    vFrame->GetXaxis()->SetLabelSize(0.03);
    vFrame->GetXaxis()->SetTitleSize(0.04);
    vFrame->GetXaxis()->SetTitleOffset(0.95);
    vFrame->SetYTitle("#mu / Ee");
  
    vFrame->Draw();

    gPad->SetLogx(1);

    for (int j = 0; j < 6; j++){
    
      TGraphErrors* gMu = (TGraphErrors*) aMu->At(j);
      if (j==0) gMu->DrawClone("SAMEP");
      else gMu->DrawClone("SAMEP");
    }
  
    C->cd(2);
  
    TH1F *vFrame2 = gPad->DrawFrame(5.0,0.0,700.,0.03);
    vFrame2->SetTitle("Resolution in ECAL (Cball fit)");
    vFrame2->SetTitleSize(0.2);
    vFrame2->SetXTitle("Electron Energy (GeV)");
    vFrame2->GetYaxis()->SetLabelSize(0.03);
    vFrame2->GetYaxis()->SetTitleSize(0.04);
    vFrame2->GetYaxis()->SetTitleOffset(1.2);
    vFrame2->GetXaxis()->SetLabelSize(0.03);
    vFrame2->GetXaxis()->SetTitleSize(0.04);
    vFrame2->GetXaxis()->SetTitleOffset(0.95);

    vFrame2->SetYTitle("#sigma/#mu");
  
    vFrame2->Draw();

    gPad->SetLogx(1);

    for (int j = 0; j < 6; j++){
    
      TGraphErrors* gSigma = (TGraphErrors*) aSigma->At(j);
      if (j==0) gSigma->DrawClone("SAMEP");
      else gSigma->DrawClone("SAMEP");

      TLegend* Legend = new TLegend(0.15, 0.6, 0.6., 0.8);

    }

    Legend->AddEntry((TGraphErrors*) aSigma->At(0), "LSO Fast Sim perfect light coll", "p");
    Legend->AddEntry((TGraphErrors*) aSigma->At(1), "LSO Fast Sim realistic", "p");
    Legend->AddEntry((TGraphErrors*) aSigma->At(2), "LSO Full Sim perfect light coll", "p");
    Legend->AddEntry((TGraphErrors*) aSigma->At(0), "PbWO Fast Sim perfect light coll", "p");
    Legend->AddEntry((TGraphErrors*) aSigma->At(1), "PbWO Fast Sim realistic", "p");
    Legend->AddEntry((TGraphErrors*) aSigma->At(2), "PbWO Full Sim perfect light coll", "p");
   
    Legend->Draw();

    C->SaveAs("ELECTRON_FORWARD/LSO_PBWO_FEB2012_ELECTRON_ENERGY_RESOLUTION.png");
  

}
