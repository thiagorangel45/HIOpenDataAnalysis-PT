### Preparando o contêiner:

Vamos usar um contêiner Docker para realizar esta análise. Você pode baixar o Docker a partir do seguinte link: [Docker container](https://www.docker.com/products/docker-desktop/). Após o download, abra seu terminal e cole o seguinte código: 

  ```
  docker run -it --name hi2013_od -P -p 5901:5901 -p 6080:6080 -v ${HOME}/hi2013_od:/code/hi2013_od gitlab-registry.cern.ch/cms-cloud/cmssw-docker-opendata/cmssw_5_3_20-slc6_amd64_gcc472 /bin/bash
  ```

Depois de baixar o contêiner, você verá as pastas neste repositório. Ao entrar na pasta [test](HeavyIonsAnalysis/JetAnalysis/test), você encontrará alguns scripts em Python. No entanto, baixaremos outro script usando o seguinte comando (na verdade, não é um script diferente; é apenas uma modificação simples do `runForest_pPb_Data_53X.py`:

```
wget https://raw.githubusercontent.com/cms-opendata-validation/HeavyIonDataValidation/53X/runForest_pPb_DATA_53X_OD.py

```

### Executando o script:
```
cmsRun runForest_pPb_DATA_53X_OD.py
```
  
Você receberá mensagens de erro e processamento, mas funcionará e produzirá um arquivo ROOT chamado 'HiForest.root'. Este arquivo ROOT está disponível neste repositório dentro da pasta [test](HeavyIonsAnalysis/JetAnalysis/test) e você pode baixar apenas o arquivo ROOT se preferir verificar as árvores e ramos. Tudo deve funcionar bem para jatos até este ponto.

### Problema com a Árvore dos Múons

Problema com a Árvore de Múons:
Como nossa análise se concentra no canal de múons, tentamos incluir a árvore de múons descomentando a linha 234 no arquivo `runForest_pPb_DATA_53X_OD.py` e executando o código como antes. No entanto, encontramos um problema em que a árvore de múons permanece vazia, enquanto as outras árvores continuam a funcionar corretamente.

A saída é um arquivo ROOT que está disponível em [HiForest.root](HeavyIonsAnalysis). Você pode baixar e abrir com o TBrowser no ROOT para verificar.
