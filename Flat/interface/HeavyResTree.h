// THIS FILE IS AUTOGENERATED //
#ifndef HeavyResTree_H
#define HeavyResTree_H
// STARTCUSTOM HEADER

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TString.h"
#include "genericTree.h"
#include <map>

// ENDCUSTOM
class HeavyResTree : public genericTree {
  public:
    HeavyResTree();
    ~HeavyResTree();
    void WriteTree(TTree* t);
    void Fill() { treePtr->Fill(); }
    void Reset();
// STARTCUSTOM PUBLIC
    const std::vector<double>& get_betas() const { return betas; }
    const std::vector<int>& get_ibetas() const { return ibetas; }
    const std::vector<int>& get_Ns() const { return Ns; }
    const std::vector<int>& get_orders() const { return orders; }

    // public objects
    struct ECFParams {
      int ibeta;
      int N;
      int order;
      bool operator==(const ECFParams &o) const {
        return ibeta==o.ibeta && N==o.N && order==o.order;
      }
      bool operator<(const ECFParams &o) const {
        return ( N<o.N ||
                (N==o.N && order<o.order) ||
                (N==o.N && order==o.order && ibeta<o.ibeta) );
      }
      bool operator>(const ECFParams &o) const {
        return ! operator<(o);
      }
    };
    std::map<ECFParams,float> clf_ECFNs;
    virtual void SetAuxTree(TTree *t);
// ENDCUSTOM
  private:
// STARTCUSTOM PRIVATE
    const std::vector<double> betas = {0.5, 1.0, 2.0, 4.0};
    const std::vector<int> ibetas = {0,1,2,3};
    const std::vector<int> Ns = {1,2,3,4};
    const std::vector<int> orders = {1,2,3};
    std::vector<ECFParams> ecfParams;
    TString makeECFString(ECFParams p) {
      return TString::Format("ECFN_%i_%i_%.2i",p.order,p.N,int(10*betas.at(p.ibeta)));
    }
// ENDCUSTOM
  public:
  float recoil;
  int runNumber;
  int lumiNumber;
  ULong64_t i_evt;
  int npv;
  float rho;
  float mcWeight;
  int sampleType;
  float gen_pt;
  float gen_eta;
  float gen_phi;
  float gen_size;
  int gen_pdgid;
  float clf_Tau32;
  float clf_Tau21;
  float clf_Tau32SD;
  float clf_Tau21SD;
  float clf_MSD;
  float clf_MSD_corr;
  float clf_Pt;
  float clf_Phi;
  float clf_Eta;
  float clf_M;
  float clf_MaxCSV;
  int clf_IsMatched;
  float clf_HTTFRec;
};
#endif