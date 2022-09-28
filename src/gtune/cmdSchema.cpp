#include "../cmn/card.hpp"
#include "../cmn/lines.hpp"
#include "run.hpp"
#include <iostream>

class schemaCommand : public iCommand {
public:
   schemaCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      auto& C = m_s.demand<cards>();
      std::cout << C.s << std::endl;
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<schemaCommand> gSchemaCmd("schema");

