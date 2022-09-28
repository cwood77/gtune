#pragma once
#include <list>
#include <ostream>
#include <string>
#include <vector>

class card;

class textTableConfig {
public:
   textTableConfig() : hrules(true) {}

   std::list<std::string> cols;
   bool hrules;
};

class textTableBuilder {
public:
   explicit textTableBuilder(textTableConfig& cfg);

   const textTableConfig& getConfig() const { return m_cfg; }

   void appendRow();
   void appendCell(const std::string& value);

   void format(std::ostream& o);

private:
   void computeRules();
   void format(std::vector<std::string>& row, std::ostream& o);
   std::string format(const std::string& cell, size_t i);

   textTableConfig& m_cfg;
   std::vector<std::vector<std::string> > m_cells;
   std::vector<size_t> m_maxColWidth;
};

class cardTableBuilder {
public:
   explicit cardTableBuilder(textTableBuilder& ttb) : m_ttb(ttb) {}

   void add(card& c);

private:
   textTableBuilder& m_ttb;
};
