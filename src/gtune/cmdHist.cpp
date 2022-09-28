#include "../cmn/card.hpp"
#include "run.hpp"
#include <iostream>

class histCommand : public iCommand {
public:
   histCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      auto& S = m_s.demand<cardSet>();

      histogram c(m_i.getRestOfLine());
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
