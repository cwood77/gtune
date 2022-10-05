#include "../cmn/card.hpp"
#include "run.hpp"
#include <iostream>

class setfCommand : public iCommand {
public:
   setfCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      auto words = m_i.getRemainingWords();
      if(words.size() != 2)
         throw std::runtime_error("setf needs two args");

      auto& S = m_s.demand<cardSet>();
      for(auto *pC : S.s)
         pC->setField(words[0],words[1]);
      std::cout << "updated " << S.s.size() << " card(s)" << std::endl;
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<setfCommand> gSetfCmd("setf");
