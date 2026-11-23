#include "Queue.h"
#include "Serialization.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <limits>

using namespace std;

Queue::Queue() : head(nullptr), tail(nullptr) {
}

Queue::~Queue() {
    clear();
}

void Queue::clear() {
    while (head) {
        FNode* p = head;
        head = head->next;
        delete p;
    }

    tail = nullptr;
}

void Queue::push(const string& v) {
    FNode* p = new FNode(v);

    if (tail) {
        tail->next = p;
    }
    else {
        head = p;
    }

    tail = p;
}

bool Queue::pop(string& v) {
    if (!head) {
        return false;
    }

    FNode* p = head;
    v = p->value;
    head = head->next;

    if (!head) {
        tail = nullptr;
    }

    delete p;

    return true;
}

void Queue::print() const {
    for (const FNode* p = head; p; p = p->next) {
        cout << p->value << " ";
    }

    cout << endl;
}
void Queue::saveText(ostream& out) const {
    int count = 0;

    for (const FNode* p = head; p; p = p->next) {
        if (count == numeric_limits<int>::max()) {
            throw length_error("queue too large");
        }

        count++;
    }

    out << count << '\n';

    for (const FNode* p = head; p; p = p->next) {
        out << quoted(p->value) << '\n';
    }

    if (!out) {
        throw runtime_error("write error");
    }
}

void Queue::loadText(istream& in) {
    int count = 0;

    if (!(in >> count) || count < 0) {
        throw runtime_error("invalid queue size");
    }

    Queue temp;

    for (int i = 0; i < count; i++) {
        string value;

        if (!(in >> quoted(value))) {
            throw runtime_error("read error");
        }

        temp.push(value);
    }

    swap(head, temp.head);
    swap(tail, temp.tail);
}

void Queue::saveBinary(ostream& out) const {
    int count = 0;

    for (const FNode* p = head; p; p = p->next) {
        if (count == numeric_limits<int>::max()) {
            throw length_error("queue too large");
        }

        count++;
    }

    writeInt(out, count);

    for (const FNode* p = head; p; p = p->next) {
        writeString(out, p->value);
    }
}

void Queue::loadBinary(istream& in) {
    int count = readInt(in);

    if (count < 0) {
        throw runtime_error("invalid queue size");
    }

    Queue temp;

    for (int i = 0; i < count; i++) {
        string value = readString(in);

        temp.push(value);
    }

    swap(head, temp.head);
    swap(tail, temp.tail);
}
