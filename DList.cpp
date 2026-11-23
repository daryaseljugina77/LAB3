#include "DList.h"
#include "Serialization.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <limits>

using namespace std;

LNode::LNode(const string& v) : value(v), next(nullptr), prev(nullptr) {
}

DList::DList() : head(nullptr), tail(nullptr) {
}

DList::~DList() {
    clear();
}

void DList::clear() {
    while (head) {
        LNode* p = head;
        head = head->next;
        delete p;
    }

    tail = nullptr;
}

void DList::pushBack(const string& v) {
    LNode* p = new LNode(v);

    p->prev = tail;

    if (tail) {
        tail->next = p;
    }
    else {
        head = p;
    }

    tail = p;
}

void DList::pushFront(const string& v) {
    LNode* p = new LNode(v);

    p->next = head;

    if (head) {
        head->prev = p;
    }
    else {
        tail = p;
    }

    head = p;
}

bool DList::before(const string& target, const string& v) {
    for (LNode* p = head; p; p = p->next) {
        if (p->value == target) {
            LNode* n = new LNode(v);

            n->next = p;
            n->prev = p->prev;

            if (p->prev) {
                p->prev->next = n;
            }
            else {
                head = n;
            }

            p->prev = n;

            return true;
        }
    }

    return false;
}

bool DList::after(const string& target, const string& v) {
    for (LNode* p = head; p; p = p->next) {
        if (p->value == target) {
            LNode* n = new LNode(v);

            n->prev = p;
            n->next = p->next;

            if (p->next) {
                p->next->prev = n;
            }
            else {
                tail = n;
            }

            p->next = n;

            return true;
        }
    }

    return false;
}

void DList::removeNode(LNode* p) {
    if (!p) {
        return;
    }

    if (p->prev) {
        p->prev->next = p->next;
    }
    else {
        head = p->next;
    }

    if (p->next) {
        p->next->prev = p->prev;
    }
    else {
        tail = p->prev;
    }

    delete p;
}

bool DList::eraseHead() {
    if (!head) {
        return false;
    }

    removeNode(head);

    return true;
}

bool DList::eraseTail() {
    if (!tail) {
        return false;
    }

    removeNode(tail);

    return true;
}

bool DList::eraseBefore(const string& target) {
    for (LNode* p = head; p; p = p->next) {
        if (p->value == target) {
            if (!p->prev) {
                return false;
            }

            removeNode(p->prev);

            return true;
        }
    }

    return false;
}

bool DList::eraseAfter(const string& target) {
    for (LNode* p = head; p; p = p->next) {
        if (p->value == target) {
            if (!p->next) {
                return false;
            }

            removeNode(p->next);

            return true;
        }
    }

    return false;
}

bool DList::eraseValue(const string& v) {
    for (LNode* p = head; p; p = p->next) {
        if (p->value == v) {
            removeNode(p);

            return true;
        }
    }

    return false;
}

bool DList::find(const string& v) const {
    for (const LNode* p = head; p; p = p->next) {
        if (p->value == v) {
            return true;
        }
    }

    return false;
}

void DList::print() const {
    for (const LNode* p = head; p; p = p->next) {
        cout << p->value << " ";
    }

    cout << endl;
}

void DList::printReverse() const {
    for (const LNode* p = tail; p; p = p->prev) {
        cout << p->value << " ";
    }

    cout << endl;
}

void DList::saveText(ostream& out) const {
    int count = 0;

    for (const LNode* p = head; p; p = p->next) {
        if (count == numeric_limits<int>::max()) {
            throw length_error("list too large");
        }

        count++;
    }

    out << count << '\n';

    for (const LNode* p = head; p; p = p->next) {
        out << quoted(p->value) << '\n';
    }

    if (!out) {
        throw runtime_error("write error");
    }
}

void DList::loadText(istream& in) {
    int count = 0;

    if (!(in >> count) || count < 0) {
        throw runtime_error("invalid list size");
    }

    DList temp;

    for (int i = 0; i < count; i++) {
        string value;

        if (!(in >> quoted(value))) {
            throw runtime_error("read error");
        }

        temp.pushBack(value);
    }

    swap(head, temp.head);
    swap(tail, temp.tail);
}

void DList::saveBinary(ostream& out) const {
    int count = 0;

    for (const LNode* p = head; p; p = p->next) {
        if (count == numeric_limits<int>::max()) {
            throw length_error("list too large");
        }

        count++;
    }

    writeInt(out, count);

    for (const LNode* p = head; p; p = p->next) {
        writeString(out, p->value);
    }
}

void DList::loadBinary(istream& in) {
    int count = readInt(in);

    if (count < 0) {
        throw runtime_error("invalid list size");
    }

    DList temp;

    for (int i = 0; i < count; i++) {
        string value = readString(in);

        temp.pushBack(value);
    }

    swap(head, temp.head);
    swap(tail, temp.tail);
}
