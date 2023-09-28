# HiForestProducerTool

Este repositório hospeda um conjunto de exemplos simples que usam CMSSW EDAnalyzers para extrair informações de Triggers e produzir um arquivo ROOT chamado HiForest a partir de dados públicos de íons pesados ​​​​do CMS que foram tomados no ano de 2011. Aqui terá algumas instruções de como rodar esses códigos e reproduzir a análise do espectro de dois múons.

## Instruções 

### Preparando o Container

Para realizar essa análise, utilizaremos o [Docker container](http://opendata.cern.ch/docs/cms-guide-docker), para isso faça o download do docker que pode ser visto no link anterior e após o download, copie e cole o seguinte comando no terminal do seu computador:

  ```
  docker run --name hi2011_od -it  gitlab-registry.cern.ch/cms-cloud/cmssw-docker/cmssw_4_4_7-slc5_amd64_gcc434:latest /bin/bash
  ```

Feito o download do container siga os seguintes passos:

- Crie um diretório e obtenha o código através do git:

  ```
  mkdir HiForest
  cd HiForest
  git clone -b 2011 https://github.com/cms-opendata-analyses/HiForestProducerTool.git HiForestProducer
  cd HiForestProducer
  ```
  
- Compile os arquivos:

  ```
  scram b
  ```
  
### Rodando o arquivo de configuração 

- Para ver se tudo está funcionando corretamente, mude no arquivo de configuração chamado hiforestanalyzer_cfg.py de `-1` para `10`, se não houver nenhum erro, mude novamente para `-1` para rodar todos os eventos.

- Execute o arquivo de configuração da seguinte forma:

  ```
  cmsRun hiforestanalyzer_cfg.py
  ```

O arquivo de configuração está configurado para ler os arquivos ROOT de input da lista `CMS_HIRun2011_HIDiMuon_RECO_04Mar2013-v1_root_file_index.txt`

Será produzido um arquivo chamado HiForestAOD_DATAtest.root como output.

NOTA: Na primeira vez que você executar o arquivo, demorará muito (dependendo da velocidade da sua conexão) a ponto de parecer que não está fazendo nada. Mas está tudo certo. Talvez seja necessário "separar" o arquivo de input em pequenos arquivos e rodar um por um. Nesse caso, sempre mude o nome do arquivo de saída, caso contrário será sobrescrito no arquivo anterior.

Para juntar todos esses arquivos de saída em um único, execute o seguinte código dentro do container do CMSSW:

```
hadd nome_do_arquivo_final arquivo_1 arquivo_2 ....
```
No final, deve ser criado um novo arquivo chamado nome_do_arquivo_final (mude para qualquer nome que quiser).

Você também pode modificar o arquivo [src/Analyzer.cc](src/Analyzer.cc) para incluir outros objetos como: (tracks, elétrons, etc) no arquivo de output hiforest. As instruções são dadas no próprio arquivo.


### Rodando a análise 

O arquivo [forest2dimuon.C](forest2dimuon/forest2dimuon.C) é um script para analisar o arquivo de saida. Nele é aplicado um trigger "filtro" e é feito uma análise básica de seleção e produção de histogramas de massa invariante.

Para rodar esse arquivo, você precisará do [ROOT](https://root.cern/install/) instalado. Com o ROOT, execute o programa da seguinte forma:
```
root -l forest2dimuon.C
```
Você pode selecionar outros Triggers para a sua análise, basta acessar o arquivo root pelo `TBrowser b` no ROOT e verificar a Tree de Triggers.




