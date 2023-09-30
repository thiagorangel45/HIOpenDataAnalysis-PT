#include <TMath.h>
#include <TLorentzVector.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <TF1.h>
#include <TLatex.h> 
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"
#include "RooChebychev.h"
#include "RooCBShape.h"
#include "RooPlot.h"
#include "RooMCStudy.h"
#include "RooFitResult.h"
#include "RooThresholdCategory.h"
#include "RooBinningCategory.h"
#include "RooWorkspace.h"
#include "RooArgList.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TAxis.h"
#include "TGraphAsymmErrors.h"
#include "TPaveLabel.h"
#include "TFolder.h"

using std::stringstream;
using std::vector;
using std::abs;

using namespace RooFit; // Add this line to bring RooFit into scope

void forest2dimuonCB(
		TString fname = "2011case.root",
		TString trig = "HLT_HIL1DoubleMuOpen",
		TString Collection = "demo", 
		float minPt = 0,
		int nevt=-1
		) {  

	using namespace std;
	TFile *f1 = new TFile(fname.Data());
	Float_t mumass=0.105658;
	TTree *HltTree = (TTree*)f1->Get("hltanalysis/HltTree");
	TTree *MuTree = (TTree*)f1->Get(Form("%s/Muons",Collection.Data()));
	TCanvas *ca = new TCanvas("ca","DiMuone",1200,800);
	TH1F *dimu_h = new TH1F("","", 50, 2.7, 3.4 );
	//dimu_h->SetStats(kFALSE);
        dimu_h->GetXaxis()->SetTitle("m_{#mu#mu}[GeV/c^{2}]"); 
        dimu_h->GetYaxis()->SetTitle("Events/(0.02 GeV/c^{2})");
	dimu_h->SetFillColor(kYellow);
        ca->SetFrameLineColor(1);
        ca->SetFrameFillColor(0);
	ca->SetFillColor(10);
	//c1->SetLogx();
	//ca->SetLogy();
	dimu_h->Sumw2();
	MuTree->AddFriend(HltTree);
	Int_t           trigBit;
	TBranch        *b_trigBit;
	if (trig == "" ) {  cout << " No Trigger selection! " << endl ;}     
	else { 
		HltTree->SetBranchAddress(trig.Data(), &trigBit, &b_trigBit);
	}
	/// Muon inputs : 
	    Int_t           nMu;
	    Float_t MuPt[10];
	    Float_t MuC[10];
	    Float_t MuEta[10];
	    Float_t MuPhi[10];
	    Int_t MuHitsV[10];
	    Int_t MuHitsP[10];
	    Float_t MuTrackChi[10];
	    Float_t MuDistPVz[10];
	    TBranch        *b_nMu;   //!
	    TBranch        *b_MuPt;   //!
	    TBranch        *b_MuC;   //!
	    TBranch        *b_MuEta;   //!
	    TBranch        *b_MuPhi;   //!
	    TBranch        *b_MuHitsV;   //!
	    TBranch        *b_MuHitsP;   //!
	    TBranch        *b_MuTrackChi;   //!
	    TBranch        *b_MuDistPVz;   //!

	    MuTree->SetBranchAddress("Nmu", &nMu, &b_nMu);
	    MuTree->SetBranchAddress("muPt", MuPt, &b_MuPt);
	    MuTree->SetBranchAddress("muC", MuC, &b_MuC);
	    MuTree->SetBranchAddress("muEta", MuEta, &b_MuEta);
	    MuTree->SetBranchAddress("muPhi", MuPhi, &b_MuPhi);
	    MuTree->SetBranchAddress("muHitsValid", MuHitsV, &b_MuHitsV);
	    MuTree->SetBranchAddress("muHitsPixel", MuHitsP, &b_MuHitsP);
	    MuTree->SetBranchAddress("muTrackChi2NDOF", MuTrackChi, &b_MuTrackChi);
	    MuTree->SetBranchAddress("muDistPV0", MuDistPVz, &b_MuDistPVz);

	////////////////////////////////////////////////////////////////////////
	//////////////////  dijet tree 
	////////////////////////////////////////////////////////////////////////
	TLorentzVector v1, v2, dimu; //4-vectors for muons and dimuon
	Int_t numevt = 0;
	if(nevt == -1) nevt = HltTree->GetEntries(); 
	cout << "Events to Analyze: "<<nevt<<endl;
	for (int iev=0; iev<nevt; iev++) {
		if(iev%100000==0)
		{ 
			cout << ">>>>> EVENT " << iev << endl; 
		}
		HltTree->GetEntry(iev);
		if ( (trig != "" ) && (!trigBit) ) {
		continue;					//check if the trigger is fired
		}
		MuTree->GetEntry(iev);
		if (nMu<2)continue;				//We need at least 2 muons in an event
	for (int i=1;i<nMu;i++){
		if (MuHitsV[i]<12) continue;			//Muon Selections
		if (MuHitsP[i]<2) continue;			//
		if (MuTrackChi[i]>4.0) continue;		//
		if (MuDistPVz[i]>0.05) continue;		//
		if (MuPt[i]<0.0) continue;			//
		if (abs(MuEta[i])>2.4) continue;		//
		for (int j=0;j<i;j++){				//loop over 2nd muond
			if (MuHitsV[j]<12) continue;		//Muon Selections
			if (MuHitsP[j]<2) continue;		//
			if (MuTrackChi[j]>4.0) continue;	//
			if (MuDistPVz[j]>0.05) continue;	//
			if (MuPt[j]<0.0) continue;		//
			if (abs(MuEta[j])>2.4) continue;	//
			if (MuC[i]>0&&MuC[j]>0) continue;	//Only opposite charge muons
			if (MuC[i]<0&&MuC[j]<0) continue;	//
		v1.SetPtEtaPhiM( MuPt[i], MuEta[i], MuPhi[i], mumass );
		v2.SetPtEtaPhiM( MuPt[j], MuEta[j], MuPhi[j], mumass );
		dimu=v1+v2;
		dimu_h->Fill(dimu.M());
		}
		
	}
	} //ende of event loop
	dimu_h->Draw("P");
	
	
	//TH1F* massa = (TH1F*)file->Get("dimu_h"); 
	double hmin = dimu_h->GetXaxis()->GetXmin(); cout << ">>>>> hmin " << hmin << endl; 
 	double hmax = dimu_h->GetXaxis()->GetXmax(); cout << ">>>>> hmax " << hmax << endl; 
 	const float psi_pdg_mass = 3.096916;
	double minMass=2.6;
        double maxMass=3.5;
	
	// Declare observable x
  	//RooRealVar x("x","x",hmin,hmax) ;
  	RooRealVar x("x","x",2.7,3.4);
        RooDataHist dh("dh","dh",x,RooFit::Import(*dimu_h));
        
        
        
 	 //Crystal ball for the J/psi
  
  	RooRealVar mean_jpsi("mean_jpsi","mean_jpsi",3.09,2.7,3.4);
  	RooRealVar sigma_jpsi("sigma_jpsi","sigma_jpsi",0.04,0,0.1);
  	RooRealVar alpha("alpha","alpha",1.5);
  	RooRealVar n("n","n",3);
 	RooCBShape *jpsi = new RooCBShape("jpsi","crystal ball PDF",x,mean_jpsi,sigma_jpsi,alpha,n);
         
        //Exponential background
  
 	 RooRealVar a("a","a",-0.85,-0.8,-0.9);
 	 RooExponential bkg("exp","exp",x,a);
  
  	 RooRealVar       c0("c0", "c0", -.732850, -2., 0.);
  	 RooExponential mBkg("mBkg", "exponential", x, c0);
  
 	 RooRealVar   c1("c1","coefficient #1", -.0276343, -.2, 0.) ; // 1.1
 	 RooExponential expo("expo","background p.d.f.", x, c1); 
 	 
 	 RooRealVar fsig("fsig","signal",0.1,0.,1.);
         RooAddPdf sum("sum","sum",RooArgList(*jpsi,bkg),RooArgList(fsig),kTRUE);

	 sum.getParameters(dh)->setAttribAll("Constant",kTRUE) ;
         fsig.setConstant(kFALSE) ;
         
         sum.fitTo(dh,Range(2.7,3.4));
         
         RooPlot* mframe = x.frame(Title("Fit of invariant mass"));
 	 dh.plotOn(mframe);
         sum.plotOn(mframe);
         sum.plotOn(mframe,Components(bkg),LineStyle(kDashed));
  
         mframe->Draw();

        
	/*auto crystalball  = new TF1("crystalball","crystalball", 2.6, 3.5);
        crystalball->SetParameters(590, 3.09, 0.04, 1.1, 1);
        dimu_h->Fit(crystalball, "R");
        crystalball->SetLineColor(kRed);
        crystalball->Draw("SAME");*/
	
	
        /*TLatex label; label.SetNDC(true);
        label.SetTextSize(0.040); label.DrawLatex(0.100, 0.920, "#bf{CMS Open Data}");
        label.SetTextSize(0.025); label.DrawLatex(0.650, 0.800, "PbPb #sqrt{s_{NN}} = 2.76 TeV");
        //label.SetTextSize(0.025); label.DrawLatex(0.150, 0.760, "L_{int} = 150#mub^{-1}");
        label.SetTextSize(0.025); label.DrawLatex(0.650, 0.760, "P^{#mu}_{T} > 0.0 GeV");
        label.SetTextSize(0.025); label.DrawLatex(0.650, 0.720, "#lbar#eta#lbar < 2.4");
        label.SetTextSize(0.025); label.DrawLatex(0.650, 0.680, "Trigger: HLT_HIL1DoubleMuOpen");*/
	
	
	ca->Print("diMuon_mass_2011_1.png");
	//	exit();
} 

