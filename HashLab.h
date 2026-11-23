#ifndef HASHLAB_H
#define HASHLAB_H

#include "FList.h"
#include <iosfwd>

unsigned int foldHash(unsigned int key);

class ChainHash {
private:
    FNode** buckets;
    int capacity;
    int count;

    int index(int key) const;
    void grow();

public:
    explicit ChainHash(int cap = 101);
    ~ChainHash();

    ChainHash(const ChainHash&) = delete;
    ChainHash& operator=(const ChainHash&) = delete;

    bool contains(int key) const;
    bool add(int key);
    bool remove(int key);

    void clear();

    void print(std::ostream& out) const;
    void printTable(std::ostream& out) const;

    void saveText(std::ostream& out) const;
    void loadText(std::istream& in);

    void saveBinary(std::ostream& out) const;
    void loadBinary(std::istream& in);
};

class CuckooHash {
private:
    struct Slot {
        int key;
        bool used;
    };

    Slot* first;
    Slot* second;

    int capacity;
    int count;
    unsigned int seed;

    int h1(int key, int cap, unsigned int salt) const;
    int h2(int key, int cap, unsigned int salt) const;

    bool place(
        int key,
        Slot* a,
        Slot* b,
        int cap,
        unsigned int salt
    ) const;

    void rebuild(int extraKey);

public:
    explicit CuckooHash(int cap = 101);
    ~CuckooHash();

    CuckooHash(const CuckooHash&) = delete;
    CuckooHash& operator=(const CuckooHash&) = delete;

    bool contains(int key) const;
    bool add(int key);
    bool remove(int key);

    void clear();

    void print(std::ostream& out) const;
    void printTable(std::ostream& out) const;

    void saveText(std::ostream& out) const;
    void loadText(std::istream& in);

    void saveBinary(std::ostream& out) const;
    void loadBinary(std::istream& in);
};

#endif
