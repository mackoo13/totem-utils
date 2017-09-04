{
gROOT->Reset();

#include <string>
#include <iostream>
#include <math.h>
using namespace std;

// draw 2D histograms showing the events on six different RPs
//
// > root
// > .x [path to the script]

// ========= PARAMS =============

// mode:
// 	0 - draw all 6 RPs separately
// 	1 - draw plots for near/far RPs separately
// 	2 - draw plots for near/far horizontal RPs separately
const int mode = 1;

// filename:
TFile file("test20000.root");

// select the RPs to draw - leave (-1) to draw all of them
const int arm = 1;
const int station = 0;
const int romanPot = -1;
const int plane = 1; 

// ========= ENDPARAMS =========

// =========== CONST ===========
// first 6 RPs for right arm, then for the left
// distances taken from RP_Dist_Beam_Cent.xml plus 11mm of th distance from the detector cut edge to its center
const double xt[24] = { 0.0, 0.0, 17.480, 17.181, 0.0, 0.0, 0.0, 0.0, 50.0, 50.0, 0.0, 0.0,
		0.0, 0.0, 17.480, 17.181, 0.0, 0.0, 0.0, 0.0, 17.480, 17.181, 0.0, 0.0 };

const double yt[24] = { 20.129, -19.885, 0.0, 0.0, 19.043, -19.320, 50.0, -50.0, 0.0, 0.0, 50.0, -50.0,
		20.052, -19.017, 0.0, 0.0, -18.553, 18.363, 50.0, -50.0, 0.0, 0.0, 50.0, -50.0 };

// rotations - with the tilted right far pots in 210 station
double degree[24] = { 135.0, -45.0, 45.0, 45.0, 135.0, -45.0, 135.0, -45.0, 45.0, 45.0, 135.0, -45.0,
		135.0, -45.0, 45.0, 53.0, 143.0, -37.0, 135.0, -45.0, 45.0, 45.0, 135.0, -45.0 };

for(int i=0; i<24; ++i) degree[i] = degree[i] * 3.14159 / 180;

string rpNames[18] = {"near top", "near bottom", "near horizontal", "far horizontal", "far top", "far bottom",
		"near hits", "far hits", "", "", "", "",
		"near hits", "far hits", "", "", "", ""};

const int divX = mode==0 ? 3 : 2;
const int divY = mode==0 ? 2 : 1;
const int RPCount = divX*divY;
const int minX = mode==0 ? -20 : (mode==1 ? -50 : -10);
const int maxX = mode==0 ? -minX : (mode==1 ? 50 : 50);
const int minY = mode==0 ? minX : (mode==1 ? -50 : -30);
const int maxY = -minY;
// =========== ENDCONST ========






TTreeReader reader("Events", &file);
TTreeReaderArray<PSimHit> ps(reader, "PSimHits_g4SimHits_TotemHitsRP_TestFlatGun.obj");

TH2D** h = new TH2D*[RPCount]; 

for(int i=0; i<RPCount; ++i) {
	h[i] = new TH2D(rpNames[6*mode+i].c_str(), rpNames[6*mode+i].c_str(), 100, minX, maxX, 100, minY, maxY);
}

while (reader.Next()) {
	for (int i=0; i<ps.GetSize(); i++) {

		int arId = (ps[i].detUnitId()>> 24) & 0x1;
		int stId = (ps[i].detUnitId()>> 22) & 0x3;
		int rpId = (ps[i].detUnitId()>> 19) & 0x7;
		int plId = (ps[i].detUnitId()>> 15) & 0xF;

		if(mode==2 && rpId!=2 && rpId!=3) continue;
		if((arm>=0 && arId!=arm) || (station>=0 && stId!=station) || (romanPot>=0 && rpId!=romanPot) || (plane>=0 && plId!=plane)) continue;

		double xVal = ps[i].entryPoint().x();
		double yVal = ps[i].entryPoint().y();
	
		if(mode==1 || mode==2) {
			double xVal2 = xVal;
			double yVal2 = yVal;
			int rpPosId = 12*arId + 6*(stId==0 ? 0 : 1) + rpId;

			if(arId==1 && stId==0 && rpId>2) {	//rotated pots
				double eightDeg = 8 * 3.14159 / 180;
				xVal = xVal2*cos(degree[rpPosId]) + yVal2*sin(degree[rpPosId]) + xt[rpPosId]*cos(eightDeg) + yt[rpPosId]*sin(eightDeg);
				yVal = xVal2*sin(degree[rpPosId]) - yVal2*cos(degree[rpPosId]) + xt[rpPosId]*sin(eightDeg) - yt[rpPosId]*cos(eightDeg);
			} else {
				xVal = xVal2*cos(degree[rpPosId]) + yVal2*sin(degree[rpPosId]) + xt[rpPosId];
				yVal = xVal2*sin(degree[rpPosId]) - yVal2*cos(degree[rpPosId]) + yt[rpPosId];
			}
		}

		int graphId = mode==0 ? rpId : (mode==1 ? rpId/3 : rpId-2);
		h[graphId]->Fill(xVal, yVal);
  	}
}

gStyle->SetPadGridX(1);
gStyle->SetPadGridY(1);
TCanvas *cc = new TCanvas("RPs", "RPs", 1800, 1800*divY/divX); 
cc->Divide(divX, divY);


for(int i=0; i<RPCount; ++i) {
	cc->cd(i+1);
	cc->SetFixedAspectRatio(1.0);
	h[i]->SetStats(false);
	h[i]->GetXaxis()->SetTitle("x [mm]");
	h[i]->GetYaxis()->SetTitle("y [mm]");
	h[i]->Draw("COLZ");
}

}

