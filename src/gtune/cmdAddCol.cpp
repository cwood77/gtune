#include "../cmn/card.hpp"
#include "../cmn/lines.hpp"
#include "run.hpp"
#include <iostream>

class addColCommand : public iCommand {
public:
   addColCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      auto words = m_i.getRemainingWords();
      if(words.size() != 1)
         throw std::runtime_error("addCol needs one arg");

      // create my own set, that honors the user's filter but not their
      // sort, because I want the cards in real order so patches are in
      // order
      auto& C = m_s.demand<cards>();
      auto& F = m_s.demand<iCardFilter>();
      cardSet S;
      S.fill(C,F);

      // create patches
      std::list<linePatch> patches;
      for(auto *pC : S.s)
         pC->addFieldIf(words[0],patches);

      if(patches.size())
      {
         std::cout << "updating " << patches.size() << " card(s)" << std::endl;

         auto& L = m_s.demand<lines>();
         m_s.invalidateDeps<lines>();
         for(auto it=patches.rbegin();it!=patches.rend();++it)
            L.inject(*it);

         L.dirty = true;
      }
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<addColCommand> gAddColCmd("addCol");
