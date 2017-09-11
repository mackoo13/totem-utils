{
gROOT->Reset();

#include <string>
#include <iostream>
using namespace std;

// draw 2D histogram of hits on pixel detector
//
// usage: the ROOT file to open should be in TFile file(...)
// then run it as a ROOT script
//
// > root
// > .x [path to the script]


TFile file("test.root");
TTreeReader reader("Events", &file);
TTreeReaderArray<PSimHit> ps(reader, "PSimHits_g4SimHits_CTPPSHitsUFSD_TestFlatGun.obj");	//todo obj name

TH2D* h = new TH2D("a", "a", 30, -12, 12, 30, -4, 4);

while (reader.Next()) {
	for (int i=0; i<ps.GetSize(); i++) {

		int arId = (ps[i].detUnitId()>> 24) & 0x1;
		int stId = (ps[i].detUnitId()>> 22) & 0x3;
		int rpId = (ps[i].detUnitId()>> 19) & 0x7;
		int plId = (ps[i].detUnitId()>> 15) & 0xF;

		//if(arId!=1 || stId!=2) continue;

		cout<<arId<<"\t"<<stId<<"\t"<<rpId<<"\t"<<plId<<"\t"<<endl;

		double xVal = ps[i].entryPoint().x();
		double yVal = ps[i].entryPoint().y();
		//cout<<xVal<<", "<<yVal<<endl;
		h->Fill(xVal, yVal);
  	}
}

h->Draw("COLZ");


}

