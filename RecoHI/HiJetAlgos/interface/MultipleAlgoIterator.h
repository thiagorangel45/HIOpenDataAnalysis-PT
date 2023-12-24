#ifndef __MultipleAlgoIterator_h_
#define __MultipleAlgoIterator_h_

#include "RecoJets/JetProducers/interface/PileUpSubtractor.h"

class MultipleAlgoIterator : public PileUpSubtractor {
 public:
    MultipleAlgoIterator(const edm::ParameterSet& iConfig, edm::ConsumesCollector && iC);
    virtual void offsetCorrectJets();
    void rescaleRMS(double s);
    double getEt(const reco::CandidatePtr & in) const;
    double getEta(const reco::CandidatePtr & in) const;
    virtual void calculatePedestal(std::vector<fastjet::PseudoJet> const & coll);
    virtual void subtractPedestal(std::vector<fastjet::PseudoJet> & coll);
    virtual void calculateOrphanInput(std::vector<fastjet::PseudoJet> & orphanInput);

    double minimumTowersFraction_;

    bool sumRecHits_;
    bool dropZeroTowers_;
    ~MultipleAlgoIterator(){;}
    
};


#endif
