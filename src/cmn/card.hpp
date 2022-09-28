#pragma once
#include <list>
#include <map>
#include <set>
#include <string>

class line;
class lines;

class cardSchema {
public:
   cardSchema() : maxTagCount(0) {}

   std::map<std::string,line*> tags;
   std::set<std::string> singleLineTags;
   std::map<std::string,size_t> tagCounts;
   size_t maxTagCount;
   std::set<std::string> projected;
};

std::ostream& operator<<(std::ostream& o, const cardSchema& s);

class card {
public:
   std::map<std::string,line*> tags;

   const std::string& getField(const std::string& tag) const;
   void setField(const std::string& tag, const std::string& value)
   { m_fieldCache[tag] = value; }

   const std::string& operator[](const std::string& tag) const { return getField(tag); }

private:
   mutable std::map<std::string,std::string> m_fieldCache;
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

class cardSortCriteria {
public:
   typedef card* arg_t;
   bool operator()(const arg_t& lhs, const arg_t& rhs) const { return isLess(lhs,rhs); }

   std::list<std::string> fields;

private:
   bool isLess(const card *pLhs, const card *pRhs) const;
};

class cardSet {
public:
   cardSortCriteria c;
   std::set<card*,cardSortCriteria> s;

   void fill(cards& C);

   template<class T>
   void addTo(T& o)
   {
      for(auto *pC : s)
         o.add(*pC);
   }
};

class histogram {
public:
   histogram(const std::string& tag) : m_tag(tag) {}

   void add(card& c);

   void format(std::ostream& s);

private:
   void buildRevValues();

   std::string m_tag;
   std::map<std::string,size_t> m_values;
   std::map<size_t,std::set<std::string> > m_revValues;
};
