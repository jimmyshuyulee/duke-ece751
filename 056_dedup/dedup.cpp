#include <dirent.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

void checkFileExistance(const string & file, unordered_map<string, string> & m) {
  ifstream ifs;
  ifs.open(file.c_str());
  stringstream ss;
  ss << ifs.rdbuf();
  if (m.find(ss.str()) == m.end()) {
    m[ss.str()] = file;
  }
  else {
    cout << "#Removing " << file << " (duplicate of " << m[ss.str()] << ")." << endl;
    cout << "rm " << file << endl;
  }
}

void findAllFiles(const string & path, unordered_map<string, string> & m) {
  struct dirent * de;
  DIR * dr = opendir(path.c_str());

  if (dr == NULL) {
    cerr << "Could not open current directory";
    return;
  }

  while ((de = readdir(dr)) != NULL) {
    if (de->d_name[0] == '.') {
      continue;
    }
    if (de->d_type == DT_DIR) {
      findAllFiles(path + de->d_name + "/", m);
    }
    if (de->d_type == DT_REG) {
      checkFileExistance(path + de->d_name, m);
    }
  }
  closedir(dr);
}

int main(int argc, char ** argv) {
  cout << "#!/bin/bash" << endl;
  unordered_map<string, string> existFiles;
  for (int i = 1; i < argc; ++i) {
    string cd(argv[i]);
    findAllFiles(cd, existFiles);
  }
  return EXIT_SUCCESS;
}
