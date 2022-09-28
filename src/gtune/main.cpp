#include "../cmn/card.hpp"
#include "../cmn/lines.hpp"
#include "../cmn/textTable.hpp"
#include <fstream>
#include <iostream>

int main(int,const char *[])
{
   std::string filePath = "C:\\Users\\cwood\\Desktop\\game21 - nostoi\\crayon\\adv.txt";
   std::ifstream fstream(filePath.c_str());
   lines L;
   lineReader::load(fstream,L);

   lineClassifier::run(L);

   cards C;
   schemaBuilder::build(L,C.s);
   std::cout << C.s << std::endl;

   cardBuilder::build(L,C);

   if(1)
   {
      cardSet S;
      S.c.fields.push_back("suit");
      S.c.fields.push_back("title");
      S.fill(C);

      textTableConfig ttc;
      ttc.cols.push_back("suit");
      ttc.cols.push_back("title");
      ttc.cols.push_back("subtitle");
      textTableBuilder ttb(ttc);
      S.addTo(ttb);
      ttb.format(std::cout);
   }

   return 0;
}
