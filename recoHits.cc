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


// the reco plots in absolute coordinates can be transformed to the coordinates of the detector plane
// translation (xt, yt) + rotation by 'a' degrees
//
// [ cos a	sin a	0	] [ 1	0	xt	] [ x ] [ x2 ]
// [ sin a	-cos a	0	]*[ 0	1	yt	]*[ y ]=[ y2 ]
// [ 0		0	1	] [ 0	0	1	] [ 1 ] [ 1  ]
//

// PARAMS
const bool transform = true; 
const bool plotAll = false;
TFile file("test_reco200000.root");
// END PARAMS

// CONST
const double xt[6] = { 0.0, 0.0, -20.0, -20.0, 0.0, 0.0 };
const double yt[6] = { -18.0, 18.0, 0.0, 0.0, -18.0, 18.0 };
const double degree[6] = { 135.0, -45.0, 45.0, 45.0, 135.0, -45.0 };

string rpNames[6] = {"near top", "near bottom", "near horizontal", "far horizontal", "far top", "far bottom"};
string rpGroupNames[2] = {"near", "far"};
const int divX = plotAll ? 3 : 2;
const int divY = plotAll ? 2 : 1;
const int RPCount = divX*divY;
const int plotRange = 70;
// END CONST

TTreeReader reader("Events", &file);
TTreeReaderArray<edm::DetSet<TotemRPLocalTrack>> ps(reader, "TotemRPLocalTrackedmDetSetVector_totemRPLocalTrackFitter__TestFlatGun.obj._sets");

TH2D** h = new TH2D*[RPCount]; 

for(int i=0; i<RPCount; ++i) {
	if(!transform && !plotAll) h[i] = new TH2D(rpGroupNames[i].c_str(), rpGroupNames[i].c_str(), 100, -plotRange, plotRange, 100, -plotRange, plotRange);
	else if(transform && !plotAll) h[i] = new TH2D(rpNames[i+2].c_str(), rpNames[i+2].c_str(), 100, -plotRange, plotRange, 100, -plotRange, plotRange);
	else h[i] = new TH2D(rpNames[i].c_str(), rpNames[i].c_str(), 100, -plotRange, plotRange, 100, -plotRange, plotRange);
}

while (reader.Next()) {
	for (int i=0; i<ps.GetSize(); i++) {

		int arId = (ps[i].detId()>> 24) & 0x1;
		int stId = (ps[i].detId()>> 22) & 0x3;
		int rpId = (ps[i].detId()>> 19) & 0x7;
		int plId = (ps[i].detId()>> 15) & 0xF;		//always 0 in reco

		if(arId!=1 || stId!=0) continue;
		if(transform && !plotAll && rpId!=2 && rpId!=3) continue;

		for (auto &psEl : ps[i]) {  
			double xVal = psEl.getX0();
			double yVal = psEl.getY0();

			double d = degree[rpId] * 3.14159 / 180;
			double xVal2 = transform ? ((xVal+xt[rpId])*cos(d) + (yVal+yt[rpId])*sin(d)) : xVal;
			double yVal2 = transform ? ((xVal+xt[rpId])*sin(d) - (yVal+yt[rpId])*cos(d)) : yVal;

			int hId = plotAll ? rpId : (transform ? rpId-2 : rpId/3);
			h[hId]->Fill(xVal2, yVal2);
		}

  	}
}

TCanvas *cc = new TCanvas("RPs", "RPs", 1800, 1800*divY/divX); 
cc->Divide(divX, divY);

int j=1;
for(int i=0; i<RPCount; ++i) {
	cc->cd(i+1);
	h[i]->SetStats(false);
	h[i]->Draw("COLZ");
}



}

