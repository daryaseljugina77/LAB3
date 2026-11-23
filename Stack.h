#ifndef STACK_H
#define STACK_H

#include <string>
#include <iosfwd>
#include "FList.h"

class Stack {
private:
    FNode* top;

public:
    Stack();
    ~Stack();

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void clear();

    void push(const std::string& v);
    bool pop(std::string& v);

    void print() const;

    void saveText(std::ostream& out) const;
    void loadText(std::istream& in);

    void saveBinary(std::ostream& out) const;
    void loadBinary(std::istream& in);
};

#endif

