void tut6() {
    
    TH1F *hist = new TH1F("hist", "Histogram", 100, 0, 10); // Criando histograma

    TRandom2 *rand = new TRandom2(3); // Gerando os dados aleatórios

    std::fstream file; // Usando fstream para abrir o arquivo
    file.open("data.txt", std::ios::out); // Abrindo o arquivo

    for (int i = 0; i < 1000; i++) { // Corrigindo a condição do loop
        double r = rand->Gaus(5, 1);
        file << r << endl; // Adicionando uma quebra de linha após cada valor
    }
    file.close(); // Fechando o arquivo

    file.open("data.txt", std::ios::in); // Reabrindo o arquivo em modo de leitura

    double value;
    while (file >> value) { // Extrair os valores
        hist->Fill(value); // Preencher o histograma
    }

    file.close(); // Fechando o arquivo novamente

    hist->GetXaxis()->SetTitle("Distribuição Aleatória"); // Label do eixo x
    hist->GetYaxis()->SetTitle("Entries Axis"); // Label do eixo y

    TCanvas *c1 = new TCanvas(); // Criando o Canvas

    hist->Draw(); // Desenhando o canvas
}