#include "../cmn/card.hpp"
#include "../cmn/lines.hpp"
#include "run.hpp"
#include <fstream>
#include <iostream>

class saveCommand : public iCommand {
public:
   saveCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      size_t nCards = 0;
      size_t nLines = 0;

      auto& C = m_s.demand<cards>();
      for(auto& c : C.c)
      {
         auto n = c.updateLines();
         if(n)
         {
            nCards++;
            nLines+=n;
         }
      }

      std::cout << "updated " << nCards << " card(s)" << std::endl;
      std::cout << "updated " << nLines << " line(s)" << std::endl;

      auto& L = m_s.demand<lines>();
      if(!nLines)
      {
         std::cout << "no cards were changed" << std::endl;
         if(L.dirty)
         {
            nLines = 1;
            std::cout << " but saving anyway because lines are dirty" << std::endl;
         }
      }

      if(nLines)
      {
         auto path = L.filePath + "-updated";
         std::cout << "writing to '" << path << "'" << std::endl;
         std::ofstream out(path.c_str());
         lineWriter::save(L,out);
      }
      else
         std::cout << "skipping actual save" << std::endl;
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<saveCommand> gSaveCmd("save");
