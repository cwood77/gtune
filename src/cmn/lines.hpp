#pragma once
#include <istream>
#include <string>
#include <vector>

class lines;

class line {
public:
   enum types {
      kUnclassified,
      kTag,
      kPayload,
      kBlankLine,
      kPreamble,
      kDirective,
   } type;

   enum {
      kFlagSingleLineTag = 0x1
   };

   size_t flags;
   std::string tag;
   std::string directive;
   std::string text;
   lines *pOwner;
   size_t index;
};

class linePatch {
public:
   std::vector<line> lines;
   size_t at;
};

class lines {
public:
   lines() : dirty(false) {}

   std::string filePath;
   std::vector<line> l;
   bool dirty;

   void inject(std::vector<line>& more, size_t before);
   void inject(linePatch& p) { inject(p.lines,p.at); }
};

class lineReader {
public:
   static void load(std::istream& s, lines& l);
};

class lineWriter {
public:
   static void save(lines& l, std::ostream& s);
};

class lineClassifier {
public:
   static void run(lines& l);

private:
   lineClassifier() : m_pLastTag(NULL), m_numPayloadLines(0) {}

   void _run(lines& l);
   void run(line& l);

   void openTag(line& l);
   void closeTagIf();

   static bool tryParseTag(const std::string& text, std::string& tag);

   line *m_pLastTag;
   size_t m_numPayloadLines;
};
