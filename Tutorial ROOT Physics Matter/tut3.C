void tut3()
{
	TH1F *hist = new TH1F("hist","Histogram", 10, 0, 10); // criando histograma
	fstream file; // usando fstream do c++
	file.open("data.txt", ios::in); // abrindo o arquivo 
	double value; // definindo a variavel "value"
	while(1){
		file >> value; // extrair os valores
		hist->Fill(value); // preencher o histograma
		if(file.eof()) break; // parar o loop ao atingir o fim do arquivo
	}
	hist->GetXaxis()->SetTitle("Grade axis"); // Label do eixo x
	hist->GetYaxis()->SetTitle("Entries axis"); // label do eixo y
	TCanvas *c1 = new TCanvas(); // criando o Canvas
	hist->Draw();	// desenhando o canvas
}
