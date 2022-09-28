#include "intinput.hpp"
#include <iostream>

void interactivePrompt::prompt()
{
   if(m_first)
   {
      std::cout << "gtune " << __DATE__ << " " << __TIME__ << std::endl;
      m_first = false;
   }

   std::cout << ">";
}

std::string streamInput::getFirstWord()
{
   const char *pThumb = m_curLine.c_str();
   return getNextWord(pThumb);
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
   const char *pStart = pThumb;
   for(;*pThumb!=' '&&*pThumb!=0;++pThumb);
   std::string word(pStart,pThumb-pStart);
   if(pThumb!=0)
      ++pThumb;
   return word;
}
