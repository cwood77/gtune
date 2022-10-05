#include "../cmn/card.hpp"
#include "../cmn/pred.hpp"
#include "run.hpp"
#include <iostream>

class projectCommand : public iCommand {
public:
   projectCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      auto words = m_i.getRemainingWords();
      if(words.size() < 2)
         throw std::runtime_error("need more arguments");
      auto newField = words[0];

      // create the predicate chain
      predChain pc;
      {
         predFactory pf;
         auto it = words.begin();
         for(++it;it!=words.end();++it)
            pc.p.push_back(pf.create(*it));
      }

      // run the chain for each card
      auto& S = m_s.demand<cardSet>();
      for(auto *pC : S.s)
      {
         predValueStack s(*pC);
         pc.evaluate(s);
         if(s.size() != 1)
            throw std::runtime_error("predicate evaluated to non-unity stack size");
         pC->setField(newField,s.pop().toString());
      }
      std::cout << "updated " << S.s.size() << " card(s)" << std::endl;

      // update the schema if necessary
      auto& C = m_s.demand<cards>();
      if(C.s.tags.find(newField) == C.s.tags.end())
         C.s.projected.insert(newField);
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<projectCommand> gProjectCmd("project");
