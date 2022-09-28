#pragma once
#include <list>
#include <map>
#include <set>
#include <string>

class line;
class lines;

class cardSchema {
public:
   std::map<std::string,line*> tags;
   std::set<std::string> singleLineTags;
};

std::ostream& operator<<(std::ostream& o, const cardSchema& s);

class card {
public:
   std::map<std::string,line*> tags;
};

class cards {
public:
   cardSchema s;
   std::list<card> c;
};

class schemaBuilder {
public:
   static void build(lines& l, cardSchema& s);

private:
   explicit schemaBuilder(cardSchema& s) : m_s(s) {}

   void onTag(line& l);

   cardSchema& m_s;
};

class cardBuilder {
public:
   static void build(lines& l, cards& c);

private:
   cardBuilder(cards& c) : m_c(c), m_pCurrCard(NULL) {}

   void onTag(line& l);

   cards& m_c;
   card *m_pCurrCard;
};

// class cardSortCriteria {
// };
// 
// class cardSet {
// public:
//    std::set<
// };
