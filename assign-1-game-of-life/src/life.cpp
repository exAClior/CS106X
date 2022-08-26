/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <fstream>  // open file
#include <iostream> // for cout
#include <string>   // play with string
// enable sleeping
#include <chrono> // std::chrono::seconds
#include <thread> // std::this_thread::sleep_for

using namespace std;

#include "console.h" // required of all files that contain the main function
#include "gevents.h" // for mouse event detection
#include "gtimer.h"
#include "simpio.h" // for getLine
#include "strlib.h"

#include "life-constants.h" // for kMaxAge
#include "life-graphics.h"  // for class LifeDisplay

/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome(int &ms);

// load starting configuraiton from file or randomly
static void getStartConfig(int &row, int &column, vector<Grid<int>> &worlds);

// draw the world on display
static void drawOnDisp(Grid<int> &world, LifeDisplay &display);

// evolve the world one day
static void evolveWorld(vector<Grid<int>> &worlds, int &curIdx);

// Count number of neighbours of a cell
static int countNeighbours(Grid<int> &world, int &row, int &col);

// checks if stable
static bool checkStable(vector<Grid<int>> &worlds, int &curIdx);

static bool runAnimation(LifeDisplay &display, vector<Grid<int>> &worlds,
                         int &curIdx, int &ms);
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
  int ms = 0;
  string command;
  bool cont = true;
  // current world and next world
  vector<Grid<int>> worlds;

  display.setTitle("Game of Life");
  welcome(ms);
  getStartConfig(row, column, worlds);
  display.setDimensions(row, column);
  drawOnDisp(worlds[curIdx], display);
  while (cont) {
    if (ms <= 0) {
      command = getLine("Hit enter to evolve or type quit to stop\n");
      if (command == "quit") {
        break;
      }
    }
    cont = runAnimation(display, worlds, curIdx, ms);
  }
  return 0;
}

static bool runAnimation(LifeDisplay &display, vector<Grid<int>> &worlds,
                         int &curIdx, int &ms) {
  GTimer timer(ms);
  timer.start();
  while (true) {
    GEvent event = waitForEvent(TIMER_EVENT + MOUSE_EVENT);
    if (event.getEventClass() == TIMER_EVENT) {
      evolveWorld(worlds, curIdx);
      drawOnDisp(worlds[curIdx], display);
      if (checkStable(worlds, curIdx)) {
        cout << "Stability reached, quitting!" << endl;
        return false;
      }
    } else if (event.getEventType() == MOUSE_PRESSED) {
      return false;
    }
  }
  timer.stop();
  return true;
}

static bool checkStable(vector<Grid<int>> &worlds, int &curIdx) {
  int rows = worlds[0].numRows();
  int cols = worlds[0].numCols();
  int prevIdx = (curIdx + 1) % 2;
  for (int ii = 0; ii < rows; ii++) {
    for (int jj = 0; jj < cols; jj++) {
      if (worlds[curIdx][ii][jj] - worlds[prevIdx][ii][jj] != 1 ||
          worlds[prevIdx][ii][jj] == 0) {
        return false;
      }
    }
  }
  return true;
}

static int countNeighbours(Grid<int> &world, int &row, int &col) {
  // all possible rows and columns of a neighbour
  int row_arr[3] = {row - 1, row, row + 1};
  int col_arr[3] = {col - 1, col, col + 1};

  int rows = world.numRows();
  int cols = world.numCols();

  int neighbours = 0;

  for (int ii : row_arr) {
    for (int jj : col_arr) {
      if (!(ii == row && jj == col)) {
        if ((ii >= 0 && ii < rows) && (jj >= 0 && jj < cols)) {
          if (world[ii][jj] > 0) {
            neighbours += 1;
          }
        }
      }
    }
  }
  return neighbours;
}

static void evolveWorld(vector<Grid<int>> &worlds, int &curIdx) {
  int nxtIdx = (curIdx + 1) % 2;
  int rows = worlds[curIdx].numRows();
  int cols = worlds[curIdx].numCols();
  int neighbours = 0;
  for (int ii = 0; ii < rows; ii++) {
    for (int jj = 0; jj < cols; jj++) {
      neighbours = countNeighbours(worlds[curIdx], ii, jj);
      if (neighbours <= 1) {
        worlds[nxtIdx][ii][jj] = 0;
      } else if (neighbours == 2) {
        worlds[nxtIdx][ii][jj] = worlds[curIdx][ii][jj] + 1;
        if (worlds[nxtIdx][ii][jj] > kMaxAge) {
          worlds[nxtIdx][ii][jj] = kMaxAge;
        }
      } else if (neighbours == 3) {
        // if there were life previously what do I do?
        worlds[nxtIdx][ii][jj] = randomInteger(1, kMaxAge);
      } else {
        worlds[nxtIdx][ii][jj] = 0;
      }
    }
  }
  curIdx = nxtIdx;
}

static void drawOnDisp(Grid<int> &world, LifeDisplay &display) {
  int rows = world.numRows();
  int cols = world.numCols();
  for (int ii = 0; ii < rows; ii++) {
    for (int jj = 0; jj < cols; jj++) {
      display.drawCellAt(ii, jj, world[ii][jj]);
    }
  }
  display.repaint();
}

static void welcome(int &ms) {
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
  ms = stoi(getLine("Provide an interval between evolution in ms, nonpositive "
                    "time to enter manual mode\n"));
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

    curWorld.resize(row, column);
    nextWorld.resize(row, column);

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
