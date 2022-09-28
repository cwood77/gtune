#include "run.hpp"
#include <iostream>

class loadCommand : public iCommand {
public:
   loadCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      std::cout << "loading stuff" << std::endl;

      auto path = m_i.getRestOfLine();
      std::cout << "loading '" << path << "'" << std::endl;
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

class loadCommandInfo : public iCommandInfo {
public:
   loadCommandInfo()
   {
      commandRegistry::get().registerCommand(*this);
   }

   virtual bool isMatch(const std::string& word) const
   {
      return word == "load";
   }

   virtual iCommand *create(iInput& i, iStateCatalog& s)
   {
      return new loadCommand(i,s);
   }
} gLoadCmd;
