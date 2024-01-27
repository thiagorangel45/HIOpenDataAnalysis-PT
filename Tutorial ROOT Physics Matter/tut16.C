void tut16()
{
    // Criação de uma nova tela (canvas)
    TCanvas *c1 = new TCanvas();

    // Abertura do arquivo ROOT
    TFile *input = new TFile("data4.root", "READ");

    // Obtém a árvore (tree) do arquivo
    TTree *tree = (TTree*)input->Get("tree");

    // Variáveis para armazenar os valores dos ramos (branches) da árvore
    double x, y;

    // Associa as variáveis aos ramos correspondentes
    tree->SetBranchAddress("x", &x);
    tree->SetBranchAddress("y", &y);

    // Obtém o número de entradas na árvore
    int entries = tree->GetEntries();
    cout << "Número de entradas: " << entries << endl;

    TH1F *hist = new TH1F("hist","histogram", 30, 0, 30);

    // Loop sobre as entradas da árvore
    for (int i = 0; i < entries; i++)
    {
        // Obtém a entrada atual
        tree->GetEntry(i);

        // Imprime os valores de x e y
        cout << "Entrada " << i << ": x = " << x << ", y = " << y << endl;
        hist->Fill(x);
    }
    hist->Draw();
}
