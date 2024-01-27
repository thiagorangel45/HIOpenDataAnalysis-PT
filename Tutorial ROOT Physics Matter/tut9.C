void tut9() {
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
}
