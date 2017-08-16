{
gROOT->Reset();

#include <string>
#include <iostream>
#include <math.h>
using namespace std;

// draw 2D reco histograms showing the events on six different RPs
//
// usage: the ROOT file to open should be in TFile file(...)
// you can choose to plot only horizontal RPs (showVertical parameter)
// then run it as a ROOT script
//
// > root
// > .x [path to the script]


// the reco plots in absolute coordinates need to be transformed to the coordinates of the detector plane
// translation (xt, yt) + rotation by 'a' degrees
//
// [ cos a	sin a	0	] [ 1	0	xt	] [ x ] [ x2 ]
// [ sin a	-cos a	0	]*[ 0	1	yt	]*[ y ]=[ y2 ]
// [ 0		0	1	] [ 0	0	1	] [ 1 ] [ 1  ]
//
const int transform = true; 
const double xt[6] = { 0.0, 0.0, -20.0, -20.0, 0.0, 0.0 };
const double yt[6] = { -18.0, 18.0, 0.0, 0.0, -18.0, 18.0 };
const double degree[6] = { 135.0, -45.0, 45.0, 45.0, 135.0, -45.0 };


string rpNames[6] = {"near top", "near bottom", "near horizontal", "far horizontal", "far top", "far bottom"};
const bool showVertical = true;
const int divX = showVertical ? 3 : 2;
const int divY = showVertical ? 2 : 1;
const int RPCount = divX*divY;

TFile file("test_reco200000.root");
TTreeReader reader("Events", &file);
TTreeReaderArray<edm::DetSet<TotemRPLocalTrack>> ps(reader, "TotemRPLocalTrackedmDetSetVector_totemRPLocalTrackFitter__TestFlatGun.obj._sets");

TH2D** h = new TH2D*[6]; 

for(int i=0; i<6; ++i) {
	h[i] = new TH2D(rpNames[i].c_str(), rpNames[i].c_str(), 100, -20, 20, 100, -20, 20);
}

while (reader.Next()) {
	for (int i=0; i<ps.GetSize(); i++) {

		int arId = (ps[i].detId()>> 24) & 0x1;
		int stId = (ps[i].detId()>> 22) & 0x3;
		int rpId = (ps[i].detId()>> 19) & 0x7;
		int plId = (ps[i].detId()>> 15) & 0xF;		//always 0 in reco

		if(arId!=0 || stId!=0) continue;

		for (auto &psEl : ps[i]) {  
			double xVal = psEl.getX0();
			double yVal = psEl.getY0();

			double d = degree[rpId] * 3.14159 / 180;
			double xVal2 = transform ? ((xVal+xt[rpId])*cos(d) + (yVal+yt[rpId])*sin(d)) : xVal;
			double yVal2 = transform ? ((xVal+xt[rpId])*sin(d) - (yVal+yt[rpId])*cos(d)) : yVal;

			h[rpId]->Fill(xVal2, yVal2);
		}

  	}
}

TCanvas *cc = new TCanvas("RPs", "RPs", 1800, 1800*divY/divX); 
cc->Divide(divX, divY);

int j=1;
for(int i=0; i<6; ++i) {
	if(!showVertical && i!=2 && i!=3) continue;
	cc->cd(j++);
	h[i]->SetStats(false);
	h[i]->Draw("COLZ");
}



}

