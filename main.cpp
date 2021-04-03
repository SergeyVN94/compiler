#include <iostream>
#include <fstream>
#include "Scanner.cpp"
#include "token.hpp"
#include "lib.cpp"

using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Требуется имя файла." << endl;
    return 1;
  }

  ifstream fileStream(argv[1]);

  if (!fileStream.is_open()) {
    cerr << "Не удалось открыть файл: '" << argv[1] << "'." << endl;
    return 1;
  }

  uint bufferSize = getFileSize(&fileStream);
  char *buffer = new char[bufferSize];
  fileStream.read(buffer, bufferSize);
  fileStream.close();

  Scanner scanner(buffer, bufferSize);

  while (!scanner.isEndScan()) {
    Token t = scanner.getNextToken();
    cout << "Строка: " << scanner.getCurrentLine() << ". имя: " << t.name << ". value: " << t.value << ". lexeme: '" << t.lexeme << "'." << endl; 
  }

  return 0;
}
