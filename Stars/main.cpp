#include <iostream>

static const int kMaxDimension = 128;
static long long fenw[kMaxDimension + 1][kMaxDimension + 1][kMaxDimension + 1];
int dimension;

struct Coord3D {
  int x;
  int y;
  int z;
};

void FenwUpdate(int x_index, int y_index, int z_index, long long update_value) {
  for (int i = x_index + 1; i <= dimension; i += i & (-i)) {
    for (int j = y_index + 1; j <= dimension; j += j & (-j)) {
      for (int k = z_index + 1; k <= dimension; k += k & (-k)) {
        fenw[i][j][k] += update_value;
      }
    }
  }
}

long long FenwSum(int x_index, int y_index, int z_index) {
  long long result = 0;
  for (int i = x_index + 1; i > 0; i -= i & (-i)) {
    for (int j = y_index + 1; j > 0; j -= j & (-j)) {
      for (int k = z_index + 1; k > 0; k -= k & (-k)) {
        result += fenw[i][j][k];
      }
    }
  }
  return result;
}

long long FenwRangeSum(const Coord3D& start, const Coord3D& end) {
  return FenwSum(end.x, end.y, end.z) - FenwSum(start.x - 1, end.y, end.z) -
         FenwSum(end.x, start.y - 1, end.z) -
         FenwSum(end.x, end.y, start.z - 1) +
         FenwSum(start.x - 1, start.y - 1, end.z) +
         FenwSum(start.x - 1, end.y, start.z - 1) +
         FenwSum(end.x, start.y - 1, start.z - 1) -
         FenwSum(start.x - 1, start.y - 1, start.z - 1);
}

int main() {
  std::cin >> dimension;

  while (true) {
    int command_type;
    std::cin >> command_type;
    if (!std::cin) {
      break;
    }

    if (command_type == 1) {
      int x_coord;
      int y_coord;
      int z_coord;
      long long update_value;
      std::cin >> x_coord >> y_coord >> z_coord >> update_value;
      FenwUpdate(x_coord, y_coord, z_coord, update_value);
    } else if (command_type == 2) {
      int x_start;
      int y_start;
      int z_start;
      int x_end;
      int y_end;
      int z_end;
      std::cin >> x_start >> y_start >> z_start >> x_end >> y_end >> z_end;
      Coord3D start = {x_start, y_start, z_start};
      Coord3D end = {x_end, y_end, z_end};
      std::cout << FenwRangeSum(start, end) << "\n";
    } else if (command_type == 3) {
      break;
    }
  }

  return 0;
}
