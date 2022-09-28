#pragma once
#include "run.hpp"

class defaultCardFactory : public iDefaultStateFactory {
public:
   virtual void createDefault(iStateCatalog& s);
};
