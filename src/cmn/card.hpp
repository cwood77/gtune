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
   void setField(const std::string& tag, const std::string& value);

   const std::string& operator[](const std::string& tag) const { return getField(tag); }

   size_t updateLines();

private:
   mutable std::map<std::string,std::string> m_fieldCache;
   mutable std::map<std::string,line*> m_singleLineField;
   std::set<std::string> m_changed;
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

class iCardFilter {
public:
   virtual bool isIn(card& c) = 0;
};

class nullCardFilter : public iCardFilter {
public:
   virtual bool isIn(card& c) { return true; }
};

class boolFilter : public iCardFilter {
public:
   explicit boolFilter(const std::string& f) : m_field(f) {}

   virtual bool isIn(card& c);

private:
   std::string m_field;
};

class cardSet {
public:
   cardSortCriteria c;
   std::set<card*,cardSortCriteria> s;

   void fill(cards& C, iCardFilter& f);

   template<class T>
   void addTo(T& o)
   {
      for(auto *pC : s)
         o.add(*pC);
   }
};

class histogram {
public:
   std::set<std::string> tags;

   void add(card& c);

   void format(std::ostream& s);

private:
   void buildRevValues();

   std::map<std::string,size_t> m_values;
   std::map<size_t,std::set<std::string> > m_revValues;
};
