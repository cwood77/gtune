#include "run.hpp"
#include <iostream>
#include <stdexcept>

stateCatalog::~stateCatalog()
{
   for(auto it=m_order.rbegin();it!=m_order.rend();++it)
   {
      std::cout << "[state] deleting '" << *it << "'" << std::endl;
      auto *pPtr = m_cat[*it];
      delete pPtr;
      m_cat.erase(*it);
   }
   if(m_cat.size())
      std::cout << "WARNING! ISE in state?" << std::endl;

   for(auto it=m_df.begin();it!=m_df.end();++it)
      delete it->second;
}

void stateCatalog::_replace(const std::string& key, iState& value)
{
   iState*& pOld = m_cat[key];
   if(!pOld)
   {
      pOld = &value;
      m_order.push_back(key);
   }
   else
   {
      std::cout << "[warning] replacing value for '" << key << "'" << std::endl;
      delete pOld;
      pOld = &value;
   }
}

iState *stateCatalog::_fetch(const std::string& key)
{
   auto it = m_cat.find(key);
   if(it == m_cat.end())
   {
      // miss; try to get a default
      auto *pDF = m_df[key];
      if(pDF)
      {
         pDF->createDefault(*this);
         it = m_cat.find(key);
      }
   }

   if(it == m_cat.end())
      return NULL;
   else
      return it->second;
}

void stateCatalog::_registerDefault(const std::string& key, iDefaultStateFactory& f)
{
   auto*& pOld = m_df[key];
   if(pOld != NULL)
      delete pOld;
   pOld = &f;
}

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
