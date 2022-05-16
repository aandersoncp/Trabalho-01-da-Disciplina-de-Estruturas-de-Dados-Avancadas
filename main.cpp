#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "LinkedList.cpp"

using namespace std;

int main() {
  string line;
  Node* aux;
  ifstream input_file ("arquivo-entrada.txt");
  ofstream output_file("arquivo-saida.txt"); 
  LinkedList* list = new LinkedList();

  if (input_file.is_open()) {
    while (!input_file.eof()) {
      getline (input_file, line); 
      cout << "Realizando leitura da linha: " << line << endl;

      // obtendo tokens de uma linha
      vector<string> tokens;
      string delimiter = " ";
      size_t pos = 0;
      string token;
      do {
        pos = line.find(delimiter);
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + delimiter.length());
      } while (pos != string::npos);

      // tratamento dos casos: INC, REM, SUC, IMP
      string operation = tokens[0];
      int value = stoi(tokens[1]);
      if (operation == "INC"){
        aux = new Node(value);
        list->add_node(aux);
      }
      if (operation == "REM"){
        list->remove_node(value);
      }
      if (operation == "SUC"){
        int version = stoi(tokens[2]);
        Node* suc = list->successor(value, version);
        string result = list->successor_string(suc);
        output_file << "SUC " << value << " " << version << endl;
        output_file << result << endl;
      }
      if (operation == "IMP"){
        string result = list->print(value);
        output_file << "IMP " << value << endl;
        output_file << result << endl;
      }
    }
    input_file.close();
    output_file.close();
  } else {
    cout << "Arquivo de entrada não encontrado!";
  }
  
}