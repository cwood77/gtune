#include "../cmn/card.hpp"
#include "run.hpp"
#include <iostream>

class histCommand : public iCommand {
public:
   histCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      histogram c;
      auto words = m_i.getRemainingWords();
      for(auto word : words)
         c.tags.insert(word);

      auto& S = m_s.demand<cardSet>();
      S.addTo(c);

      std::cout << std::endl;
      c.format(std::cout);
      std::cout << std::endl;
      std::cout << std::endl;
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<histCommand> gHistCmd("hist");
