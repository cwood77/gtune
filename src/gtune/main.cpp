#include "../cmn/card.hpp"
#include "../cmn/lines.hpp"
#include "../cmn/textTable.hpp"
#include "defCard.hpp"
#include "intinput.hpp"
#include <fstream>
#include <iostream>

class quitCommandInfo : public iCommandInfo {
private:
   class cmd : public iCommand {
   public:
      virtual void run() { throw haltException(); }
   };
public:
   virtual bool isMatch(const std::string& word) const { return word == "q"; }
   virtual iCommand *create(iInput& i, iStateCatalog& s) { return new cmd(); }
};

int main(int,const char *[])
{
   quitCommandInfo q;
   commandRegistry::get().registerCommand(q);

   stateCatalog state;
   state.registerDefault<cards>(*new defaultCardFactory());

   interactivePrompt pmpt;
   streamInput in(std::cin,pmpt);

   while(in.nextLine())
   {
      try
      {
         std::unique_ptr<iCommand> pCmd(commandRegistry::get().create(in,state));
         try
         {
            pCmd->run();
         }
         catch(haltException&)
         {
            std::cout << "quitting" << std::endl;
            break;
         }
      }
      catch(std::exception& x)
      {
         std::cout << "ERROR: " << x.what() << std::endl;
      }
   }

#if 0
   // load <path>
   std::string filePath = "C:\\Users\\cwood\\Desktop\\game21 - nostoi\\crayon\\adv.txt";
   std::ifstream fstream(filePath.c_str());
   lines L;
   lineReader::load(fstream,L);

   lineClassifier::run(L);

   cards C;
   schemaBuilder::build(L,C.s);
   // print-schema
   std::cout << C.s << std::endl;

   cardBuilder::build(L,C);

   if(1)
   {
      // sort-by col0 col1
      cardSet S;
      S.c.fields.push_back("suit");
      S.c.fields.push_back("title");
      S.fill(C);

      // table col0 col1 col2
      textTableConfig ttc;
      ttc.cols.push_back("suit");
      ttc.cols.push_back("title");
      ttc.cols.push_back("subtitle");
      textTableBuilder ttb(ttc);
      S.addTo(ttb);
      ttb.format(std::cout);
   }
#endif

   return 0;
}
