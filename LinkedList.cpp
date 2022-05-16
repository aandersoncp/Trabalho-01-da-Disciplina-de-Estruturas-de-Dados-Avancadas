#include "Node.cpp"
#include <iostream>
#include <string>

class LinkedList {
public:
  // Guarda identificadores para lista na versão n, com ponteiro para
  // start (next) e end (prev)
  Mod *versions[100];
  int version;

  LinkedList() {
    Mod *initID = new Mod();
    initID->is_identifier = true;
    version = 0;
    versions[0] = initID;
  }

  // Adicionar mais um item no topo da lista de versões
  void add_version(Mod *identifier) {
    if(version < 99){
      version++;
      versions[version] = identifier;
    }
  }

  // Método de inclusão de nó
  void add_node(Node *node) {
    //Adicionar nó na lista vazia
    Mod* identifier = versions[version];
    Mod* new_identifier = new Mod();
    if(identifier->next == nullptr and identifier->prev == nullptr){
      new_identifier->next = node;
      new_identifier->prev = node;
      add_version(new_identifier);
      return;
    }
    //adicionar nó no inicio da lista
    Node* aux = successor(node->value, version);
    Mod* mod = new Mod();
    if(aux == identifier->next){
      node->next = aux;
      mod->prev = node;
      aux->update_version(version, mod);
      new_identifier->next = node;
      Node* end = (Node*)identifier->prev;
      new_identifier->prev = end->get_last_pointer(); // Garantindo overflow
      add_version(new_identifier);
      return;
    }
    //adicionar o nó no fim da lista
    if(aux == nullptr){
      aux = (Node*) identifier->prev;
      node->prev = aux;
      mod->next = node;
      aux->update_version(version, mod);
      Node* start = (Node*)identifier->next;
      new_identifier->next = start->get_last_pointer(); // Garantindo overflow
      new_identifier->prev = node;
      add_version(new_identifier);
      return;
    } 
    //adicionar o nó entre dois nós
    Node* aux2 = (Node*) aux->get_prev(version);
    Mod* mod2 = new Mod();
    node->next = aux;
    node->prev = aux2;
    mod->prev = node;
    mod2->next = node;
    aux->update_version(version, mod);
    aux2->update_version(version, mod2);
    Node* end = (Node*)identifier->prev;
    Node* start = (Node*)identifier->next;
    new_identifier->next = start->get_last_pointer();
    new_identifier->prev = end->get_last_pointer();
    add_version(new_identifier);
    return;
  }

  // Método de remover um valor da lista caso ele exista
  void remove_node(int value) {
    // Buscar o sucessor
    // Verificar se value é igual ao anterior do sucessor
    // Caso value não seja, não existe na lista (duplicar última idetificador)
    // Caso seja igual, remover da lista com mod nos nós vizinhos
    Mod* identifier = versions[version];
    Node* aux = successor(value, version);
    Mod* new_identifier = new Mod();
    Mod* nullmod = new Mod();
    
    if(aux == nullptr){
      // Verificando se valor está no final da lista
      aux = (Node*) identifier->prev;
      if(aux->value == value){
        // Removendo no final da lista
        Node* new_end = (Node*) aux->get_prev(version);
        nullmod->next_is_removed = true; //alteração
        new_end->update_version(version, nullmod);
        new_identifier->prev = new_end->get_last_pointer();
        Node* start = (Node*) identifier->next;
        new_identifier->next = start->get_last_pointer();
        add_version(new_identifier);
        return;
      }

      // Nenhum alteração pôde ser feita, duplicando última versão
      add_version(identifier);
      return;
    }

    aux = (Node*) aux->get_prev(version);
    if(aux->value != value){
      add_version(identifier);
      return;
    }

    // Remoção no íncio
    if(aux == identifier->next){
      cout << "removendo no início" << endl;
      Node* new_start = (Node*) aux->get_next(version);
      nullmod->prev_is_removed = true; //alteração
      new_start->update_version(version, nullmod);
      new_identifier->next = new_start->get_last_pointer();
      Node* end = (Node*) identifier->prev;
      new_identifier->prev = end->get_last_pointer();
      add_version(new_identifier);
      return;
    }

    // Remoção entre dois nós
    Mod* nullmod2 = new Mod();

    //(aux->prev)->next = aux->next
    Node* prev = (Node*) aux->get_prev(version);
    nullmod->next = aux->get_next(version);
    prev->update_version(version, nullmod);

    //(aux->next)->prev = aux->prev
    Node* next = (Node*) aux->get_next(version);
    nullmod2->prev = aux->get_prev(version);
    next->update_version(version, nullmod2);

    // Os ponteiros next e prev de aux são setados para null
    nullmod = new Mod();
    nullmod->next_is_removed = true;
    nullmod->prev_is_removed = true;
    aux->update_version(version, nullmod);

    // Atualizar o start e o end e criar nova versão
    Node* start = (Node*) identifier->next;
    new_identifier->next = start->get_last_pointer();
    Node* end = (Node*) identifier->prev;
    new_identifier->prev = end->get_last_pointer();
    add_version(new_identifier);
    return;
  }

  Node *successor(int value, int version) {
    // Retorn Node caso exista
    // Retorna null caso não exista
    if(version > this->version){
      version = this->version;
    }

    Mod* identifier = versions[version];
    // Verificar se a lista tá vazia    
    Node* start = (Node*) identifier->next;
    if(start == nullptr) return nullptr;

    Node* aux = (Node*) start->get_next(version);
    while(start->value <= value){
      if(aux == nullptr) return nullptr;
      if(aux->value > value) return aux;
      start = aux;
      aux = (Node*) start->get_next(version);
    }

    return start;
  }

  // Converte o valor de successor para string do arquivo
  string successor_string(Node *successor) {
    if(successor == nullptr) return "INF";
    return to_string(successor->value);
  }

  // Imprime os valores da lista na versão n ou última versão
  string print(int version) {
    Mod* identifier;
    if(version > this->version){
      version = this->version;
    }
    identifier = versions[version];
    Node* aux = (Node*) identifier->next;
    if(aux == nullptr){
      return "";
    } 
    string result = "";
    while(aux->get_next(version) != nullptr){
      result = result + to_string(aux->value) + " ";
      aux = (Node*) aux->get_next(version);
    }
    result = result + to_string(aux->value) + " ";
    return result;
  }
};