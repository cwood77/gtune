#include "card.hpp"
#include "lines.hpp"
#include "textTable.hpp"
#include <sstream>

static bool isLastChar(const char *string, size_t slen, char c)
{
   return slen >= 1 && string[slen-1] == c;
}

const std::string& card::getField(const std::string& tag) const
{
   auto it = m_fieldCache.find(tag);
   if(it == m_fieldCache.end())
   {
      // compute value and stash it for later computes
      std::stringstream stream;

      auto tit = tags.find(tag);
      if(tit == tags.end())
         // this card doesn't have this tag
         stream << "<?>";
      else
      {
         // sweep over subsequent lines until the next tag
         line *pTagLine = tit->second;
         lines& L = *pTagLine->pOwner;
         size_t idx = pTagLine->index+1;
         for(;idx < L.l.size();idx++)
         {
            auto& l = L.l[idx];
            if(l.type != line::kTag)
            {
               if(!stream.str().empty())
                  stream << std::endl;
               stream << l.text;
            }
            else
               break;
         }
      }

      // post-process: shave off trailing newline
      std::string ans = stream.str();
      size_t ansLen = ans.length();
      if(isLastChar(ans.c_str(),ansLen,'\n'))
         ansLen--;
      if(isLastChar(ans.c_str(),ansLen,'\r'))
         ansLen--;
      ans = std::string(ans.c_str(),ansLen);

      m_fieldCache[tag] = ans;
      it = m_fieldCache.find(tag);
   }
   return it->second;
}

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

   auto& cnt = m_s.tagCounts[l.tag];
   cnt++;
   if(cnt > m_s.maxTagCount)
      m_s.maxTagCount = cnt;
}

std::ostream& operator<<(std::ostream& o, const cardSchema& s)
{
   o
      << "schema {" << std::endl
   ;

   for(auto it=s.tags.begin();it!=s.tags.end();++it)
   {
      bool isSingleLine = (s.singleLineTags.find(it->first) != s.singleLineTags.end());
      bool isSparse = (s.tagCounts.find(it->first)->second != s.maxTagCount);

      o << "   " << it->first;
      if(isSingleLine)
         o << " [single-line]";
      if(isSparse)
         o << " [sparse]";
      o << std::endl;
   }

   if(s.projected.size())
   {
      o << "+" << s.projected.size() << " projected field(s)" << std::endl;
      for(auto f : s.projected)
         o << "   " << f << std::endl;
   }

   o
      << "}";
   ;

   return o;
}

void cardBuilder::build(lines& l, cards& c)
{
   cardBuilder self(c);
   for(auto& lin : l.l)
      if(lin.type == line::kTag)
         self.onTag(lin);
}

void cardBuilder::onTag(line& l)
{
   if(m_pCurrCard == NULL)
   {
      // no active card... create one
      m_c.c.push_back(card());
      m_pCurrCard = &m_c.c.back();
      m_pCurrCard->tags[l.tag] = &l;
   }
   else
   {
      bool alreadyHas = (m_pCurrCard->tags.find(l.tag) != m_pCurrCard->tags.end());
      if(alreadyHas)
      {
         // time for new card
         m_pCurrCard = NULL;
         onTag(l);
      }
      else
      {
         // add new tag to existing card
         m_pCurrCard->tags[l.tag] = &l;
      }
   }
}

bool cardSortCriteria::isLess(const card *pLhs, const card *pRhs) const
{
   for(auto f : fields)
   {
      auto& lhsF = (*pLhs)[f];
      auto& rhsF = (*pRhs)[f];
      if(lhsF != rhsF)
         return lhsF < rhsF;
   }
   return pLhs < pRhs;
}

void cardSet::fill(cards& C)
{
   // recreate set b/c it copies the criteria
   s = std::set<card*,cardSortCriteria>(c);

   for(auto& c : C.c)
      s.insert(&c);
}

void histogram::add(card& c)
{
   for(auto tag : tags)
      m_values[c[tag]]++;
}

void histogram::format(std::ostream& s)
{
   buildRevValues();

   textTableConfig ttc;
   ttc.cols.push_back("#");
   ttc.cols.push_back("value");
   textTableBuilder ttb(ttc);

   for(auto it=m_revValues.rbegin();it!=m_revValues.rend();++it)
   {
      std::stringstream cnt;
      cnt << it->first;

      auto& V = it->second;
      for(auto v : V)
      {
         ttb.appendRow();
         ttb.appendCell(cnt.str());
         ttb.appendCell(v);
      }
   }

   ttb.format(s);
}

void histogram::buildRevValues()
{
   m_revValues.clear();

   for(auto it=m_values.begin();it!=m_values.end();++it)
      m_revValues[it->second].insert(it->first);
}
