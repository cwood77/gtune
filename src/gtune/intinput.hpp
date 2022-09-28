#pragma once
#include "run.hpp"
#include <istream>

class iInputPrompt {
public:
   virtual void prompt() = 0;
};

class interactivePrompt : public iInputPrompt {
public:
   interactivePrompt() : m_first(true) {}

   virtual void prompt();

private:
   bool m_first;
};

class streamInput : public iInput {
public:
   streamInput(std::istream& s, iInputPrompt& p) : m_s(s), m_p(p) {}

   virtual std::string getFirstWord();
   virtual const std::string& getRestOfLine() { throw __LINE__; }
   virtual std::vector<std::string> getRemainingWords() { throw __LINE__; }
   virtual bool nextLine();

private:
   static std::string getNextWord(const char *&pThumb);

   std::istream& m_s;
   iInputPrompt& m_p;
   std::string m_curLine;
};
