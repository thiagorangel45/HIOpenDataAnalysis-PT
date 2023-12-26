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

void forest2dimuonCB_v2(
		TString fname = "2010case.root",
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
	TCanvas *ca = new TCanvas("ca","DiMuone",900,1000);
	ca->SetTickx(); // Ticks na parte superior do eixo X
        ca->SetTicky(); // Ticks na parte esquerda do eixo y
	TH1F *dimu_h = new TH1F("","", 35, 2.7, 3.4 );
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
		if (MuPt[i]<2.0) continue;			//
		if (abs(MuEta[i])>2.4) continue;		//
		for (int j=0;j<i;j++){				//loop over 2nd muond
			if (MuHitsV[j]<12) continue;		//Muon Selections
			if (MuHitsP[j]<2) continue;		//
			if (MuTrackChi[j]>4.0) continue;	//
			if (MuDistPVz[j]>0.05) continue;	//
			if (MuPt[j]<2.0) continue;		//
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
	dimu_h->SetMarkerStyle(20);
	
	
 	const float psi_pdg_mass = 3.096916;
	double minMass=2.7;
        double maxMass=3.4;
	
	// Declare observable x
  	RooRealVar x("x","m_{#mu#mu}[GeV/c^{2}]",2.7,3.4);
        RooDataHist dh("dh","dh",x,RooFit::Import(*dimu_h));
        
 	//Crystal ball for the J/psi
  	RooRealVar mean_jpsi("mean_jpsi","mean_jpsi",3.09,2.8,3.3);
  	RooRealVar sigma_jpsi("sigma_jpsi","sigma_jpsi",0.04,0,0.1);
  	RooRealVar alpha("alpha","alpha",1.5);
  	RooRealVar n("n","n",3);
 	RooCBShape *jpsi = new RooCBShape("jpsi","crystal ball PDF",x,mean_jpsi,sigma_jpsi,alpha,n);
         
        //Exponential background
 	RooRealVar a("a","a", 0, -5, 5); //-0.85
 	RooExponential bkg("exp","exp",x,a);
 	 
 	RooRealVar fsig("fsig","signal",0.25, 0., 1.);
        RooAddPdf sum("sum","sum",RooArgList(*jpsi,bkg),RooArgList(fsig),kTRUE);

	sum.getParameters(dh)->setAttribAll("Constant",kTRUE) ;
        fsig.setConstant(kFALSE) ;
         
        //sum.fitTo(dh,Range(2.6, 3.5));
         
        RooPlot* mframe = x.frame(Title("Fit of invariant mass in 2010 PbPb collision"));
 	dh.plotOn(mframe);
        sum.plotOn(mframe, FillStyle(3004), FillColor(kBlack), LineColor(kBlack), DrawOption("LF"), Range(2.7, 3.4));
        
        // Plotar a componente background com cor azul e tracejada
        sum.plotOn(mframe, Components(bkg), FillStyle(3001), FillColor(kBlue), DrawOption("LF"), LineStyle(kDashed));
    
        // Desenhando o mframe     
        mframe->Draw();     
         
        // Adicionando a legenda ao gráfico
         
        TLegend *legend = new TLegend(0.15, 0.3, 0.3, 0.5);  
        legend->SetTextSize(0.028);  
        legend->SetBorderSize(0);  

        // Adicionar entrada para os dados
        TLegendEntry *entry1 = legend->AddEntry(dimu_h, "Data", "lep");
        entry1->SetMarkerStyle(20);

        // Adicionar entrada para o ajuste da Crystall ball
        TLegendEntry *entry2 = legend->AddEntry(jpsi, "Fit", "l");
        entry2->SetLineWidth(2.0);  // Ajustar a largura da linha

        // Adicionar entrada para o ajuste do Background
        TLegendEntry *entry3 = legend->AddEntry(jpsi, "Background", "l");
        entry3->SetLineColor(kBlue);       
        entry3->SetLineStyle(kDashed);
    
        // Plotando a legenda
        legend->Draw("same");
    
        // Adicionando texto no canvas
        TLatex *latex = new TLatex();
        latex->SetNDC(); 
        latex->SetTextSize(0.05);
        latex->SetTextColor(kBlack);
        latex->DrawLatex(0.15, 0.80, "CMS Open Data");
    
        latex->SetTextSize(0.03); // para deixar o resto com outro tamanho de fonte
        latex->DrawLatex(0.15, 0.74, "PbPb 2010 #sqrt{s_{NN}} = 2.76 TeV");
        latex->DrawLatex(0.15, 0.70, "P^{#mu}_{T} > 2.0 GeV");
        latex->DrawLatex(0.15, 0.66, "#lbar#eta#lbar < 2.4");
        latex->DrawLatex(0.15, 0.62, "Trigger: HLT_HIL1DoubleMuOpen");
        
        TFile* outputFile = new TFile("dimuon_mass_fit_2010_PbPb_2.root", "RECREATE");
        outputFile->cd();
        dimu_h->Write();
        outputFile->Close();
        
        // Salvando o plot como um arquivo .png  
	ca->Print("diMuon_mass_2010_PbPb_2.png");

} 

