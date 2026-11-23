#include "FList.h"
#include "Serialization.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <limits>

using namespace std;

FNode::FNode(const string& v) : value(v), next(nullptr) {
}

FList::FList() : head(nullptr) {
}

FList::~FList() {
    clear();
}

void FList::clear() {
    while (head) {
        FNode* p = head;
        head = head->next;

        delete p;
    }
}

void FList::pushBack(const string& v) {
    FNode* p = new FNode(v);

    if (!head) {
        head = p;

        return;
    }

    FNode* q = head;

    while (q->next) {
        q = q->next;
    }

    q->next = p;
}

void FList::pushFront(const string& v) {
    FNode* p = new FNode(v);

    p->next = head;
    head = p;
}

bool FList::before(const string& target, const string& v) {
    FNode* p = head;
    FNode* prev = nullptr;

    while (p && p->value != target) {
        prev = p;
        p = p->next;
    }

    if (!p) {
        return false;
    }

    FNode* n = new FNode(v);

    n->next = p;

    if (prev) {
        prev->next = n;
    }
    else {
        head = n;
    }

    return true;
}

bool FList::after(const string& target, const string& v) {
    for (FNode* p = head; p; p = p->next) {
        if (p->value == target) {
            FNode* n = new FNode(v);

            n->next = p->next;
            p->next = n;

            return true;
        }
    }

    return false;
}

bool FList::eraseHead() {
    if (!head) {
        return false;
    }

    FNode* p = head;
    head = head->next;

    delete p;

    return true;
}

bool FList::eraseTail() {
    if (!head) {
        return false;
    }

    if (!head->next) {
        delete head;
        head = nullptr;

        return true;
    }

    FNode* p = head;

    while (p->next->next) {
        p = p->next;
    }

    delete p->next;
    p->next = nullptr;

    return true;
}

bool FList::eraseBefore(const string& target) {
    FNode* p = head;
    FNode* prev = nullptr;
    FNode* beforePrev = nullptr;

    while (p && p->value != target) {
        beforePrev = prev;
        prev = p;
        p = p->next;
    }

    if (!p || !prev) {
        return false;
    }

    if (beforePrev) {
        beforePrev->next = p;
    }
    else {
        head = p;
    }

    delete prev;

    return true;
}

bool FList::eraseAfter(const string& target) {
    for (FNode* p = head; p; p = p->next) {
        if (p->value == target) {
            if (!p->next) {
                return false;
            }

            FNode* n = p->next;
            p->next = n->next;

            delete n;

            return true;
        }
    }

    return false;
}

bool FList::eraseValue(const string& v) {
    FNode* p = head;
    FNode* prev = nullptr;

    while (p && p->value != v) {
        prev = p;
        p = p->next;
    }

    if (!p) {
        return false;
    }

    if (prev) {
        prev->next = p->next;
    }
    else {
        head = p->next;
    }

    delete p;

    return true;
}

bool FList::find(const string& v) const {
    for (const FNode* p = head; p; p = p->next) {
        if (p->value == v) {
            return true;
        }
    }

    return false;
}

void FList::print() const {
    for (const FNode* p = head; p; p = p->next) {
        cout << p->value << " ";
    }

    cout << endl;
}

void FList::printReverseNodes(const FNode* p) const {
    if (!p) {
        return;
    }

    printReverseNodes(p->next);

    cout << p->value << " ";
}

void FList::printReverse() const {
    printReverseNodes(head);

    cout << endl;
}

void FList::saveText(ostream& out) const {
    int count = 0;

    for (const FNode* p = head; p; p = p->next) {
        if (count == numeric_limits<int>::max()) {
            throw length_error("list too large");
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

void FList::loadText(istream& in) {
    int count = 0;

    if (!(in >> count) || count < 0) {
        throw runtime_error("invalid list size");
    }

    FList temp;
    FNode* tail = nullptr;

    for (int i = 0; i < count; i++) {
        string value;

        if (!(in >> quoted(value))) {
            throw runtime_error("read error");
        }

        FNode* p = new FNode(value);

        if (!temp.head) {
            temp.head = p;
        }
        else {
            tail->next = p;
        }

        tail = p;
    }

    swap(head, temp.head);
}

void FList::saveBinary(ostream& out) const {
    int count = 0;

    for (const FNode* p = head; p; p = p->next) {
        if (count == numeric_limits<int>::max()) {
            throw length_error("list too large");
        }

        count++;
    }

    writeInt(out, count);

    for (const FNode* p = head; p; p = p->next) {
        writeString(out, p->value);
    }
}

void FList::loadBinary(istream& in) {
    int count = readInt(in);

    if (count < 0) {
        throw runtime_error("invalid list size");
    }

    FList temp;
    FNode* tail = nullptr;

    for (int i = 0; i < count; i++) {
        string value = readString(in);

        FNode* p = new FNode(value);

        if (!temp.head) {
            temp.head = p;
        }
        else {
            tail->next = p;
        }

        tail = p;
    }

    swap(head, temp.head);
}
