#include "../cmn/card.hpp"
#include "run.hpp"
#include <iostream>

class filterCommand : public iCommand {
public:
   filterCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      auto words = m_i.getRemainingWords();
      if(words.size() > 1)
         throw std::runtime_error("only one parameter can be passed to filter");

      std::unique_ptr<iCardFilter> pF;
      if(words.size())
         pF.reset(new boolFilter(words[0]));
      else
         pF.reset(new nullCardFilter());

      m_s.publish(*pF.release());
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<filterCommand> gFilterCmd("filter");
