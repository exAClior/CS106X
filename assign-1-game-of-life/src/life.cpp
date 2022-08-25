/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <fstream>  // open file
#include <iostream> // for cout
#include <string>   // play with string
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "gevents.h" // for mouse event detection
#include "simpio.h"  // for getLine
#include "strlib.h"

#include "life-constants.h" // for kMaxAge
#include "life-graphics.h"  // for class LifeDisplay

/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome();

static void getStartConfig(int &row, int &column, vector<Grid<int>> &worlds);

static void drawOnDisp(Grid<int> & world, LifeDisplay & display);
/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
  LifeDisplay display;
  // prepare row and column
  int row = 0, column = 0;
  int curIdx = 0;
  // current world and next world
  vector<Grid<int>> worlds;

  display.setTitle("Game of Life");
  welcome();
  getStartConfig(row, column, worlds);
  display.setDimensions(row, column);
  drawOnDisp(worlds[curIdx],display);
  display.repaint();
  getLine("Give me some time to look at this");
  return 0;
}

static void drawOnDisp(Grid<int> & world, LifeDisplay & display){
  int rows = world.numRows();
  int cols = world.numCols();
  for (int ii = 0 ; ii < rows; ii ++){
    for (int jj = 0; jj < cols; jj ++){
      display.drawCellAt(ii,jj,world[ii][jj]);
    }
  }
}

static void welcome() {
  cout << "Welcome to the game of Life, a simulation of the lifecycle of a "
          "bacteria colony."
       << endl;
  cout << "Cells live and die by the following rules:" << endl << endl;
  cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
  cout << "\tLocations with 2 neighbors remain stable" << endl;
  cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
  cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl
       << endl;
  cout << "In the animation, new cells are dark and fade to gray as they age."
       << endl
       << endl;
  getLine("Hit [enter] to continue....   ");
}

static void getStartConfig(int &row, int &column, vector<Grid<int>> &worlds) {
  string filename =
      getLine("Provide a file name that you wish to load config from:");
  ifstream my_file;
  my_file.open("files/" + filename);

  Grid<int> curWorld, nextWorld;

  if (!my_file) {
    cout << filename << " does not exist, will load random config" << endl;
    row = randomInteger(40, 60);
    column = randomInteger(40, 60);

    curWorld.resize(row,column);
    nextWorld.resize(row,column);

    for (int ii = 0; ii < row; ii++) {
      for (int jj = 0; jj < column; jj++) {
        curWorld[ii][jj] = randomInteger(0, 1) * randomInteger(1, kMaxAge);
      }
    }
  } else {
    string line;
    // get column and row info first
    while (getline(my_file, line)) {
      if (line[0] != '#') {
        row = stoi(line);
        getline(my_file, line);
        column = stoi(line);
        break;
      }
    }

    int row_count = 0;

    curWorld.resize(row, column);
    nextWorld.resize(row, column);

    while (getline(my_file, line)) {
      for (int column_count = 0; column_count < column; column_count++) {
        if (line[column_count] == '-') {
          curWorld[row_count][column_count] = 0;
        } else {
          curWorld[row_count][column_count] = 1;
        }
      }
      row_count += 1;
    }
  }

  worlds.push_back(curWorld);
  worlds.push_back(nextWorld);
  my_file.close();
}
