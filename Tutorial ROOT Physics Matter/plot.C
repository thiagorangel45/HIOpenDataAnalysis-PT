void plot()
{
    TFile *file = new TFile("output.root", "READ"); // Fixed variable name
    TH1F *hist = (TH1F*)file->Get("hist"); // Fixed variable name and casting
    hist->Draw();  
}
