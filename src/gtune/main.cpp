#include "../cmn/card.hpp"
#include "../cmn/lines.hpp"
#include "../cmn/textTable.hpp"
#include <fstream>
#include <iostream>

int main(int,const char *[])
{
   std::string filePath = "C:\\cygwin64\\home\\cwood\\dev\\gtune\\testdata.txt";
   std::ifstream fstream(filePath.c_str());
   lines L;
   lineReader::load(fstream,L);

   lineClassifier::run(L);

   cards C;
   schemaBuilder::build(L,C.s);
   std::cout << C.s << std::endl;

   cardBuilder::build(L,C);
   std::cout << "found " << C.c.size() << " card(s)" << std::endl;

   {
      cardSet S;
      S.fill(C);
      for(auto *pC : S.s)
         std::cout << "onCard '" << (*pC)["name"] << "'" << std::endl;
   }
   {
      cardSet S;
      S.c.fields.push_back("name");
      S.fill(C);
      for(auto *pC : S.s)
         std::cout << "onCard '" << (*pC)["name"] << "'" << std::endl;

      textTableConfig ttc;
      ttc.cols.push_back("name");
      ttc.cols.push_back("desc");
      textTableBuilder ttb(ttc);
      S.addTo(ttb);
      ttb.format(std::cout);
   }

   return 0;
}
