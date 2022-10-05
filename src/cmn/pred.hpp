#pragma once
#include <list>
#include <string>

class card;

class predValue {
public:
   static predValue fromString(const std::string& v) { return predValue(v); }
   static predValue fromBool(bool v) { return predValue(v ? "true" : "false"); }

   std::string toString() const { return m_str; }
   bool toBool() const { return m_str != "false"; }

private:
   explicit predValue(const std::string& v) : m_str(v) {}

   std::string m_str;
};

class predValueStack {
public:
   explicit predValueStack(card& c) : m_c(c) {}

   card& getCard() { return m_c; }
   void push(const predValue& v) { m_s.push_back(v); }
   predValue pop() { predValue v = m_s.back(); m_s.pop_back(); return v; }
   size_t size() const { return m_s.size(); }

private:
   card& m_c;
   std::list<predValue> m_s;
};

class iPred {
public:
   virtual ~iPred() {}
   virtual void evaluate(predValueStack& v) = 0;
};

class predChain : public iPred {
public:
   ~predChain();

   virtual void evaluate(predValueStack& v);

   std::list<iPred*> p;
};

class predFactory {
public:
   iPred *create(const std::string& name);
};
