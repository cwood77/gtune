#include "card.hpp"
#include "lines.hpp"

void schemaBuilder::build(lines& l, cardSchema& s)
{
   schemaBuilder self(s);
   for(auto& lin : l.l)
      if(lin.type == line::kTag)
         self.onTag(lin);
}

void schemaBuilder::onTag(line& l)
{
   bool has = (m_s.tags.find(l.tag)!=m_s.tags.end());
   if(!has)
      m_s.tags[l.tag] = &l;

   if(l.flags & line::kFlagSingleLineTag)
   {
      if(!has)
         m_s.singleLineTags.insert(l.tag);
   }
   else
   {
      m_s.singleLineTags.erase(l.tag);
   }
}

std::ostream& operator<<(std::ostream& o, const cardSchema& s)
{
   o
      << "schema {" << std::endl
   ;

   for(auto it=s.tags.begin();it!=s.tags.end();++it)
   {
      bool isSingleLine = (s.singleLineTags.find(it->first) != s.singleLineTags.end());
      o << "   " << it->first;
      if(isSingleLine)
         o << " [sl]";
      o << std::endl;
   }

   o
      << "}";
   ;

   return o;
}
