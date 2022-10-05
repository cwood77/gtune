#include "card.hpp"
#include "textTable.hpp"

textTableBuilder::textTableBuilder(textTableConfig& cfg)
: m_cfg(cfg)
{
   m_maxColWidth.resize(cfg.cols.size());

   // create the header row
   m_cells.push_back(std::vector<std::string>());
   for(auto s : m_cfg.cols)
      appendCell(s);

   // create rule row
   if(m_cfg.hrules)
      m_cells.push_back(std::vector<std::string>());
}

void textTableBuilder::appendRow()
{
   m_cells.push_back(std::vector<std::string>());
}

void textTableBuilder::appendCell(const std::string& value)
{
   auto& lastRow = m_cells.back();

   // determine how big the value can be
   size_t truncWidth = value.length();
   auto wit = m_cfg.maxWidth.find(lastRow.size());
   if(wit!=m_cfg.maxWidth.end())
      truncWidth = wit->second;

   // truncate the value if necessary
   std::string truncValue = value;
   if(truncValue.length() > truncWidth)
   {
      truncValue = std::string(value.c_str(),truncWidth);
      truncValue += "...";
   }

   // add it
   lastRow.push_back(truncValue);

   // update maxWidth
   size_t& maxWidth = m_maxColWidth[lastRow.size()-1];
   maxWidth = maxWidth > truncValue.length() ? maxWidth : truncValue.length();
}

void textTableBuilder::format(std::ostream& o)
{
   if(m_cfg.hrules)
      computeRules();

   for(auto rit=m_cells.begin();rit!=m_cells.end();++rit)
   {
      if(rit!=m_cells.begin())
         o << std::endl;
      format(*rit,o);
   }
}

void textTableBuilder::computeRules()
{
   for(size_t x : m_maxColWidth)
      m_cells[1].push_back(std::string(x,'-'));
}

void textTableBuilder::format(std::vector<std::string>& row, std::ostream& o)
{
   for(size_t i=0;i<row.size();i++)
      o << format(row[i],i);
}

std::string textTableBuilder::format(const std::string& cell, size_t i)
{
   std::string rowSpacer(i > 0 ? 1 : 0,' ');
   std::string pad(m_maxColWidth[i] - cell.length(),' ');
   return rowSpacer + cell + pad;
}

void cardTableBuilder::autoConfigure(textTableConfig& ttc, cardSchema& s, cardSortCriteria& c)
{
   std::set<std::string> done;

   for(auto& f : c.fields)
   {
      ttc.cols.push_back(f);
      done.insert(f);
   }

   for(auto tag : s.tagsInOrder)
   {
      if(done.find(tag)!=done.end())
         continue; // already present

      if(!s.isSingleLine(tag))
         continue; // omit multiline by default

      const char *pThumb = tag.c_str();
      for(;*pThumb=='-'||*pThumb=='=';++pThumb);
      if(*pThumb==0) continue; // skip hrule fields

      ttc.cols.push_back(tag);
      ttc.maxWidth[ttc.cols.size()-1] = 10; // guess at a max-width
   }
}

void cardTableBuilder::add(card& c)
{
   m_ttb.appendRow();
   for(auto col : m_ttb.getConfig().cols)
      m_ttb.appendCell(c[col]);
}
