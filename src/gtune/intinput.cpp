#include "intinput.hpp"
#include <iostream>

void interactivePrompt::prompt()
{
   if(m_first)
   {
      std::cout << "gtune " << __DATE__ << " " << __TIME__ << std::endl;
      m_first = false;
   }

   std::cout << "gT>";
}

std::string streamInput::getFirstWord()
{
   const char *pThumb = m_curLine.c_str();
   return getNextWord(pThumb);
}

std::string streamInput::getRestOfLine()
{
   const char *pThumb = m_curLine.c_str();
   getNextWord(pThumb);
   return pThumb;
}

std::vector<std::string> streamInput::getRemainingWords()
{
   std::vector<std::string> v;
   const char *pThumb = m_curLine.c_str();
   bool first = true;
   while(true)
   {
      auto word = getNextWord(pThumb);
      if(first)
         first = false;
      else
         v.push_back(word);
      if(*pThumb == 0)
         break;
   }
   return v;
}

bool streamInput::nextLine()
{
   if(!m_s.good())
      return false;

   m_p.prompt();
   std::getline(m_s,m_curLine);

   return true;
}

std::string streamInput::getNextWord(const char *&pThumb)
{
   bool isQuoted = (*pThumb == '"');
   if(isQuoted)
      pThumb++;

   const char *pStart = pThumb;
   for(;*pThumb!=0;++pThumb)
   {
      if(isQuoted && *pThumb == '"')
         break;
      else if(!isQuoted && *pThumb == ' ')
         break;
   }

   std::string word(pStart,pThumb-pStart);

   if(*pThumb=='"')
      ++pThumb;
   if(*pThumb!=0)
      ++pThumb;

   return word;
}
