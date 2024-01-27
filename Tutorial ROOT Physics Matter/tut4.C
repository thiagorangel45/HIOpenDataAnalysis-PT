void tut4(){
    
    TGraph *gr = new TGraph();
    gr->SetMarkerStyle(kFullCircle);
    fstream file;
    file.open("data2.txt", ios::in);
    gr->SetLineWidth(2);
    gr->SetLineColor(kBlue);
    gr->GetXaxis()->SetTitle("X values");
    gr->GetYaxis()->SetTitle("Y values");
    gr->SetTitle("Graph");
    while (1)
    {
        double x, y;
        file >> x >> y;
        gr->SetPoint(gr->GetN(), x, y);
        if (file.eof()) break;
    }
    file.close();
    TCanvas *c1 = new TCanvas();
    gr->Draw("ALP");
}