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
   lastRow.push_back(value);
   size_t& maxWidth = m_maxColWidth[lastRow.size()-1];
   maxWidth = maxWidth > value.length() ? maxWidth : value.length();
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

void cardTableBuilder::add(card& c)
{
   m_ttb.appendRow();
   for(auto col : m_ttb.getConfig().cols)
      m_ttb.appendCell(c[col]);
}
