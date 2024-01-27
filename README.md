# HiForestProducerTool

This repository hosts a collection of simple examples that use CMSSW EDAnalyzers to extract information from triggers and produce a ROOT file named HiForest from public heavy-ion CMS data taken in the year 2010. Here, you will find instructions on how to run these codes and reproduce the analysis of the two-muon spectrum.

## Instructions:

### Preparing the container

To perform this analysis, we will use the Docker container. To do this, download the [Docker](https://docs.docker.com/engine/install/) as indicated in the previous link. After the download, copy and paste the following command into your computer's terminal:

  ```
  docker run --name hi2010_od -it  gitlab-registry.cern.ch/cms-cloud/cmssw-docker/cmssw_3_9_2_patch5-slc5_amd64_gcc434:latest /bin/bash
  ```

After downloading the container, follow these steps:

- Create a directory and obtain the code using git:

  ```
  mkdir HiForest
  cd HiForest
  git clone -b 2010 https://github.com/thiagorangel45/HIOpenDataAnalysis.git HiForestProducer
  cd HiForestProducer
  ```
  
- Compile the files:

  ```
  scram b
  ```
  
### Running the configuration file 

- This configuration file is set to run only `100` events, which is intended to check if the code is running correctly. If there are no error outputs, and the ROOT output file is generated correctly, change `100` to `-1` to run all events from the input file.

- Execute the configuration file as follows:

  ```
  cmsRun hiforestanalyzer_cfg.py
  ```

The configuration file is set to read the input ROOT files from the list `CMS_HIRun2010_HIAllPhysics_ZS-v2_RECO_file_index.txt`.

An output file named HiForestAOD_DATAtest.root will be generated.

NOTE: The first time you run the file, it may take a while (depending on the speed of your connection) to the point that it seems like nothing is happening. But everything is fine. It might be necessary to "split" the input file into smaller files and run them one by one. In this case, always change the name of the output file; otherwise, it will overwrite the previous file.

To merge all these output files into a single one, execute the following code within the CMSSW container:

```
hadd nome_do_arquivo_final arquivo_1 arquivo_2 ....
```
In the end, a new file named `nome_do_arquivo_final` (change it to any name you prefer) should be created.

You can also modify the file [src/Analyzer.cc](src/Analyzer.cc) to include other objects such as tracks, electrons, etc., in the hiforest output file. Instructions for this are provided within the file itself.


### Running the analysis.

The file [forest2dimuon.C](forest2dimuon.C) is a script for analyzing the output file. It applies a trigger "filter" and performs a basic selection analysis, producing histograms of the invariant mass. In the [forest2dimuon](forest2dimuon) folder, you can see some modifications to the original file and the generated plots, one of this modifications is named forest2dimuon_2011PbPb_mass.C.


To run this file, you will need [ROOT](https://root.cern/install/) installed. With ROOT installed, execute the program in the terminal as follows:

```
root -l forest2dimuon_2010PbPb_mass.C
```
and produce a plot like this: 

<p align="center">
  <img src="forest2dimuon/diMuon_mass_2010_PbPb_1.png" alt="Texto Alternativo" width="700">
</p>

You can select other triggers for your analysis by accessing the ROOT file using `TBrowser b` in ROOT and checking the Trigger Tree.




