#include "../cmn/card.hpp"
#include "../cmn/lines.hpp"
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

   return 0;
}
