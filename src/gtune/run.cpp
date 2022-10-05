#include "run.hpp"
#include <iostream>
#include <stdexcept>

stateCatalog::~stateCatalog()
{
   for(auto it=m_order.rbegin();it!=m_order.rend();++it)
   {
      auto *pPtr = m_cat[*it];
      if(pPtr)
      {
         std::cout << "[state] deleting '" << *it << "'" << std::endl;
         delete pPtr;
      }
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
      erase(key);
      m_cat[key] = &value;
   }
}

void stateCatalog::erase(const std::string& key)
{
   auto it = m_cat.find(key);
   if(it == m_cat.end())
      return;

   std::cout << "[warning] erasing existing value for '" << key << "'" << std::endl;
   delete it->second;
   m_cat.erase(it);

   _invalidateDeps(key);
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

void stateCatalog::_setDep(const std::string& obj, const std::string& dep)
{
   m_invalidMap[dep].insert(obj);
}

void stateCatalog::_registerDefault(const std::string& key, iDefaultStateFactory& f)
{
   auto*& pOld = m_df[key];
   if(pOld != NULL)
      delete pOld;
   pOld = &f;
}

void stateCatalog::_invalidateDeps(const std::string& key)
{
   auto inv = m_invalidMap[key];
   for(auto dep : inv)
      erase(dep);
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
