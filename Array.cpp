#include "Array.h"
#include "Serialization.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <limits>
#include <utility>

using namespace std;

Array::Array() : data(nullptr), size(0), capacity(0) {
}

Array::~Array() {
    clear();
}

void Array::reserve(int n) {
    if (n <= capacity) {
        return;
    }

    int newCapacity = capacity;

    if (newCapacity == 0) {
        newCapacity = 4;
    }

    while (newCapacity < n) {
        if (newCapacity > numeric_limits<int>::max() / 2) {
            newCapacity = n;
            break;
        }

        newCapacity *= 2;
    }

    string* p = new string[newCapacity];

    try {
        for (int i = 0; i < size; i++) {
            p[i] = data[i];
        }
    }
    catch (...) {
        delete[] p;

        throw;
    }

    delete[] data;

    data = p;
    capacity = newCapacity;
}

void Array::push(const string& value) {
    if (size == numeric_limits<int>::max()) {
        throw length_error("array too large");
    }

    reserve(size + 1);

    data[size] = value;
    size++;
}

bool Array::insert(int index, const string& value) {
    if (index < 0 || index > size) {
        return false;
    }

    if (size == numeric_limits<int>::max()) {
        throw length_error("array too large");
    }

    reserve(size + 1);

    for (int i = size; i > index; i--) {
        data[i] = data[i - 1];
    }

    data[index] = value;
    size++;

    return true;
}

bool Array::get(int index, string& value) const {
    if (index < 0 || index >= size) {
        return false;
    }

    value = data[index];

    return true;
}

bool Array::set(int index, const string& value) {
    if (index < 0 || index >= size) {
        return false;
    }

    data[index] = value;

    return true;
}

bool Array::erase(int index) {
    if (index < 0 || index >= size) {
        return false;
    }

    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }

    size--;
    data[size].clear();

    return true;
}

int Array::getSize() const {
    return size;
}

void Array::print() const {
    for (int i = 0; i < size; i++) {
        cout << data[i] << " ";
    }

    cout << endl;
}

void Array::clear() {
    delete[] data;

    data = nullptr;
    size = 0;
    capacity = 0;
}

void Array::saveText(ostream& out) const {
    out << size << '\n';

    for (int i = 0; i < size; i++) {
        out << quoted(data[i]) << '\n';
    }

    if (!out) {
        throw runtime_error("write error");
    }
}

void Array::loadText(istream& in) {
    int n = 0;

    if (!(in >> n) || n < 0) {
        throw runtime_error("invalid array size");
    }

    Array temp;

    for (int i = 0; i < n; i++) {
        string value;

        if (!(in >> quoted(value))) {
            throw runtime_error("read error");
        }

        temp.push(value);
    }

    swap(data, temp.data);
    swap(size, temp.size);
    swap(capacity, temp.capacity);
}

void Array::saveBinary(ostream& out) const {
    writeInt(out, size);

    for (int i = 0; i < size; i++) {
        writeString(out, data[i]);
    }
}

void Array::loadBinary(istream& in) {
    int n = readInt(in);

    if (n < 0) {
        throw runtime_error("invalid array size");
    }

    Array temp;

    for (int i = 0; i < n; i++) {
        string value = readString(in);

        temp.push(value);
    }

    swap(data, temp.data);
    swap(size, temp.size);
    swap(capacity, temp.capacity);
}
