#ifndef GENERICTREE
#define GENERICTREE 

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TString.h"
#include "TRegexp.h"
#include <vector>
#include "PandaCore/Tools/interface/TreeTools.h"

#define NGENMAX 100

class genericTree {
  public:
    genericTree() {  }
    genericTree(const genericTree&) = delete; 
    genericTree& operator=(const genericTree&) = delete; 
    virtual ~genericTree() {}
    TTree *treePtr{nullptr};
    virtual void WriteTree(TTree *t)=0;
    virtual void ReadTree(TTree* t)=0;
    virtual void RemoveBranches(std::vector<TString> droppable,
                                std::vector<TString> keeppable={}) final;
    virtual void SetAuxTree(TTree *t) {}
    virtual void Reset() { }
    virtual void SetBranchStatus(const char *bname, bool status, UInt_t *ret=0) final { 
      if (treePtr != nullptr) 
        treePtr->SetBranchStatus(bname,status,ret); 
    }
  protected: 
    virtual bool Book(TString bname, void *address, TString leafs) final;
    virtual bool Bind(TString bname, void *address) final;

  private:
    std::vector<TRegexp> r_droppable, r_keeppable;
    TTree *auxTree{nullptr};
};

#endif

