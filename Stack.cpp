#include "Stack.h"
#include "Serialization.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <limits>

using namespace std;

Stack::Stack() : top(nullptr) {
}

Stack::~Stack() {
    clear();
}

void Stack::clear() {
    while (top) {
        FNode* p = top;
        top = top->next;

        delete p;
    }
}

void Stack::push(const string& v) {
    FNode* p = new FNode(v);

    p->next = top;
    top = p;
}

bool Stack::pop(string& v) {
    if (!top) {
        return false;
    }

    FNode* p = top;
    v = p->value;
    top = top->next;

    delete p;

    return true;
}

void Stack::print() const {
    for (const FNode* p = top; p; p = p->next) {
        cout << p->value << " ";
    }

    cout << endl;
}

void Stack::saveText(ostream& out) const {
    int count = 0;

    for (const FNode* p = top; p; p = p->next) {
        if (count == numeric_limits<int>::max()) {
            throw length_error("stack too large");
        }

        count++;
    }

    out << count << '\n';

    for (const FNode* p = top; p; p = p->next) {
        out << quoted(p->value) << '\n';
    }

    if (!out) {
        throw runtime_error("write error");
    }
}

void Stack::loadText(istream& in) {
    int count = 0;

    if (!(in >> count) || count < 0) {
        throw runtime_error("invalid stack size");
    }

    Stack temp;
    FNode* last = nullptr;

    for (int i = 0; i < count; i++) {
        string value;

        if (!(in >> quoted(value))) {
            throw runtime_error("read error");
        }

        FNode* p = new FNode(value);

        if (!temp.top) {
            temp.top = p;
        }
        else {
            last->next = p;
        }

        last = p;
    }

    swap(top, temp.top);
}

void Stack::saveBinary(ostream& out) const {
    int count = 0;

    for (const FNode* p = top; p; p = p->next) {
        if (count == numeric_limits<int>::max()) {
            throw length_error("stack too large");
        }

        count++;
    }

    writeInt(out, count);

    for (const FNode* p = top; p; p = p->next) {
        writeString(out, p->value);
    }
}

void Stack::loadBinary(istream& in) {
    int count = readInt(in);

    if (count < 0) {
        throw runtime_error("invalid stack size");
    }

    Stack temp;
    FNode* last = nullptr;

    for (int i = 0; i < count; i++) {
        string value = readString(in);

        FNode* p = new FNode(value);

        if (!temp.top) {
            temp.top = p;
        }
        else {
            last->next = p;
        }

        last = p;
    }

    swap(top, temp.top);
}
