#include "HashTable.h"
HashTable::HashTable(){
    size=101;
    count=0;
    buckets = new Node*[size];
    for(int j=0;j<size;j++){
        buckets[j]=nullptr;
    }
}

HashTable::HashTable(HashTable& hashtable){
    this->size=hashtable.size;
    this->count=hashtable.count;
    buckets = new Node*[size];
    for(int i=0;i<size;i++){
        buckets[i]=nullptr;
    }
    for(int i=0;i<size;i++){
        Node* current=hashtable.buckets[i];
        while(current!=nullptr){
            if(current->b){
                current=current->next;
                this->count--;
                continue;
            }
            Node* newNode = new Node;
            newNode->key = current->key;
            newNode->value = new SDS(*current->value);
            newNode->next = buckets[i];
            buckets[i] = newNode;
            current=current->next;
        }
    }
}

HashTable::~HashTable() {
    for (int j = 0; j < size; ++j) {
        Node* current = buckets[j];
        while (current != nullptr) {
            Node* next = current->next;
            delete current->value;
            delete current;
            current = next;
        }
    }
    delete[] buckets;
}

unsigned int HashTable::hashCode(SDS &key){
    unsigned int len = key.len;
    unsigned int seed = 0x9747b28c;
    unsigned int hash = seed ^ len;

    for(unsigned int j=0;j<len;j++){
        unsigned int k = key.buf[j];
        hash *= 0x9e3779b1;
        hash ^= (k * hash) + (hash >> 16);
        hash *= 0x85ebca6b;
    }

    hash ^= hash >> 13;
    hash *= 0x7f57c9a3;
    hash ^= hash >> 16;

    return hash % size;
}

void HashTable::insert(SDS &key,  SDS& value) {
    int hash = hashCode(key);
   // std::cout<<hash<<std::endl;
    Node* current = buckets[hash];
    while (current != nullptr) {
        
        if (current->key == key) {
            *(current->value) = value;     
            return;
        }
        current = current->next;
    }

    Node* newNode = new Node;
    newNode->key = key;
    
    newNode->value = new SDS(value);
    newNode->next = buckets[hash];
    buckets[hash] = newNode;
    count++;
    //value.print();
    //printf("Setting Successc\n");
    if (count > size * 0.75) { 
        resize();
    }
}

void HashTable::resize() {
    int newSize = size * 2;
    Node** newBuckets = new Node*[newSize];
    for (int j = 0; j < newSize; ++j) {
        newBuckets[j] = nullptr;
    }

    for (int j = 0; j < size; ++j) {
        Node* current = buckets[j];
        while (current != nullptr) {
            int newHash = hashCode(current->key) % newSize;
            Node* next = current->next;
            if (newBuckets[newHash] == nullptr) {
                current->next = newBuckets[newHash];
                newBuckets[newHash] = current;
            } else {
                Node* temp = newBuckets[newHash];
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = current;
            }
            current = next;
        }
    }
    delete[] buckets;
    buckets = newBuckets;
    size = newSize;
}

SDS* HashTable::find(SDS& key){
    int hash=hashCode(key);
    Node* current = buckets[hash];
    while (current != nullptr) {

        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    //perror("No value");
    return nullptr;
    
}

void HashTable::delet(SDS& key){
    int hash=hashCode(key);
    Node* current = buckets[hash];
    while (current != nullptr) {

        if (current->key == key) {
            current->b=1;
            return;
        }
        current = current->next;
    }
}
