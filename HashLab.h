#ifndef HASHLAB_H
#define HASHLAB_H

#include <string>
#include <iosfwd>

unsigned int foldHash(const std::string& key);

class ChainHash {
private:
    struct HashNode {
        std::string key;
        std::string value;
        HashNode* next;

        HashNode(const std::string& k, const std::string& v);
    };

    HashNode** buckets;
    int capacity;
    int count;

    int index(const std::string& key) const;
    void grow();

public:
    explicit ChainHash(int cap = 101);
    ~ChainHash();

    ChainHash(const ChainHash&) = delete;
    ChainHash& operator=(const ChainHash&) = delete;

    bool add(const std::string& key, const std::string& value);
    bool find(const std::string& key, std::string& value) const;
    bool contains(const std::string& key) const;
    bool remove(const std::string& key);

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
        std::string key;
        std::string value;
        bool used = false;
    };

    Slot* first;
    Slot* second;
    int capacity;
    int count;
    unsigned int seed;

    int h1(const std::string& key, int cap, unsigned int salt) const;
    int h2(const std::string& key, int cap, unsigned int salt) const;
    bool place(Slot item, Slot* a, Slot* b, int cap, unsigned int salt) const;
    void rebuild(const std::string& key, const std::string& value);

public:
    explicit CuckooHash(int cap = 101);
    ~CuckooHash();

    CuckooHash(const CuckooHash&) = delete;
    CuckooHash& operator=(const CuckooHash&) = delete;

    bool add(const std::string& key, const std::string& value);
    bool find(const std::string& key, std::string& value) const;
    bool contains(const std::string& key) const;
    bool remove(const std::string& key);

    void clear();
    void print(std::ostream& out) const;
    void printTable(std::ostream& out) const;

    void saveText(std::ostream& out) const;
    void loadText(std::istream& in);
    void saveBinary(std::ostream& out) const;
    void loadBinary(std::istream& in);
};

#endif
