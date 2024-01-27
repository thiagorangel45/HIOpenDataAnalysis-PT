void tut11() {
    TH1F *hist = new TH1F("hist", "Histogram", 100, 0, 10); // Criando histograma
    TRandom2 *rand = new TRandom2(3); // Gerando os dados aleatórios
    std::fstream file; // Usando fstream para abrir o arquivo
    file.open("data.txt", std::ios::out); // Abrindo o arquivo

    for (int i = 0; i < 1000; i++) {
        double r = rand->Gaus(5, 1);
        file << r << std::endl;
    }
    file.close();

    file.open("data.txt", std::ios::in); // Reabrindo o arquivo para leitura

    double value;
    while (file >> value) {
        hist->Fill(value);
    }

    file.close();

    hist->GetXaxis()->SetTitle("Distribution");
    hist->GetYaxis()->SetTitle("Entries");
    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.05);
    hist->GetYaxis()->SetLabelSize(0.05);

    TF1 *fit = new TF1("fit", "gaus", 0, 10);
    fit->SetLineWidth(3);
    fit->SetLineColor(kRed);
    fit->SetLineStyle(2);

    TCanvas *c1 = new TCanvas(); // Criando o Canvas
    c1->SetTickx();
    c1->SetTicky();
    hist->SetStats(0);
    hist->SetFillColor(kYellow - 9);
    hist->Draw();
    hist->Fit("fit", "R");

    // Criando uma legenda
    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->AddEntry(hist, "Histogram", "l");
    leg->AddEntry(fit, "Fit Function", "l");
    leg->AddEntry(hist, "measured data", "f");
    leg->SetBorderSize(0);
    leg->Draw();

    TLine *l = new TLine(0, 20, 10, 20);
    l->SetLineWidth(2);
    l->SetLineColor(kBlue);
    l->Draw();

    double x0 = 6.3;
    int bin = hist->FindBin(x0);
    double y0 = hist->GetBinContent(bin);

    TArrow *arr = new TArrow(10,30,x0,y0);
    arr->SetLineWidth(2);
    arr->SetArrowSize(0.02);
    arr->SetLineColor(kBlue);
    arr->Draw(); 

    TLatex *t = new TLatex(10,30, "important point");
    t->Draw();

}
