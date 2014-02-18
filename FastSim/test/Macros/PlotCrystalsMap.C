{
  TGraph2D* Map1 = new TGraph2D(100);
  TGraph2D* Map2 = new TGraph2D(100);

  double x, y, x1, y1, x2, y2, x3, y3, x4, y4, dummy;
  string s;
  int n;

  double lowX =10000, lowY=10000, highX=-10000, highY=-10000;

  ifstream flux;
  flux.open("../data/EventWithMissingCrystals_1.txt");

 if (!flux)   {
   cout << "Cannot open file.\n"; 
   return 1; 
 } 

 int i = -1, j = -1;

 TObjArray* Squares = new TObjArray(100);
 TObjArray* Pts = new TObjArray(100);
 TObjArray* Text = new TObjArray(100);

while(flux)
{

  /*
  ic = 12 Entrance xp = (54.28,18.46,320.58)
 corners[0] (59.45,26.45,319.48)
 corners[1] (62.28,26.45,319.11)
 corners[2] (62.30,23.60,319.25)
 corners[3] (59.47,23.60,319.63)
  */



  flux >> s >> s >> n >> s >> s >> s >> x >> y;
  flux >> s >> x1 >> y1;
  flux >> s >> x2 >> y2;
  flux >> s >> x3 >> y3;
  flux >> s >> x4 >> y4;
  cout << "n = " << n << " x = " << x << " y = " << y << endl;
  cout << " x1 = " << x1 << " y1 = " << y1 << endl;
  cout << " x2 = " << x2 << " y2 = " << y2 << endl;
  cout << " x3 = " << x3 << " y3 = " << y3 << endl;
  cout << " x4 = " << x4 << " y4 = " << y4 << endl;
  
  if (lowX>x1) lowX = x1;
  if (lowX>x2) lowX = x2;
  if (lowX>x3) lowX = x3;
  if (lowX>x4) lowX = x4;

  if (highX<x1) highX = x1;
  if (highX<x2) highX = x2;
  if (highX<x3) highX = x3;
  if (highX<x4) highX = x4;

  if (lowY>y1) lowY = y1;
  if (lowY>y2) lowY = y2;
  if (lowY>y3) lowY = y3;
  if (lowY>y4) lowY = y4;

  if (highY<y1) highY = y1;
  if (highY<y2) highY = y2;
  if (highY<y3) highY = y3;
  if (highY<y4) highY = y4;


  Double_t x5[5] = {x1, x2, x3, x4, x1};
  Double_t y5[5] = {y1, y2, y3, y4, y1};

  double meanX = (x1+x2+x3+x4)/4;
  double meanY = (y1+y2+y3+y4)/4;

  TPolyLine* square = new TPolyLine(5, x5, y5);
  TEllipse* point = new TEllipse(x,y, 0.1);
  TText* T = new TText(meanX, meanY, Form("%d", n));
  point->SetFillColor(kRed);
  point->SetLineColor(kRed);

  Squares->AddAt(square, n);
  Pts->AddAt(point, n);
  Text->AddAt(T, n);

 } 

 TH2D* PLOTTER = new TH2D("PLOTTER", "", 1, lowX-1, highX+1, 1, lowY-1, highY+1);

 TCanvas* c1 = new TCanvas("c1", "ParticlePath", 1000., 600.);
 c1->Divide(2,1);
 c1->cd(1);

 PLOTTER->Draw("P0");
 PLOTTER->SetStats(0);

 for (int i = 0; i < 49; i++){

   TPolyLine* square =  (TPolyLine*) Squares->At(i);
   TEllipse* point =  (TEllipse*) Pts->At(i); 
   TText* T = (TText*) Text->At(i);
 
   square->Draw();
   point->Draw();
   T->Draw();

 }

 c1->cd(2);
 PLOTTER->Draw("P0");
 PLOTTER->SetStats(0);


 for (int i =49; i < n+1; i++){

   TPolyLine* square =  (TPolyLine*) Squares->At(i);
   TEllipse* point =  (TEllipse*) Pts->At(i); 
   TText* T = (TText*) Text->At(i);

   square->Draw();
   point->Draw();
   T->Draw();

 }

 c1->SaveAs("CrackEvent.png");


  /*
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
  */

}
