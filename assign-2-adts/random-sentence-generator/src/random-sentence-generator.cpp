/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <fstream>
#include <iostream>
using namespace std;

#include "console.h"
#include "simpio.h" // for getLine
#include "strlib.h" // for toLowerCase, trim

static const string kGrammarsDirectory = "grammars/";
static const string kGrammarFileExtension = ".g";
static string getNormalizedFilename(string filename) {
  string normalizedFileName = kGrammarsDirectory + filename;
  if (!endsWith(normalizedFileName, kGrammarFileExtension))
    normalizedFileName += kGrammarFileExtension;
  return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
  string normalizedFileName = getNormalizedFilename(filename);
  ifstream infile(normalizedFileName.c_str());
  return !infile.fail();
}

static string getFileName() {
  while (true) {
    string filename =
        trim(getLine("Name of grammar file? [<return> to quit]: "));
    if (filename.empty() || isValidGrammarFilename(filename))
      return filename;
    cout << "Failed to open the grammar file named \"" << filename
         << "\". Please try again...." << endl;
  }
}

static void loadFile(string filename) {
  ifstream my_file;
  string my_line;
  my_file.open(getNormalizedFilename(filename));
  if (my_file.is_open()) {
    while (my_file) {
      getline(my_file, my_line);
      // make a vector for first line
      // make a map of <string> and list of lines for the rest
      // pop continuously of first line
      // each time non <>, pinrt
      //  <> expand and push line back into queue? this is problematic,
    }
  }
}
int main() {
  while (true) {
    string filename = getFileName();
    if (filename.empty())
      break;
    cout << "Here's where you read in the \"" << filename << "\" grammar "
         << "and generate three random sentences." << endl;
    loadFile(filename);
    // for (int i = 0; i < 3; i++) {
    // }
  }

  cout << "Thanks for playing!" << endl;
  return 0;
}
