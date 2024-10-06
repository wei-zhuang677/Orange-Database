#include "DateTable.h"
DateTable::DateTable(){
    size=101;
    count=0;
    savedKeysCount=0;
    interval=20;
    threshold=5;
    buckets = new Node*[size];
    for(int j=0;j<size;j++){
        buckets[j]=nullptr;
    }
}
DateTable::~DateTable() {
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

unsigned int DateTable::hashCode(SDS &key){
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

void DateTable::insert(SDS &key, const Value& value) {
    int hash = hashCode(key);
   // std::cout<<hash<<std::endl;
    Node* current = buckets[hash];
    while (current != nullptr) {
        
        if (current->key == key) {
            *(current->value) = value;
            printf("Setting Success\n");
           
            return;
        }
        current = current->next;
    }

    Node* newNode = new Node;
    newNode->key = key;
    //value.sds->print();
    newNode->value = new Value(value);
    newNode->next = buckets[hash];
    buckets[hash] = newNode;
    count++;
    
    printf("Setting Successc\n");
    if (count > size * 0.75) { 
        resize();
    }
}

void DateTable::resize() {
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

Value* DateTable::find(SDS& key){
    int hash=hashCode(key);
    Node* current = buckets[hash];
    savedKeysCount++;
    while (current != nullptr) {

        if (current->key == key) {
            if(current->value->tpye==0)
                return nullptr;
            return current->value;
        }
        current = current->next;
    }

    //perror("No value");
    return nullptr;
    
}

void DateTable::odbsave(){
    std::ofstream outfile;
    outfile.open("./save/save.odb");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open the file for reading." << std::endl;
        return;
    }
    mtx.lock();
    outfile<<0<<' '<<interval<<' '<<threshold<<'\n';
    mtx.unlock();
        printf("Odbsave finished\n");
    for(int i=0;i<size;i++){
        Node* node=buckets[i];
        while(node!=nullptr){
            Value* value=node->value;
            if(value->tpye){
            switch (value->tpye)
            {
            case 1:{
                outfile<<1<<';';
                for(int j=0;j<node->key.len;j++)
                    outfile<<node->key.buf[j];
                outfile<<';';
                for(int j=0;j<value->sds->len;j++)
                    outfile<<value->sds->buf[j];
                outfile<<';'<<'\n';
                break;
            }
            case 2:{
                outfile<<2<<';';
                for(int j=0;j<node->key.len;j++)
                    outfile<<node->key.buf[j];
                outfile<<';';
                List::Node* head=value->list->head;
                while(head!=nullptr){
                    for(int j=0;j<head->value->len;j++){
                        outfile<<head->value->buf[j];
                    }
                    outfile<<';';
                    head=head->next;
                }
                outfile<<'\n';
                break;}
            case 3:{
                outfile<<3<<';';
                for(int j=0;j<node->key.len;j++)
                    outfile<<node->key.buf[j];
                outfile<<';';
                for(int j=0;j<value->hashtable->size;j++){
                    HashTable::Node* current=value->hashtable->buckets[j];
                    while(current!=nullptr){
                        for(int t=0;t<current->key.len;t++)
                        {
                            outfile<<current->key.buf[t];
                        }
                        outfile<<';';
                        for(int t=0;t<current->value->len;t++){
                            outfile<<current->value->buf[t];
                        }
                        outfile<<';';
                        current=current->next;
                    }
                }
                outfile<<'\n';
                break;
            }
            default:
                break;
            }
            }
            node=node->next;
        }
    }
    outfile<<'!';
    outfile.close();

}

void DateTable::odbload(){
    std::ifstream infile; 
    infile.open("./save/save.odb"); 
    if (!infile.is_open()) {
        std::cerr << "Failed to open the file for reading." << std::endl;
        return;
    }
    char type='\0';
    char buf[BUFSIZ];
    mtx.lock();
    infile>>type>>interval>>threshold;
    mtx.unlock();
    infile.get(type);  
  //  printf("tyoe:%c  ",type);
    while (type!='!')
    {
    //    printf("tyoe:%c  ",type);
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
           // key.print();
            l=0;
            infile.get(c);
            while(c!=';'){
                buf[l]=c;
                l++;
                infile.get(c);
            }
            SDS sds(buf,0,l);
           
            Value value(sds); 
            this->insert(key,value);
            infile.get(c);
            break;
        }
        case '2':{
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
            
            infile.get(c);
            List list;
            while(c!='\n'){
                l=0;
                
                while(c!=';'){
                    buf[l]=c;
                   l++;
                   infile.get(c);
                }
                SDS sds(buf,0,l);
                list.addr(sds);
             //   sds.print();
                infile.get(c);
            }
            Value value(list);
            this->insert(key,value); 
            break;
        }
        case '3':{
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
            
            infile.get(c);
            HashTable hashtable;
            while(c!='\n'){
                l=0;
                while(c!=';'){
                    buf[l]=c;
                   l++;
                   infile.get(c);
                }
                SDS feild(buf,0,l);
                l=0;
                infile.get(c);
                while(c!=';'){
                    buf[l]=c;
                   l++;
                   infile.get(c);
                }
                SDS sds(buf,0,l);
                hashtable.insert(feild,sds);
                infile.get(c);
            }
            Value value(hashtable);
            this->insert(key,value);
            break;
        }
        default:{
           break;
        }
            
        }
    
        infile.get(type);
    }
    
      
    infile.close();


}

void DateTable::checkAndSave() {

    while (true) {
        int a=interval,b=threshold;
        std::this_thread::sleep_for(std::chrono::seconds(interval));
        mtx.lock();
        printf("checking\n");
        if(a==interval&&b==threshold&&b<=savedKeysCount){
            mtx.unlock();
            printf("saving\n");
            std::thread t(&DateTable::odbsave,this);
            t.detach();
        }
        else{
            mtx.unlock();
            a=interval;
            b=threshold;
        }
        savedKeysCount=0;
        
    }
}

void DateTable::updateConfig(int a,int b){
    mtx.lock();
    interval=a;
    threshold=b;
    mtx.unlock();
    printf("Updateconfig finished\n");
}