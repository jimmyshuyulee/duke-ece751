#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void read_input() {
  string line;
  vector<string> input_lines;
  while (getline(cin, line)) {
    input_lines.push_back(line);
  }

  sort(input_lines.begin(), input_lines.end());

  for (int i = 0; i < input_lines.size(); ++i) {
    cout << input_lines[i] << endl;
  }
}

void read_file(string filename) {
  ifstream ifs(filename);
  vector<string> input_lines;
  if (ifs.is_open()) {
    string line;
    while (getline(ifs, line)) {
      input_lines.push_back(line);
    }

    sort(input_lines.begin(), input_lines.end());

    for (int i = 0; i < input_lines.size(); ++i) {
      cout << input_lines[i] << endl;
    }
    ifs.close();
  }
  else {
    cout << "Cannot open the file.";
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char ** argv) {
  if (argc == 1) {
    read_input();
  }
  else {
    for (int i = 1; i < argc; ++i) {
      read_file(argv[i]);
    }
  }
  return EXIT_SUCCESS;
}
