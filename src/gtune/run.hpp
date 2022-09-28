#pragma once
#include <list>
#include <memory>
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
   std::unique_ptr<T> pPtr;
};

class iStateCatalog;

class iDefaultStateFactory {
public:
   virtual void createDefault(iStateCatalog& s) = 0;
};

class iStateCatalog {
public:
   template<class T> void publish(T& s);
   template<class T> T& demand();

   template<class T> void registerDefault(iDefaultStateFactory& f);

protected:
   virtual void _publish(const std::string& key, iState& pValue) = 0;
   virtual iState *_fetch(const std::string& key) = 0;
   virtual void _registerDefault(const std::string& key, iDefaultStateFactory& f) = 0;
};

class stateCatalog : public iStateCatalog {
protected:
   virtual void _publish(const std::string& key, iState& pValue) { throw __LINE__; }
   virtual iState *_fetch(const std::string& key) { throw __LINE__; }
   virtual void _registerDefault(const std::string& key, iDefaultStateFactory& f) { throw __LINE__; }
};

// input --------------------------------------------------------

class iInput {
public:
   virtual std::string getFirstWord() = 0;

   virtual const std::string& getRestOfLine() = 0;
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
