void tut17()
{
    TCanvas *c1 = new TCanvas();
    TH2F *hist = new TH2F("hist", "histograma", 100, -1, 1, 100, -1, 1);
    hist->SetStats(0);
    TRandom *rand = new TRandom(10);
    for(int i = 0; i < 1e8; i++)
    {   
        double x = rand->Gaus();
        double y = rand->Gaus();
        hist->Fill(x,y);
    }
    

    hist->Draw("Colz");
}