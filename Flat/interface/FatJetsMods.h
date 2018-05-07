#ifndef FATJETSMODS
#define FATJETSMODS

#include "Module.h"
#include "JetsMods.h" // need BaseJetMod 

namespace pa {
  class FatJetReclusterMod : public AnalysisMod {
  public: 
    FatJetReclusterMod(panda::EventAnalysis& event_, 
                       Config& cfg_,                 
                       Utils& utils_,                
                       GeneralTree& gt_) :                 
      AnalysisMod("fatjet recluster", event_, cfg_, utils_, gt_) { 
        if (!on())
          return;
        jetDef = new fastjet::JetDefinition(analysis.ak8 ? fastjet::antikt_algorithm : 
                                                           fastjet::cambridge_algorithm,
                                            analysis.ak8 ?  0.8 : 1.5);
      }
    virtual ~FatJetReclusterMod () { 
      delete jetDef;
    }

    virtual bool on() { return analysis.recluster; }
    
  protected:
    void do_init(Registry& registry) {
      fj1 = registry.accessConst<panda::FatJet*>("fj1");
    }
    void do_execute();  
  private:
    panda::FatJet * const * fj1{nullptr}; // pointer to a const pointer 
    fastjet::JetDefinition       *jetDef                {nullptr};
  };


  class FatJetMod : public BaseJetMod {
  public: 
    FatJetMod(panda::EventAnalysis& event_, 
              Config& cfg_,                 
              Utils& utils_,                
              GeneralTree& gt_) :                 
      BaseJetMod("fatjet", event_, cfg_, utils_, gt_),
      fatjets(analysis.ak8 ? event.puppiAK8Jets : event.puppiCA15Jets) { 
        recluster = new FatJetReclusterMod(event_, cfg_, utils_, gt_); subMods.push_back(recluster);
        jetType = "AK8PFPuppi";
      }
    virtual ~FatJetMod () { }

    virtual bool on() { return !analysis.genOnly && analysis.fatjet; }
    
  protected:
    void do_init(Registry& registry) {
      registry.publishConst("fj1", &fj1);
      registry.publishConst("fatjets", &fatjets);
      matchLeps = registry.accessConst<std::vector<panda::Lepton*>>("looseLeps");
      matchPhos = registry.accessConst<std::vector<panda::Photon*>>("tightPhos");
    }
    void do_execute();  
    float getMSDCorr(float,float);
  private:
    void setupJES(); 

    panda::FatJet *fj1{nullptr}; 
    panda::FatJetCollection &fatjets;

    const std::vector<panda::Lepton*>* matchLeps;
    const std::vector<panda::Photon*>* matchPhos;

    FatJetReclusterMod *recluster{nullptr};
  };

  class FatJetMatchingMod : public AnalysisMod {
  public: 
    FatJetMatchingMod(panda::EventAnalysis& event_, 
                      Config& cfg_,                 
                      Utils& utils_,                
                      GeneralTree& gt_) :                 
      AnalysisMod("fatjet matching", event_, cfg_, utils_, gt_) { }
    virtual ~FatJetMatchingMod () { }

    virtual bool on() { return !analysis.genOnly && analysis.fatjet && !analysis.isData; }
    
  protected:
    void do_init(Registry& registry) {
      fjPtr = registry.accessConst<panda::FatJet*>("fj1");
      genP = registry.accessConst<std::vector<panda::Particle*>>("genP");
    }
    void do_execute();  
    void do_reset() { genObjects.clear(); }
  private:
    panda::FatJet * const *fjPtr{nullptr}; // non-const pointer to a const pointer 
    const std::vector<panda::Particle*> *genP{nullptr}; 
    std::map<const panda::GenParticle*,float> genObjects; // gen particle -> pt 
    const panda::GenParticle* matchGen(double eta, double phi, double r2, int pdgid=0) const;  
  };
}

#endif