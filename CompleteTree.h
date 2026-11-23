#ifndef COMPLETETREE_H
#define COMPLETETREE_H

#include <string>
#include <iosfwd>

class CompleteTree {
private:
    struct TNode {
        std::string value;
        TNode* left;
        TNode* right;

        TNode(const std::string& v);
    };

    TNode* root;
    int count;

    void destroy(TNode* p);

    void printPre(const TNode* p) const;
    void printIn(const TNode* p) const;
    void printPost(const TNode* p) const;

public:
    CompleteTree();
    ~CompleteTree();

    CompleteTree(const CompleteTree&) = delete;
    CompleteTree& operator=(const CompleteTree&) = delete;

    void clear();
    void insert(const std::string& v);
    bool find(const std::string& v) const;
    bool isComplete() const;

    int getSize() const;

    void printPre() const;
    void printIn() const;
    void printPost() const;
    void printLevel() const;

    void saveText(std::ostream& out) const;
    void loadText(std::istream& in);

    void saveBinary(std::ostream& out) const;
    void loadBinary(std::istream& in);
};

#endif
