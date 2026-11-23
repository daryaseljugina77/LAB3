#ifndef DLIST_H
#define DLIST_H

#include <string>
#include <iosfwd>

struct LNode {
    std::string value;
    LNode* next;
    LNode* prev;

    LNode(const std::string& v);
};

class DList {
private:
    LNode* head;
    LNode* tail;

    void removeNode(LNode* p);

public:
    DList();
    ~DList();

    DList(const DList&) = delete;
    DList& operator=(const DList&) = delete;

    void clear();

    void pushBack(const std::string& v);
    void pushFront(const std::string& v);

    bool before(const std::string& target, const std::string& v);
    bool after(const std::string& target, const std::string& v);

    bool eraseHead();
    bool eraseTail();
    bool eraseBefore(const std::string& target);
    bool eraseAfter(const std::string& target);
    bool eraseValue(const std::string& v);

    bool find(const std::string& v) const;

    void print() const;
    void printReverse() const;

    void saveText(std::ostream& out) const;
    void loadText(std::istream& in);

    void saveBinary(std::ostream& out) const;
    void loadBinary(std::istream& in);
};

#endif
