/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"

static string getWord(const Lexicon &english, const string &prompt) {
  while (true) {
    string response = trim(toLowerCase(getLine(prompt)));
    if (response.empty() || english.contains(response))
      return response;
    cout << "Your response needs to be an English word, so please try again."
         << endl;
  }
}

static bool wordUsed(const Vector<string> &partialLadder, const string &word) {
  for (string aWord : partialLadder) {
    if (word == aWord) {
      return true;
    }
  }
  return false;
}

static void extendLadder(const Lexicon &english,
                         const Vector<string> &prevLadder,
                         Queue<Vector<string>> &ladderQueue) {
  for (int jj = 0; jj < (int)prevLadder.end()->length(); jj++) {
    for (int ii = 0; ii < 26; ii++) {
      string newWord = *prevLadder.end();
      newWord[jj] = 'a' + ii;
      if (english.contains(newWord) && !wordUsed(prevLadder, newWord)) {
        Vector<string> newLadder = prevLadder;
        newLadder.add(newWord);
        ladderQueue.enqueue(newLadder);
      }
    }
  }
}

static void generateLadder(const Lexicon &english, const string &start,
                           const string &end) {
  cout << "Start generating ladder" << endl;
  Vector<string> wordLadder;
  wordLadder += start;

  Queue<Vector<string>> ladderQueue;
  ladderQueue.enqueue(wordLadder);
  while (!ladderQueue.isEmpty()) {
    cout << "Start dequeuing ladder" << endl;
    Vector<string> lastLadder = ladderQueue.dequeue();
    cout << *lastLadder.end() << endl;
    cout << end << endl;
    if (*lastLadder.end() == end) {
      wordLadder = lastLadder;
      break;
    } else {
      extendLadder(english, lastLadder, ladderQueue);
    }
  }

  cout << "Here's where you'll search for a word ladder connecting \"" << start
       << "\" to \"" << end << "\"." << endl;
  cout << wordLadder << endl;
}

static const string kEnglishLanguageDatafile = "dictionary.txt";
static void playWordLadder() {
  Lexicon english(kEnglishLanguageDatafile);
  while (true) {
    string start =
        getWord(english, "Please enter the source word [return to quit]: ");
    if (start.empty())
      break;
    string end = getWord(
        english, "Please enter the destination word [return to quit]: ");
    if (end.empty())
      break;
    generateLadder(english, start, end);
  }
}

int main() {
  cout << "Welcome to the CS106 word ladder application!" << endl << endl;
  playWordLadder();
  cout << "Thanks for playing!" << endl;
  return 0;
}
