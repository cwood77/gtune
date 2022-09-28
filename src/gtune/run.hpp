#pragma once
#include <list>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// state --------------------------------------------------------

class iState {
public:
   virtual ~iState() {}
};

template<class T>
class stateWrapper : public iState {
public:
   stateWrapper(T& s) : pPtr(&s) {}
   std::unique_ptr<T> pPtr;
};

class iStateCatalog;

class iDefaultStateFactory {
public:
   virtual ~iDefaultStateFactory() {}
   virtual void createDefault(iStateCatalog& s) = 0;
};

class iStateCatalog {
public:
   template<class T> void publish(T& s)
   { _replace(typeid(T).name(),*new stateWrapper<T>(s)); }

   template<class T> T& demand()
   {
      iState *pS = _fetch(typeid(T).name());
      if(!pS)
         throw std::runtime_error("state not found");
      auto *_pS = dynamic_cast<stateWrapper<T>*>(pS);
      if(!_pS)
         throw std::runtime_error("state of wrong type");
      return *_pS->pPtr.get();
   }

   template<class T> void registerDefault(iDefaultStateFactory& f)
   { _registerDefault(typeid(T).name(),f); }

protected:
   virtual void _replace(const std::string& key, iState& value) = 0;
   virtual iState *_fetch(const std::string& key) = 0;
   virtual void _registerDefault(const std::string& key, iDefaultStateFactory& f) = 0;
};

class stateCatalog : public iStateCatalog {
public:
   ~stateCatalog();

protected:
   virtual void _replace(const std::string& key, iState& value);
   virtual iState *_fetch(const std::string& key);
   virtual void _registerDefault(const std::string& key, iDefaultStateFactory& f);

private:
   std::map<std::string,iState*> m_cat;
   std::list<std::string> m_order;
   std::map<std::string,iDefaultStateFactory*> m_df;
};

// input --------------------------------------------------------

class iInput {
public:
   virtual std::string getFirstWord() = 0;

   virtual std::string getRestOfLine() = 0;
   virtual std::vector<std::string> getRemainingWords() = 0;

   virtual bool nextLine() = 0;
};

// commands --------------------------------------------------------

class iCommand {
public:
   virtual ~iCommand() {}
   virtual void run() = 0;
};

class iCommandInfo {
public:
   virtual bool isMatch(const std::string& word) const = 0;
   virtual iCommand *create(iInput& i, iStateCatalog& s) = 0;
};

class haltException : public std::exception {};

class commandRegistry {
public:
   static commandRegistry& get();

   void registerCommand(iCommandInfo& c);
   iCommand *create(iInput& i, iStateCatalog& s);

private:
   std::list<iCommandInfo*> m_cis;
};

template<class T>
class autoCommandInfo : public iCommandInfo {
public:
   explicit autoCommandInfo(const std::string& word) : m_word(word)
   {
      commandRegistry::get().registerCommand(*this);
   }

   virtual bool isMatch(const std::string& word) const
   {
      return word == m_word;
   }

   virtual iCommand *create(iInput& i, iStateCatalog& s)
   {
      return new T(i,s);
   }

private:
   std::string m_word;
};
