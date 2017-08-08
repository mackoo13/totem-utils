{
gROOT->Reset();

#include <string>
#include <iostream>
using namespace std;

// draw 2D histograms showing the events on six different RPs
//
// usage: the ROOT file to open should be in TFile file(...)
// then run it as a ROOT script
//
// > root
// > .x [path to the script]


string rpNames[6] = {"near top", "near bottom", "near horizontal", "far horizontal", "far top", "far bottom"};

TFile file("test20000_beam1.root");
TTreeReader reader("Events", &file);
TTreeReaderArray<PSimHit> ps(reader, "PSimHits_g4SimHits_TotemHitsRP_TestFlatGun.obj");

TH2D** h = new TH2D*[6]; 

for(int i=0; i<6; ++i) {
	h[i] = new TH2D(rpNames[i].c_str(), rpNames[i].c_str(), 100, -20, 20, 100, -20, 20);
}

while (reader.Next()) {
	for (int i=0; i<ps.GetSize(); i++) {

		int arId = (ps[i].detUnitId()>> 24) & 0x1;
		int stId = (ps[i].detUnitId()>> 22) & 0x3;
		int rpId = (ps[i].detUnitId()>> 19) & 0x7;
		int plId = (ps[i].detUnitId()>> 15) & 0xF;

		if(arId==0x0) continue;

		double xVal = ps[i].entryPoint().x();
		double yVal = ps[i].entryPoint().y();
		h[rpId]->Fill(xVal, yVal);
  	}
}

TCanvas *cc = new TCanvas("RPs","RPs",1500,1000); 
cc->Divide(3, 2);

for(int i=0; i<6; ++i) {
	cc->cd(i+1);
	h[i]->Draw("COLZ");
}



}

