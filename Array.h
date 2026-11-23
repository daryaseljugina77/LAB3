#ifndef ARRAY_H
#define ARRAY_H

#include <string>
#include <iosfwd>

class Array {
private:
    std::string* data;
    int size;
    int capacity;

    void reserve(int n);

public:
    Array();
    ~Array();

    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    void push(const std::string& value);
    bool insert(int index, const std::string& value);
    bool get(int index, std::string& value) const;
    bool set(int index, const std::string& value);
    bool erase(int index);

    int getSize() const;
    void print() const;
    void clear();

    void saveText(std::ostream& out) const;
    void loadText(std::istream& in);

    void saveBinary(std::ostream& out) const;
    void loadBinary(std::istream& in);
};

#endif
