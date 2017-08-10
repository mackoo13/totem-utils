{
gROOT->Reset();

#include <string>
#include <iostream>
using namespace std;

// draw 2D histograms showing the events on ten different planes
//
// usage: the ROOT file to open should be in TFile file(...)
// then run it as a ROOT script
//
// > root
// > .x [path to the script]


string planeNames[10] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10"};

TFile file("test20000.root");
TTreeReader reader("Events", &file);
TTreeReaderArray<PSimHit> ps(reader, "PSimHits_g4SimHits_TotemHitsRP_TestFlatGun.obj");

TH2D** h = new TH2D*[10]; 

for(int i=0; i<10; ++i) {
	h[i] = new TH2D(planeNames[i].c_str(), planeNames[i].c_str(), 100, -20, 20, 100, -20, 20);
}

while (reader.Next()) {
	for (int i=0; i<ps.GetSize(); i++) {

		int arId = (ps[i].detUnitId()>> 24) & 0x1;
		int stId = (ps[i].detUnitId()>> 22) & 0x3;
		int rpId = (ps[i].detUnitId()>> 19) & 0x7;
		int plId = (ps[i].detUnitId()>> 15) & 0xF;

		if(arId!=0 || stId!=0 || rpId!=4) continue;

		double xVal = ps[i].entryPoint().x();
		double yVal = ps[i].entryPoint().y();
		h[plId]->Fill(xVal, yVal);
  	}
}

TCanvas *cc = new TCanvas("Planes","Planes",1800,720); 
cc->Divide(5, 2);

for(int i=0; i<10; ++i) {
	cc->cd(i+1);
	h[i]->Draw("COLZ");
}



}

