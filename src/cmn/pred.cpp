#include "../cmn/card.hpp"
#include "pred.hpp"
#include <stdexcept>

namespace {

class fieldValue : public iPred {
public:
   explicit fieldValue(const std::string& name) : m_name(name) {}

   virtual void evaluate(predValueStack& v)
   {
      v.push(predValue::fromString(v.getCard()[m_name]));
   }

private:
   std::string m_name;
};

class literalValue : public iPred {
public:
   explicit literalValue(const std::string& v) : m_v(v) {}

   virtual void evaluate(predValueStack& v)
   {
      v.push(predValue::fromString(m_v));
   }

private:
   std::string m_v;
};

class isEqual : public iPred {
public:
   virtual void evaluate(predValueStack& v)
   {
      auto lhs = v.pop();
      auto rhs = v.pop();
      bool equ = (lhs.toString() == rhs.toString());
      v.push(predValue::fromBool(equ));
   }
};

} // anonymous namespace

predChain::~predChain()
{
   for(auto pr : p)
      delete pr;
}

void predChain::evaluate(predValueStack& v)
{
   for(auto pr : p)
      pr->evaluate(v);
}

iPred *predFactory::create(const std::string& name)
{
   if(name == "==")
      return new isEqual();
   else if(name.c_str()[0] == '.')
      return new fieldValue(name.c_str()+1);
   else
      return new literalValue(name);
}
