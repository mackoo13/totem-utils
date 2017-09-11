{
gROOT->Reset();

#include <string>
#include <iostream>
using namespace std;

// draw 2D histograms showing the events on diamond or UFSD timing detectors
//
// usage: the ROOT file to open should be in TFile file(...)
// you can switch diamond or UFSD mode by setting the flag ufsdMode 
// then run it as a ROOT script
//
// > root
// > .x [path to the script]


TFile file("test.root");
const bool ufsdMode = true;
TTreeReader reader("Events", &file);
string objName = ufsdMode ? "PSimHits_g4SimHits_CTPPSHitsUFSD_TestFlatGun.obj" : "PSimHits_g4SimHits_CTPPSHitsDiamond_TestFlatGun.obj";
TTreeReaderArray<PSimHit> ps(reader, objName.c_str());

const int divX = ufsdMode ? 125 : 50;
const int divY = 50;
const int spanX = ufsdMode ? 10 : 3;
const int spanY = ufsdMode ? 4 : 3;
const int cellsNum = ufsdMode ? 1 : 3;

TH2D** h = new TH2D*[cellsNum]; 
for(int i=0; i<cellsNum; ++i) {
	h[i] = new TH2D("", "", divX, -spanX, spanX, divY, -spanY, spanY);
}

while (reader.Next()) {
	for (int i=0; i<ps.GetSize(); i++) {

		int arId = (ps[i].detUnitId()>> 24) & 0x1;
		int stId = (ps[i].detUnitId()>> 22) & 0x3;
		int rpId = (ps[i].detUnitId()>> 19) & 0x7;
		int plId = (ps[i].detUnitId()>> 15) & 0xF;

		//cout<<arId<<"\t"<<stId<<"\t"<<rpId<<"\t"<<plId<<"\t"<<endl;

		double xVal = ps[i].entryPoint().x();
		double yVal = ps[i].entryPoint().y();
		h[ufsdMode ? 0 : plId-1]->Fill(xVal, yVal);
  	}
}

TCanvas *cc = new TCanvas(" ", " ", 1800, ufsdMode ? 720 : 600); 
cc->Divide(cellsNum, 1);

for(int i=0; i<cellsNum; ++i) {
	cc->cd(i+1);
	h[i]->Draw("COLZ");
}


}

