#include "../cmn/card.hpp"
#include "../cmn/textTable.hpp"
#include "run.hpp"
#include <iostream>

class tableCommand : public iCommand {
public:
   tableCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      auto& S = m_s.demand<cardSet>();
      auto& C = m_s.demand<cards>();

      textTableConfig ttc;
      auto cols = m_i.getRemainingWords();
      if(cols.size())
         for(auto col : cols)
            ttc.cols.push_back(col);
      else
         cardTableBuilder::autoConfigure(ttc,C.s,S.c);

      textTableBuilder ttb(ttc);
      {
         cardTableBuilder adapter(ttb);
         S.addTo(adapter);
      }

      std::cout << std::endl;
      ttb.format(std::cout);
      std::cout << std::endl;
      std::cout << std::endl;
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<tableCommand> gTableCmd("table");
