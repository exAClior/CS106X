/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "maze-graphics.h"
#include "set.h"
#include "simpio.h"
#include "vector.h"

static int getMazeDimension(string prompt, int minDimension = 7,
                            int maxDimension = 50) {
  while (true) {
    int response = getInteger(prompt);
    if (response == 0)
      return response;
    if (response >= minDimension && response <= maxDimension)
      return response;
    cout << "Please enter a number between " << minDimension << " and "
         << maxDimension << ", inclusive." << endl;
  }
}

// static void generate_cells(Vector<cell> &cells, int dim) {
//   for (int i = 0; i < dim * dim; i++) {
//     cell aCell = {.row = i / dim, .col = i % dim};
//     cells.add(aCell);
//   }
// }

// static void generate_walls(Vector<wall> &walls, Vector<cell> &cells, int dim) {
//   for (int i = 0; i < dim - 1; i++) {
//     for (int j = 0; j < dim; j++) {
//       wall aWall = {.one = cells[i * dim + j], .two = cells[(i + 1) * dim + j]};
//       wall bWall = {.one = cells[j * dim + i], .two = cells[j * dim + (i + 1)]};
//       walls.add(aWall);
//       walls.add(bWall);
//     }
//   }
// }
int main() {
  while (true) {
    int dimension = getMazeDimension(
        "What should the dimension of your maze be [0 to exit]? ");
    if (dimension == 0)
      break;
    cout << "This is where I'd animate the construction of a maze of dimension "
         << dimension << "." << endl;
    // generate walls
    MazeGeneratorView mazeView;
    mazeView.setDimension(dimension);

    // // add walls first time
    Vector<wall> walls;
    Vector<cell> cells;
    generate_cells(cells, dimension);
    generate_walls(walls, cells, dimension);
    // ignore two openings?
    // paint
    mazeView.repaint();
    // remove walls iteratively
    // repaint
    mazeView.repaint();
  }

  return 0;
}
