#include "HashTable.h"
HashTable::HashTable(){
    size=101;
    count=0;
    buckets = new Node*[size];
    for(int i=0;i<size;i++){
        buckets[i]=nullptr;
    }
}
HashTable::~HashTable() {
    for (int i = 0; i < size; ++i) {
        Node* current = buckets[i];
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

    for(unsigned int i=0;i<len;i++){
        unsigned int k = key.buf[i];
        hash *= 0x9e3779b1;
        hash ^= (k * hash) + (hash >> 16);
        hash *= 0x85ebca6b;
    }

    hash ^= hash >> 13;
    hash *= 0x7f57c9a3;
    hash ^= hash >> 16;

    return hash % size;
}

void HashTable::insert(SDS &key, const Value& value) {
    int hash = hashCode(key);
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
    newNode->value = new Value(value);
    newNode->next = buckets[hash];
    buckets[hash] = newNode;
    count++;
    //key.print();
    printf("Setting Success\n");
    if (count > size * 0.75) { 
        resize();
    }
}

void HashTable::resize() {
    int newSize = size * 2;
    Node** newBuckets = new Node*[newSize];
    for (int i = 0; i < newSize; ++i) {
        newBuckets[i] = nullptr;
    }

    for (int i = 0; i < size; ++i) {
        Node* current = buckets[i];
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

Value* HashTable::find(SDS& key){
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

void HashTable::odbsave(){
    std::ofstream outfile;
    outfile.open("./save/save");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open the file for reading." << std::endl;
        return;
    }
    for(int i=0;i<size;i++){
        Node* node=buckets[i];
        while(node!=nullptr){
            Value* value=node->value;
            switch (value->tpye)
            {
            case 1:
                if(value->sds.ld)
                    break;
                outfile<<1<<';';
                for(int i=0;i<node->key.len;i++)
                    outfile<<node->key.buf[i];
                outfile<<';';
                for(int i=0;i<value->sds.len;i++)
                    outfile<<value->sds.buf[i];
                outfile<<';'<<'\n';
                break;
            
            default:
                break;
            }
            node=node->next;
        }
    }
    outfile<<'!';
    outfile.close();
    printf("Odbsave finished\n");
}

void HashTable::odbload(){
    std::ifstream infile; 
    infile.open("./save/save"); 
    if (!infile.is_open()) {
        std::cerr << "Failed to open the file for reading." << std::endl;
        return;
    }
    char type='\0';
    char buf[BUFSIZ];
    
    infile.get(type);  
    
    while (type!='!')
    {
        printf("tyoe:%c  ",type);
        switch (type)
        {
        case '1':{
            int l=0;
            char c;
            infile.get(c);
            infile.get(c);
            while(c!=';'){
                buf[l]=c;
                l++;
                infile.get(c);
            }
            SDS key(buf,0,l);
            key.print();
            l=0;
            infile.get(c);
            while(c!=';'){
                buf[l]=c;
                l++;
                infile.get(c);
            }
            SDS sds(buf,0,l);
            sds.print();
            Value value(sds);
            this->insert(key,value);
            infile.get(c);
            break;
        }
        default:
            break;
        }
      
        infile.get(type);
    }
    printf("Odbload finished\n");
    
    infile.close();


}
