#include "../cmn/lines.hpp"
#include "run.hpp"
#include <fstream>
#include <iostream>

class loadCommand : public iCommand {
public:
   loadCommand(iInput& i, iStateCatalog& s) : m_i(i), m_s(s) {}

   virtual void run()
   {
      std::unique_ptr<lines> pL(new lines());
      {
         auto path = m_i.getRestOfLine();

         // just a little hack for testing
         if(path == "!n")
            path = "C:\\Users\\cwood\\Desktop\\game21 - nostoi\\crayon\\adv.txt";

         std::cout << "loading '" << path << "'" << std::endl;
         std::ifstream fstream(path.c_str());
         if(!fstream.good())
            throw std::runtime_error("file can't be loaded");
         lineReader::load(fstream,*pL);
      }

      std::cout << "analyzing " << pL->l.size() << " line(s)" << std::endl;
      lineClassifier::run(*pL);
      m_s.publish(*pL.release());
   }

private:
   iInput& m_i;
   iStateCatalog& m_s;
};

static autoCommandInfo<loadCommand> gLoadCmd("load");
