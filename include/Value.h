#ifndef VALUE_H
#define VALUE_H

class Value {
public:
    virtual Value& operator=(const Value& other){
        return *this;
    };
    virtual ~Value(){};

};

#endif