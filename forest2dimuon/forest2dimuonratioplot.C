#include <TMath.h>
#include <TLorentzVector.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <TF1.h>
#include <TLatex.h>

using std::stringstream;
using std::vector;
using std::abs;

void forest2dimuonratioplot(
        TString fname1 = "2011ppcase.root",
        TString trig1 = "HLT_L2DoubleMu0_v3",
        TString fname2 = "2011case.root",
        TString trig2 = "HLT_HIL1DoubleMuOpen",
        TString Collection = "demo",
        float minPt = 0,
        int nevt = -1
) {
    using namespace std;

    Float_t mumass = 0.105658;


//***************************************** Seleção do Primeiro arquivo de colisões pp **************************************************//

    TFile *f1 = new TFile(fname1.Data());
    TTree *HltTree1 = (TTree *) f1->Get("hltanalysis/HltTree");
    TTree *MuTree1 = (TTree *) f1->Get(Form("%s/Muons", Collection.Data()));
    TCanvas *c1 = new TCanvas("c1", "DiMuone", 1200, 1200);
    TH1F *dimu_h1 = new TH1F("hist1", "", 100, 0, 10);
    dimu_h1->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
    dimu_h1->GetYaxis()->SetTitle("Events/(1 GeV)");
    dimu_h1->SetLineStyle(1);
    dimu_h1->SetLineColor(kBlue); 
    dimu_h1->SetStats(kFALSE);
    
//*************************************************** Informação sobre o Canvas *********************************************************//

 
    c1->SetFrameLineColor(1);
    c1->SetFrameFillColor(0);
    c1->SetFillColor(10);
    c1->SetLogx();
    c1->SetLogy();
    
    
//************************************************* Fim Informação sobre o Canvas *******************************************************//
    
    dimu_h1->Sumw2();
    MuTree1->AddFriend(HltTree1);
    Int_t trigBit1;
    TBranch *b_trigBit1;
    if (trig1.Length() > 0) {
        HltTree1->SetBranchAddress(trig1.Data(), &trigBit1);
    } else {
        cout << "No Trigger selection for File 1!" << endl;
    }
    Int_t nMu;
    Float_t MuPt[10];
    Float_t MuC[10];
    Float_t MuEta[10];
    Float_t MuPhi[10];
    Int_t MuHitsV[10];
    Int_t MuHitsP[10];
    Float_t MuTrackChi[10];
    Float_t MuDistPVz[10];
    TBranch *b_nMu, *b_MuPt, *b_MuC, *b_MuEta, *b_MuPhi, *b_MuHitsV, *b_MuHitsP, *b_MuTrackChi, *b_MuDistPVz;

    MuTree1->SetBranchAddress("Nmu", &nMu, &b_nMu);
    MuTree1->SetBranchAddress("muPt", MuPt, &b_MuPt);
    MuTree1->SetBranchAddress("muC", MuC, &b_MuC);
    MuTree1->SetBranchAddress("muEta", MuEta, &b_MuEta);
    MuTree1->SetBranchAddress("muPhi", MuPhi, &b_MuPhi);
    MuTree1->SetBranchAddress("muHitsValid", MuHitsV, &b_MuHitsV);
    MuTree1->SetBranchAddress("muHitsPixel", MuHitsP, &b_MuHitsP);
    MuTree1->SetBranchAddress("muTrackChi2NDOF", MuTrackChi, &b_MuTrackChi);
    MuTree1->SetBranchAddress("muDistPV0", MuDistPVz, &b_MuDistPVz);

    ////////////////////////////////////////////////////////////////////////
    //////////////////  dijet tree 1
    ////////////////////////////////////////////////////////////////////////
    TLorentzVector v1, v2, dimu; // 4-vectors for muons and dimuon
    Int_t numevt = 0;
    if (nevt == -1) nevt = HltTree1->GetEntries();
    cout << "Events to Analyze: " << nevt << endl;
    for (int iev = 0; iev < nevt; iev++) {
        if (iev % 100000 == 0) {
            cout << ">>>>> EVENT " << iev << endl;
        }
        HltTree1->GetEntry(iev);
        if ((trig1 != "") && (!trigBit1)) {
            continue; // check if the trigger is fired
        }
        MuTree1->GetEntry(iev);
        if (nMu < 2) continue; // We need at least 2 muons in an event
        for (int i = 1; i < nMu; i++) {
            if (MuHitsV[i] < 12) continue; // Muon Selections
            if (MuHitsP[i] < 2) continue; //
            if (MuTrackChi[i] > 4.0) continue; //
            if (MuDistPVz[i] > 0.05) continue; //
            if (MuPt[i] < 1.4) continue; //
            if (abs(MuEta[i]) > 2.4) continue; //
            for (int j = 0; j < i; j++) { // loop over 2nd muond
                if (MuHitsV[j] < 12) continue; // Muon Selections
                if (MuHitsP[j] < 2) continue; //
                if (MuTrackChi[j] > 4.0) continue; //
                if (MuDistPVz[j] > 0.05) continue; //
                if (MuPt[j] < 1.4) continue; //
                if (abs(MuEta[j]) > 2.4) continue; //
                if (MuC[i] > 0 && MuC[j] > 0) continue; // Only opposite charge muons
                if (MuC[i] < 0 && MuC[j] < 0) continue; //
                v1.SetPtEtaPhiM(MuPt[i], MuEta[i], MuPhi[i], mumass);
                v2.SetPtEtaPhiM(MuPt[j], MuEta[j], MuPhi[j], mumass);
                dimu = v1 + v2;
                dimu_h1->Fill(dimu.M());
            }
        }
    } // end of event loop for HltTree1 
    
    
//************************************* Fim da Seleção do Primeiro arquivo de colisões pp ***********************************************//

    
//***************************************** Seleção do Segundo arquivo de colisões PbPb *************************************************//
    
    
    TFile *f2 = new TFile(fname2.Data());
    TTree *HltTree2 = (TTree *) f2->Get("hltanalysis/HltTree");
    TTree *MuTree2 = (TTree *) f2->Get(Form("%s/Muons", Collection.Data()));
    TH1F *dimu_h2 = new TH1F("hist2", "", 100, 0, 10);
    dimu_h2->SetLineStyle(1);
    dimu_h2->SetLineColor(kBlack);
    dimu_h2->SetStats(kFALSE);
    dimu_h2->Sumw2();
    MuTree2->AddFriend(HltTree2);
    Int_t trigBit2;
    TBranch *b_trigBit2;
    if (trig2.Length() > 0) {
        HltTree2->SetBranchAddress(trig2.Data(), &trigBit2);
    } else {
        cout << "No Trigger selection for File 2!" << endl;
    }

    Int_t nMu2;
    Float_t MuPt2[10];
    Float_t MuC2[10];
    Float_t MuEta2[10];
    Float_t MuPhi2[10];
    Int_t MuHitsV2[10];
    Int_t MuHitsP2[10];
    Float_t MuTrackChi2[10];
    Float_t MuDistPVz2[10];
    TBranch *b_nMu2, *b_MuPt2, *b_MuC2, *b_MuEta2, *b_MuPhi2, *b_MuHitsV2, *b_MuHitsP2, *b_MuTrackChi2, *b_MuDistPVz2;

    MuTree2->SetBranchAddress("Nmu", &nMu2, &b_nMu2);
    MuTree2->SetBranchAddress("muPt", MuPt2, &b_MuPt2);
    MuTree2->SetBranchAddress("muC", MuC2, &b_MuC2);
    MuTree2->SetBranchAddress("muEta", MuEta2, &b_MuEta2);
    MuTree2->SetBranchAddress("muPhi", MuPhi2, &b_MuPhi2);
    MuTree2->SetBranchAddress("muHitsValid", MuHitsV2, &b_MuHitsV2);
    MuTree2->SetBranchAddress("muHitsPixel", MuHitsP2, &b_MuHitsP2);
    MuTree2->SetBranchAddress("muTrackChi2NDOF", MuTrackChi2, &b_MuTrackChi2);
    MuTree2->SetBranchAddress("muDistPV0", MuDistPVz2, &b_MuDistPVz2);

    ////////////////////////////////////////////////////////////////////////
    //////////////////  dijet tree 2
    ////////////////////////////////////////////////////////////////////////
    TLorentzVector v1_2, v2_2, dimu_2; // 4-vectors for muons and dimuon
    Int_t numevt_2 = 0;
    if (nevt == -1) nevt = HltTree2->GetEntries();
    cout << "Events to Analyze: " << nevt << endl;
    for (int iev = 0; iev < nevt; iev++) {
       if (iev % 100000 == 0) {
        cout << ">>>>> EVENT " << iev << endl;
    }
    HltTree2->GetEntry(iev);
    if ((trig2 != "") && (!trigBit2)) {
        continue; // check if the trigger is fired
    }
    MuTree2->GetEntry(iev);
    if (nMu2 < 2) continue; // We need at least 2 muons in an event
    for (int i = 1; i < nMu2; i++) {
        if (MuHitsV2[i] < 12) continue; // Muon Selections
        if (MuHitsP2[i] < 2) continue; //
        if (MuTrackChi2[i] > 4.0) continue; //
        if (MuDistPVz2[i] > 0.05) continue; //
        if (MuPt2[i] < 1.4) continue; //
        if (abs(MuEta2[i]) > 2.4) continue; //
        for (int j = 0; j < i; j++) { // loop over 2nd muond
            if (MuHitsV2[j] < 12) continue; // Muon Selections
            if (MuHitsP2[j] < 2) continue; //
            if (MuTrackChi2[j] > 4.0) continue; //
            if (MuDistPVz2[j] > 0.05) continue; //
            if (MuPt2[j] < 1.4) continue; //
            if (abs(MuEta2[j]) > 2.4) continue; //
            if (MuC2[i] > 0 && MuC2[j] > 0) continue; // Only opposite charge muons
            if (MuC2[i] < 0 && MuC2[j] < 0) continue; //
            v1_2.SetPtEtaPhiM(MuPt2[i], MuEta2[i], MuPhi2[i], mumass);
            v2_2.SetPtEtaPhiM(MuPt2[j], MuEta2[j], MuPhi2[j], mumass);
            dimu_2 = v1_2 + v2_2;
            dimu_h2->Fill(dimu_2.M());
        }
    }
} // end of event loop for HltTree2 

//************************************ Fim da Seleção do Segundo arquivo de colisões PbPb ***********************************************//



//************************************************** Fator de Normalização  *************************************************************//

    
	double scale_factor = dimu_h1->Integral() / dimu_h2->Integral();
	dimu_h2->Scale(scale_factor);

    
//*************************************************** Fator de Normalização *************************************************************//



//******************************************************** Ratio Plot *******************************************************************//
	
	c1->cd();
	c1->Divide(1, 2);  
	c1->cd(1);
	gPad->SetLogy();
	dimu_h1->Draw("Hist");
	dimu_h2->Draw("Hist SAME");


	c1->cd(2);
	
//******************************************************** Ratio Plot *******************************************************************//

	
        TH1F *ratio = (TH1F *)dimu_h1->Clone("ratio");
        ratio->Divide(dimu_h2);
        ratio->SetLineColor(kBlack);
        ratio->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
        ratio->GetYaxis()->SetTitle("pp / PbPb");
        ratio->GetYaxis()->SetRangeUser(0.0, 5.0);
        ratio->Draw("Hist");
        
//******************************************************** Ratio Plot *******************************************************************//


	c1->Print("diMuon_ratio_plot.png");
    
}
