# HiForestProducerTool

Este repositório hospeda um conjunto de exemplos simples que usam CMSSW EDAnalyzers para extrair informações de Triggers e produzir um arquivo ROOT chamado HiForest a partir de dados públicos de íons pesados do CMS que foram coletados no ano de 2010. Aqui, você encontrará algumas instruções sobre como executar esses códigos e reproduzir a análise do espectro de dois múons.

## Instruções 

### Preparando o Container

Para realizar essa análise, utilizaremos o [Docker container](http://opendata.cern.ch/docs/cms-guide-docker). Para isso, faça o download do Docker, que pode ser encontrado no link anterior. Após o download, copie e cole o seguinte comando no terminal do seu computador:

  ```
  docker run --name hi2010_od -it  gitlab-registry.cern.ch/cms-cloud/cmssw-docker/cmssw_3_9_2_patch5-slc5_amd64_gcc434:latest /bin/bash
  ```

Após o download do container, siga os seguintes passos:

- Crie um diretório e obtenha o código através do git:

  ```
  mkdir HiForest
  cd HiForest
  git clone -b 2010 https://github.com/cms-opendata-analyses/HiForestProducerTool.git HiForestProducer
  cd HiForestProducer
  ```

- Compile os arquivos:

  ```
  scram b
  ```
  
### Rodando o arquivo de configuração 

-para verificar se tudo está funcionando corretamente, altere o valor no arquivo de configuração chamado "hiforestanalyzer_cfg.py" de -1 para 10. Se tudo funcionar corretamente, altere-o novamente para -1 para executar todos os eventos.

-Execute o arquivo de configuração da seguinte maneira:

  ```
  cmsRun hiforestanalyzer_cfg.py
  ```

O arquivo de configuração está configurado para ler os arquivos ROOT de input da lista CMS_HIRun2010_HIAllPhysics_ZS-v2_RECO_file_index.txt.Será gerado um arquivo chamado HiForestAOD_DATAtest.root como output.

NOTA: Na primeira vez que você executar o trabalho, pode demorar bastante (dependendo da velocidade da sua conexão) a ponto de parecer que não está fazendo nada. Mas está tudo correto.

Você pode modificar o arquivo [src/Analyzer.cc](src/Analyzer.cc) para incluir outros objetos, como trilhas (tracks), elétrons, etc., no arquivo de output HiForest. As instruções estão disponíveis no próprio arquivo.

### Rodando a análise 

O arquivo [forest2dimuon.C](forest2dimuon.C) é um script para analisar o arquivo de saída. Nele, é aplicado um filtro de trigger e realizada uma análise básica de seleção, resultando na produção de histogramas de massa invariante.

Você pode ver também algumas variações desse arquivo na pasta hi2010.

Para executar este arquivo, é necessário ter o  [ROOT](https://root.cern/install/) instalado. Com o ROOT instalado, execute o arquivo da seguinte maneira:

```
root -l forest2dimuon.C
```
Você pode selecionar outros Triggers para a sua análise. Basta acessar o arquivo root usando o comando TBrowser b no ROOT e verificar a Tree de Triggers.




