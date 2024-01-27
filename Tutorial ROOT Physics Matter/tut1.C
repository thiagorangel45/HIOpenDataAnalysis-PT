void tut1()
{
	
	TH1F *hist = new TH1F("hist","Histogram", 100, 0, 100); // criando histograma
	hist->Fill(10);
	hist->Fill(90);
	hist->GetXaxis()->SetTitle("X axis"); // Label do eixo x
	hist->GetYaxis()->SetTitle("Y axis"); // label do eixo y
	TCanvas *c1 = new TCanvas(); // criando o Canvas
	hist->Draw();	// desenhando o canvas
}
