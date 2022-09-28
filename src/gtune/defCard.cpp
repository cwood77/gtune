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
}
