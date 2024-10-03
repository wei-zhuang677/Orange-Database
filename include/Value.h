#ifndef VALUE_H
#define VALUE_H


class Value {
public:
    Value() = default;
    Value(const Value& other) = default;
    Value& operator=(const Value& other) = default;
    virtual ~Value();
    bool dl=0;
};

#endif