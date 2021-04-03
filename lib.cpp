#pragma once

#include <fstream>

using namespace std;

long long unsigned int getFileSize(ifstream *fileStream) {
  long long unsigned int currentPosition = fileStream->tellg();
  fileStream->seekg(0, ios_base::end);
  long long unsigned int fileSize = fileStream->tellg();
  fileStream->seekg(currentPosition, ios_base::beg);

  return fileSize;
}
