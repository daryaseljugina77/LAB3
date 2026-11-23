#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include <iosfwd>
#include "FList.h"

class Queue {
private:
    FNode* head;
    FNode* tail;

public:
    Queue();
    ~Queue();

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

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
