#include "run.hpp"
#include <stdexcept>

commandRegistry& commandRegistry::get()
{
   static commandRegistry the;
   return the;
}

void commandRegistry::registerCommand(iCommandInfo& c)
{
   m_cis.push_back(&c);
}

iCommand *commandRegistry::create(iInput& i, iStateCatalog& s)
{
   for(auto *pCi : m_cis)
      if(pCi->isMatch(i.getFirstWord()))
         return pCi->create(i,s);

   throw std::runtime_error("command not understood");
}
