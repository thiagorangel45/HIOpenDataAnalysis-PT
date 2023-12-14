## Instructions

### Preparing the container:

We will use a Docker container to perform this analysis. You can download Docker from the following link: [Docker container](https://www.docker.com/products/docker-desktop/). After downloading, open your terminal and paste the following code: 

  ```
  docker run -it --name hi2013_od -P -p 5901:5901 -p 6080:6080 -v ${HOME}/hi2013_od:/code/hi2013_od gitlab-registry.cern.ch/cms-cloud/cmssw-docker-opendata/cmssw_5_3_20-slc6_amd64_gcc472 /bin/bash
  ```

Once the container is downloaded, you will see the folders in this repository. When you go into the 'test' folder: [test](HeavyIonsAnalysis/JetAnalysis/test) you will find some Python scripts. However, we will download another script using the following command (actually, it's not a different script; it's just a simple modification of `runForest_pPb_Data_53X.py`:

```
wget https://raw.githubusercontent.com/cms-opendata-validation/HeavyIonDataValidation/53X/runForest_pPb_DATA_53X_OD.py

```

### Running the script:

Running the python script:

```
cmsRun runForest_pPb_DATA_53X_OD.py
```
  
You will receive some error and processing messages, but it will work and produce a ROOT file named 'HiForest.root.' This ROOT file is available in this repository inside the [test](HeavyIonsAnalysis/JetAnalysis/test) folder, and you can download only the ROOT file if you prefer to check the trees and branches. Everything should work fine for jets up to this point.

### Problem with the Muon Tree

Problem with the Muon Tree:
As our analysis focuses on the muon channel, we attempted to include the muon tree by uncommenting line 234 in the `runForest_pPb_DATA_53X_OD.py` file and rerunning the code as before. However, we encountered an issue where the muon tree remains empty, while the other trees continue to function correctly.

The ROOT output is available in [HiForest.root](HeavyIonsAnalysis). You can download and open it with the `TBrowser` in ROOT to check.















