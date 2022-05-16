#include <iostream>
#include <string>
#include <vector>

// Clase modificação
// Guarda uma versão, ou uma modificação de ponteiro
class Mod {
  public:
    int version = 0;
    Mod* prev = nullptr;
    Mod* next = nullptr;
    bool is_identifier;  // Indica que é um identificado no início de uma lista
    bool next_is_removed;// Indica modificações de remoção
    bool prev_is_removed;

    Mod() : version(0), prev(nullptr), next(nullptr), next_is_removed(false), prev_is_removed(false) {};
    Mod(int version): version(version) {};
};