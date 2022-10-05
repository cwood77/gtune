#include "../cmn/card.hpp"
#include "run.hpp"
#include <iostream>

class sortCommand : public iCommand {
public:
   sortCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      auto& C = m_s.demand<cards>();
      auto& f = m_s.demand<iCardFilter>();

      std::cout << "sorting cards" << std::endl;

      std::unique_ptr<cardSet> pS(new cardSet());
      auto cols = m_i.getRemainingWords();
      for(auto col : cols)
         pS->c.fields.push_back(col);
      pS->fill(C,f);

      m_s.publish(*pS.release());
      m_s.dependsOn<cardSet,cards>();
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<sortCommand> gSortCmd("sort");
