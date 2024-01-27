void tut5() {
    
    TRandom2 *rand = new TRandom2(0);
    TH1F *hist = new TH1F("","", 100, 0, 100);
    for (int i = 0; i < 100000; i++) {  
        double r = rand->Rndm()*100;
        std::cout << r << std::endl; 
        hist->Fill(r);
    }
    TCanvas *c1 = new TCanvas();
    hist->GetYaxis()->SetRangeUser(0,2000);
    hist->Draw();
}
