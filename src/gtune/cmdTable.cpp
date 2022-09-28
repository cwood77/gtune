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

      textTableConfig ttc;
      auto cols = m_i.getRemainingWords();
      for(auto col : cols)
         ttc.cols.push_back(col);

      textTableBuilder ttb(ttc);
      S.addTo(ttb);
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
