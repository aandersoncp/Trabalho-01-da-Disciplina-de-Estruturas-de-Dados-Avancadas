#include <iostream>
#include <string>
#include "Mod.cpp"

using namespace std;

class Node : public Mod {
public:
  int value;
  Mod* mods[4] = { nullptr, nullptr, nullptr, nullptr };
  int last_mod_pos = 0;
  Node* overflowed;

  Node() : Mod() {}

  Node(int value): Mod() , value(value) {};

  // Atualiza vetor de modificações
  void update_version(int last_version, Mod* mod){
    // Inserir modificador na última posição dispível de mod
    // Caso não tenha mods disponíveis, criar overflow
    last_version++;
    mod->version = last_version;
    if(last_mod_pos < 4){
      mods[last_mod_pos++] = mod;
      return;
    }

    create_overflow(mod);  
  };

  // Gerenciar a criação de overflowed
  void create_overflow(Mod *mod){
    // Criar um Node substituto
    // Chamar update_version com overflow para last_next e last_prev
    // (em cadeia)

    Node* overflow = new Node(this->value);
    // Verficar se mod é prev ou next
    if(mod->prev == nullptr){
      overflow->next = mod->next;
      overflow->prev = this->get_prev(mod->version);
      return;
    } 

    overflow->prev = mod->prev;
    overflow->next = this->get_next(mod->version);

    Mod* over_mod_next = new Mod();
    over_mod_next->next = overflow;

    Mod* over_mod_prev = new Mod();
    over_mod_prev->prev = overflow;

    // Fazer update em cadeia
    Node* prev_pointer = (Node*) overflow->prev;
    prev_pointer->update_version(mod->version - 1, over_mod_next);

    Node* next_pointer = (Node*) overflow->next;
    next_pointer->update_version(mod->version - 1, over_mod_prev);

    this->overflowed = overflow;
  };

  // retorna ponteiros da última versão (next)
  Mod* get_next(int version){
    Mod* next = this->next;
    for(int i = 0; i <= 3; i++) {
      if(mods[i] && mods[i]->next != nullptr  && mods[i]->version <= version){
        next = this->mods[i]->next;
      }
      if(mods[i] && mods[i]->next_is_removed == true && mods[i]->version <= version){
        next = nullptr;
      }
      
    }
    return next;
  };

  // retorna ponteiros da última versão (prev)
  Mod* get_prev(int version){
    Mod* prev = this->prev;
    for(int i = 0; i <= 3; i++) {
      //alteração na condição
      if(mods[i] && mods[i]->prev != nullptr && mods[i]->version <= version) {
        prev = this->mods[i]->prev;
      }
      if(mods[i] && mods[i]->prev_is_removed == true && mods[i]->version <= version){
        prev = nullptr;
      }
    }
    return prev;
  };

  // Retorna um ponteior para o overflow caso seja necessário
  Node* get_last_pointer(){
    if(overflowed == nullptr) return this;
    return overflowed;
  }
};