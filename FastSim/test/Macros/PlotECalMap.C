{
  TGraph2D* Map1 = new TGraph2D(100);
  TGraph2D* Map2 = new TGraph2D(100);

  double x, y , z, eta, phi, E;
 string s;
 int iSide;

ifstream flux;
flux.open("../data/EventEcal_SplitIn2_100GeV.txt");

 if (!flux)   {
   cout << "Cannot open file.\n"; 
   return 1; 
 } 

 int i = -1, j = -1;

while(flux)
{

  flux >> s >> s >> E >> s >> s >> x >> s >> s >> y >> s >> s >> phi >> s >> s >> eta >> s >> s >> iSide >> s >> s >> z;
  


 if (iSide == 1) {Map1->SetPoint(i, x, y, E);
   i++;
   cout << "i = " << i << " E = " << E << " x = " << x << " y = " << y << " iSide = " << iSide << endl;
 }
  if (iSide == 2) {
    j++;
    Map2->SetPoint(j, x, y, E);
    cout << "j = " << j << " E = " << E << " x = " << x << " y = " << y << " iSide = " << iSide << endl;

  }
 }
 flux.close();

 TH2D* PLOTTER = new TH2D("PLOTTER", "", 1, -80, -50, 1, -10, 10);

 TCanvas* c1 = new TCanvas();
 c1->cd();

 PLOTTER->Draw("P0");
 PLOTTER->SetStats(0);
 Map1->Draw("trilP0");

 TCanvas* c2 = new TCanvas();
 c2->cd();

 PLOTTER->Draw("P0");

 Map2->Draw("trilP0");

}
