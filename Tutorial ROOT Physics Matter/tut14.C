void tut14()
{
    TFile *file = new TFile("output.root", "RECREATE");
    TH1F *hist = new TH1F("hist","",100,0,100);
    hist->Fill(10);
    hist->Fill(90);
    hist->Draw();
    file->Write();
    file->Close();
}