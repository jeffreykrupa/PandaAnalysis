#ifndef MODULE
#define MODULE

#include "TString.h"
#include "vector"
#include "map"

namespace pa {

  class Registry {
    private:
      class BaseContainer { // just for polymorphism
      public:
        BaseContainer() { }
        virtual ~BaseContainer() { }
      }; 
      template <typename T>
      class Container : public BaseContainer {
      public:
        Container(T* ptr_) : ptr(ptr_) { }
        ~Container() { }
        T* ptr;
      };
      template <typename T>
      class ConstContainer : public BaseContainer {
      public:
        ConstContainer(const T* ptr_) : ptr(ptr_) { }
        ~ConstContainer() { }
        const T* ptr;
      };

    public:
      Registry() { }
      ~Registry() { for (auto& iter : _objs) { delete iter.second; } }
      template <typename T>
        void publish(TString name, T* ptr) { _objs[name] = new Container(ptr); }
      template <typename T>
        void publishConst(TString name, const T* ptr) { _objs[name] = new ConstContainer(ptr); }
      template <typename T>
        T* access(TString name) { 
          return dynamic_cast<Container<T>*>(_objs.at(name))->ptr; 
        }
      template <typename T>
        const T* accessConst(TString name) { 
          return dynamic_cast<ConstContainer<T*>>(_objs.at(name))->ptr; 
        }
      bool exists(TString name) { return _objs.find(name) != _objs.end(); }
    private:
      std::map<TString, BaseContainer*> _objs;
  };

  class BaseModule {
    public:
      BaseModule(TString name_): name(name_) { }
      virtual ~BaseModule() { }

    private:
      TString name;
  };

  class ConfigMod : public BaseModule {
    public:
      ConfigMod(Analysis* a_, GeneralTree& gt, int DEBUG_);
      ~ConfigMod() { }

      void readData(TString path);

      const Config& get_config() const { return cfg; }
      const Utils& get_utils() const { return utils; }
      const panda::utils::BranchList get_inputBranches() const { return bl; }

    protected:
      const Analysis& analysis;
      Config cfg;
      Utils utils;

      panda::utils::BranchList bl;

    private:
      void set_inputBranches(); 
      void set_outputBranches(GeneralTree& gt) const;
  };

  class AnalysisMod : public BaseModule {
    public:
      AnalysisMod(TString name, 
                  const panda::EventAnalysis& event_, 
                  const Config& cfg_, 
                  const Utils& utils_,
                  GeneralTree& gt_) : 
        BaseModule(name), 
        event(event_), 
        cfg(cfg_),
        utils(utils_),
        analysis(cfg.analysis),
        gt(gt_) { }
      virtual ~AnalysisMod() { for (auto* m : subMods) delete m; }
      
      // cascading calls to protected functions
      void initialize(Registry& registry);
      void readData(TString path);
      void execute();
      void reset();
      void terminate(); 

    protected:
      panda::EventAnalysis& event;
      const Config& cfg;
      const Utils& utils;
      const Analysis& analysis;
      GeneralTree& gt; 
      std::vector<AnalysisMod*> subMods; // memory management is done by parent

      // here, the module can publish and access data
      virtual void do_init(Registry& registry) { }
      virtual void do_readData(TString path) { }
      // this is where the actual execution is done
      virtual void do_execute() = 0;
      // reset objects between events, if needed
      virtual void do_reset() { }
      virtual void do_terminate() { }
  };
}

#endif
