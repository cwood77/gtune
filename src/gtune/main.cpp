#include "../cmn/lines.hpp"
#include <fstream>

int main(int,const char *[])
{
   std::string filePath = "C:\\cygwin64\\home\\cwood\\dev\\gtune\\testdata.txt";
   std::ifstream fstream(filePath.c_str());
   lines L;
   lineReader::load(fstream,L);

   lineClassifier::run(L);

   return 0;
}
