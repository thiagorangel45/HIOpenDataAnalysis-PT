void tut8() {
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

    hist->GetXaxis()->SetTitle("Distribuição Aleatória");
    hist->GetYaxis()->SetTitle("Entries Axis");

    TF1 *fit = new TF1("fit", "gaus", 0, 10);

    TCanvas *c1 = new TCanvas(); // Criando o Canvas
    hist->SetStats(0);
    hist->Draw();
    hist->Fit("fit", "R");

    // Criando uma legenda
    TLegend *leg = new TLegend(0.7, 0.7, 0.9, 0.9);
    leg->AddEntry(hist, "Histogram", "l");
    leg->AddEntry(fit, "Fit Function", "l");
    leg->Draw();
}
