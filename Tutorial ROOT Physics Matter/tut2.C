void tut2(){
    
    double x[5] = {1,2,3,4,5};
    double y[5] = {1,4,9,16,25};
    TGraph *gr = new TGraph(5, x, y);
    gr->SetMarkerStyle(kFullCircle);
    gr->SetMarkerSize(5);
    TCanvas *c1 = new TCanvas();
    gr->Draw("ACP");
}