#ifndef FLIST_H
#define FLIST_H

#include <string>
#include <iosfwd>

struct FNode {
    std::string value;
    FNode* next;

    FNode(const std::string& v);
};

class FList {
private:
    FNode* head;

    void printReverseNodes(const FNode* p) const;

public:
    FList();
    ~FList();

    FList(const FList&) = delete;
    FList& operator=(const FList&) = delete;

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
