#include "../cmn/card.hpp"
#include "../cmn/lines.hpp"
#include "defCard.hpp"
#include <iostream>

void defaultCardFactory::createDefault(iStateCatalog& s)
{
   auto& L = s.demand<lines>();

   std::unique_ptr<cards> pC(new cards());
   schemaBuilder::build(L,pC->s);
   cardBuilder::build(L,*pC);

   std::cout << "[default] created " << pC->c.size() << " card(s)" << std::endl;

   s.publish(*pC.release());
   s.dependsOn<cards,lines>();
}

void defaultFilterFactory::createDefault(iStateCatalog& s)
{
   std::unique_ptr<iCardFilter> pF(new nullCardFilter());
   std::cout << "[default] created null filter" << std::endl;
   s.publish(*pF.release());
}

void defaultCardSetFactory::createDefault(iStateCatalog& s)
{
   auto& C = s.demand<cards>();
   auto& f = s.demand<iCardFilter>();

   std::unique_ptr<cardSet> pS(new cardSet());
   pS->fill(C,f);

   std::cout << "[default] created cardSet" << std::endl;

   s.publish(*pS.release());
   s.dependsOn<cardSet,cards>();
   s.dependsOn<cardSet,iCardFilter>();
}
