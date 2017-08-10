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
const bool showVertical = false;
const int divX = showVertical ? 3 : 2;
const int divY = showVertical ? 2 : 1;
const int RPCount = divX*divY;

TFile file("test20000_beam2.root");
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

		if(!showVertical && rpId!=2 && rpId!=3) continue;

		if(arId!=0 || stId!=0 || plId!=0) continue;

		double xVal = ps[i].entryPoint().x();
		double yVal = ps[i].entryPoint().y();
		h[rpId]->Fill(xVal, yVal);
  	}
}

TCanvas *cc = new TCanvas("RPs", "RPs", 1800, 1800*divY/divX); 
cc->Divide(divX, divY);

int j=1;
for(int i=0; i<6; ++i) {
	if(!showVertical && i!=2 && i!=3) continue;
	cc->cd(j++);
	h[i]->Draw("COLZ");
}



}

