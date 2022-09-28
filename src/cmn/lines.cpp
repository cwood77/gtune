#include "lines.hpp"
#include <cstring>
#include <iostream>

void lineReader::load(std::istream& s, lines& l)
{
   while(s.good())
   {
      std::string sline;
      std::getline(s,sline);

      l.l.push_back(line());
      auto& line = l.l.back();
      line.type = line::kUnclassified;
      line.text = sline;
   }
}

void lineClassifier::run(lines& l)
{
   lineClassifier self;
   self._run(l);
}

void lineClassifier::_run(lines& l)
{
   for(auto& line : l.l)
      run(line);

   closeTagIf();
}

void lineClassifier::run(line& l)
{
   bool isTag = tryParseTag(l.text,l.tag);
   if(isTag)
   {
      l.type = line::kTag;
      closeTagIf();
      openTag(l);
   }
   else
   {
      if(l.text.empty())
         l.type = line::kBlankLine;
      else if(m_pLastTag)
      {
         l.type = line::kPayload;
         m_numPayloadLines++;
      }
      else
         l.type = line::kPreamble;
   }

   std::cout << "line: '" << l.text << "' -> " << l.type << std::endl;
}

void lineClassifier::openTag(line& l)
{
   m_pLastTag = &l;
}

void lineClassifier::closeTagIf()
{
   if(!m_pLastTag) return;

   if(m_numPayloadLines == 0 || m_numPayloadLines == 1)
      m_pLastTag->flags |= line::kFlagSingleLineTag;

   m_pLastTag = NULL;
   m_numPayloadLines = 0;
}

bool lineClassifier::tryParseTag(const std::string& text, std::string& tag)
{
   if(::strncmp(text.c_str(),"{{",2)!=0)
      return false;
   if(text.length() < 2 || ::strncmp(text.c_str()+text.length()-2,"}}",2)!=0)
      return false;

   tag = std::string(text.c_str()+2,text.length()-4);

   std::cout << "tagparse: " << text << " => " << tag << std::endl;

   return true;
}
