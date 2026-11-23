#include "HashLab.h"
#include "Serialization.h"
#include <string>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <iomanip>

using namespace std;

unsigned int foldHash(unsigned int key) {
    unsigned int sum = 0;

    do {
        sum += key % 1000U;
        key /= 1000U;
    }
    while (key != 0);

    return sum;
}

// Метод цепочек

ChainHash::ChainHash(int cap)
    : buckets(nullptr), capacity(cap), count(0) {
    if (cap <= 0 || cap > 10000000) {
        throw invalid_argument("invalid capacity");
    }

    buckets = new FNode*[capacity]();
}

ChainHash::~ChainHash() {
    clear();

    delete[] buckets;
}

void ChainHash::clear() {
    for (int i = 0; i < capacity; i++) {
        FNode* p = buckets[i];

        while (p) {
            FNode* next = p->next;

            delete p;

            p = next;
        }

        buckets[i] = nullptr;
    }

    count = 0;
}

int ChainHash::index(int key) const {
    return static_cast<int>(
        foldHash(static_cast<unsigned int>(key)) %
        static_cast<unsigned int>(capacity)
    );
}

bool ChainHash::contains(int key) const {
    const string s = to_string(key);

    for (const FNode* p = buckets[index(key)]; p; p = p->next) {
        if (p->value == s) {
            return true;
        }
    }

    return false;
}

void ChainHash::grow() {
    if (capacity > 4999999) {
        throw length_error("table too large");
    }

    int nextCap = capacity * 2 + 1;

    FNode** fresh = new FNode*[nextCap]();

    for (int i = 0; i < capacity; i++) {
        FNode* p = buckets[i];

        while (p) {
            FNode* next = p->next;

            unsigned int k = static_cast<unsigned int>(
                stoi(p->value)
            );

            int idx = static_cast<int>(
                foldHash(k) %
                static_cast<unsigned int>(nextCap)
            );

            p->next = fresh[idx];
            fresh[idx] = p;
            p = next;
        }
    }

    delete[] buckets;

    buckets = fresh;
    capacity = nextCap;
}

bool ChainHash::add(int key) {
    if (contains(key)) {
        return false;
    }

    if (count >= capacity * 3 / 4) {
        grow();
    }

    int idx = index(key);

    FNode* p = new FNode(to_string(key));

    p->next = buckets[idx];
    buckets[idx] = p;

    count++;

    return true;
}

bool ChainHash::remove(int key) {
    const string s = to_string(key);

    FNode** link = &buckets[index(key)];

    while (*link) {
        if ((*link)->value == s) {
            FNode* old = *link;

            *link = old->next;

            delete old;

            count--;

            return true;
        }

        link = &((*link)->next);
    }

    return false;
}

void ChainHash::print(ostream& out) const {
    for (int i = 0; i < capacity; i++) {
        for (const FNode* p = buckets[i]; p; p = p->next) {
            out << ' ' << p->value;
        }
    }
}

void ChainHash::printTable(ostream& out) const {
    out << "CHAIN (method of chains)\n";
    out << "Index  | Keys\n";
    out << "-------+--------------------------\n";

    bool hasRows = false;

    for (int i = 0; i < capacity; i++) {
        if (!buckets[i]) {
            continue;
        }

        hasRows = true;

        out << setw(6) << i << " | ";

        for (const FNode* p = buckets[i]; p; p = p->next) {
            if (p != buckets[i]) {
                out << " -> ";
            }

            out << p->value;
        }

        out << '\n';
    }

    if (!hasRows) {
        out << "(table is empty)\n";
    }

    out << "Keys: " << count
        << ", capacity: " << capacity << '\n';
}

// Хеширование кукушки

CuckooHash::CuckooHash(int cap)
    : first(nullptr), second(nullptr),
      capacity(cap), count(0), seed(0) {
    if (cap <= 0 || cap > 10000000) {
        throw invalid_argument("invalid capacity");
    }

    first = new Slot[capacity]();

    try {
        second = new Slot[capacity]();
    }
    catch (...) {
        delete[] first;

        throw;
	}
}

CuckooHash::~CuckooHash() {
    delete[] first;
    delete[] second;
}

void CuckooHash::clear() {
    for (int i = 0; i < capacity; i++) {
        first[i].used = false;
        second[i].used = false;
    }

    count = 0;
    seed = 0;
}

int CuckooHash::h1(
    int key,
    int cap,
    unsigned int salt
) const {
    return static_cast<int>(
        (foldHash(static_cast<unsigned int>(key)) + salt) %
        static_cast<unsigned int>(cap)
    );
}

int CuckooHash::h2(
    int key,
    int cap,
    unsigned int salt
) const {
    unsigned int x = static_cast<unsigned int>(key);

    return static_cast<int>(
        (
            x % static_cast<unsigned int>(cap) +
            x / static_cast<unsigned int>(cap) +
            salt
        ) % static_cast<unsigned int>(cap)
    );
}

bool CuckooHash::contains(int key) const {
    int a = h1(key, capacity, seed);
    int b = h2(key, capacity, seed);

    return (first[a].used && first[a].key == key) ||
           (second[b].used && second[b].key == key);
}

bool CuckooHash::place(
    int key,
    Slot* a,
    Slot* b,
    int cap,
    unsigned int salt
) const {
    bool useFirst = true;

    for (int step = 0; step < cap * 2; step++) {
        Slot* slot = nullptr;

        if (useFirst) {
            slot = &a[h1(key, cap, salt)];
        }
        else {
            slot = &b[h2(key, cap, salt)];
        }

        if (!slot->used) {
            slot->used = true;
            slot->key = key;

            return true;
        }

        swap(key, slot->key);

        useFirst = !useFirst;
    }

    return false;
}

void CuckooHash::rebuild(int extraKey) {
    const int total = count + 1;

    int* keys = new int[total];

    int pos = 0;

    for (int i = 0; i < capacity; i++) {
        if (first[i].used) {
            keys[pos] = first[i].key;
            pos++;
        }

        if (second[i].used) {
            keys[pos] = second[i].key;
            pos++;
        }
    }

    keys[pos] = extraKey;

    int cap = capacity;
    unsigned int salt = seed;

    try {
        while (true) {
            if (cap > 4999999) {
                throw length_error("table too large");
            }

            cap = cap * 2 + 1;
            salt++;

            Slot* a = new Slot[cap]();
            Slot* b = nullptr;

            try {
                b = new Slot[cap]();
            }
            catch (...) {
                delete[] a;

                throw;
            }

            bool ok = true;

            for (int j = 0; j < total; j++) {
                if (!place(keys[j], a, b, cap, salt)) {
                    ok = false;

                    break;
                }
            }

            if (ok) {
                delete[] first;
                delete[] second;

                first = a;
                second = b;

                capacity = cap;
                seed = salt;
                count = total;

                break;
            }

            delete[] a;
            delete[] b;
        }
    }
    catch (...) {
        delete[] keys;

        throw;
    }

    delete[] keys;
}

bool CuckooHash::add(int key) {
    if (contains(key)) {
        return false;
    }

    int a = h1(key, capacity, seed);
    int b = h2(key, capacity, seed);

    if (!first[a].used) {
        first[a].used = true;
        first[a].key = key;

        count++;

        return true;
    }

    if (!second[b].used) {
        second[b].used = true;
        second[b].key = key;

        count++;

        return true;
    }

    Slot* copyFirst = new Slot[capacity];
    Slot* copySecond = nullptr;

    try {
        copySecond = new Slot[capacity];
    }
    catch (...) {
        delete[] copyFirst;

        throw;
    }

    for (int i = 0; i < capacity; i++) {
        copyFirst[i] = first[i];
        copySecond[i] = second[i];
    }

    bool placed = place(
        key,
        copyFirst,
        copySecond,
        capacity,
        seed
    );
    if (placed) {
        delete[] first;
        delete[] second;

        first = copyFirst;
        second = copySecond;

        count++;

        return true;
    }

    delete[] copyFirst;
    delete[] copySecond;

    rebuild(key);

    return true;
}

bool CuckooHash::remove(int key) {
    int a = h1(key, capacity, seed);
    int b = h2(key, capacity, seed);

    if (first[a].used && first[a].key == key) {
        first[a].used = false;

        count--;

        return true;
    }

    if (second[b].used && second[b].key == key) {
        second[b].used = false;

        count--;

        return true;
    }

    return false;
}

void CuckooHash::print(ostream& out) const {
    for (int i = 0; i < capacity; i++) {
        if (first[i].used) {
            out << ' ' << first[i].key;
        }

        if (second[i].used) {
            out << ' ' << second[i].key;
        }
    }
}

void CuckooHash::printTable(ostream& out) const {
    out << "CUCKOO (cuckoo hashing)\n";
    out << "Index  | Table 1   | Table 2\n";
    out << "-------+-----------+-----------\n";

    bool hasRows = false;

    for (int i = 0; i < capacity; i++) {
        if (!first[i].used && !second[i].used) {
            continue;
        }

        hasRows = true;

        out << setw(6) << i << " | ";

        if (first[i].used) {
            out << setw(9) << first[i].key;
        }
        else {
            out << setw(9) << "-";
        }

        out << " | ";

        if (second[i].used) {
            out << setw(9) << second[i].key;
        }
        else {
            out << setw(9) << "-";
        }

        out << '\n';
    }

    if (!hasRows) {
        out << "(table is empty)\n";
    }

    out << "Keys: " << count
        << ", capacity of each table: "
        << capacity << '\n';
}
// Сериализация метода цепочек

void ChainHash::saveText(ostream& out) const {
    out << capacity << ' ' << count << '\n';

    for (int i = 0; i < capacity; i++) {
        for (const FNode* p = buckets[i]; p; p = p->next) {
            out << p->value << '\n';
        }
    }

    if (!out) {
        throw runtime_error("write error");
    }
}

void ChainHash::loadText(istream& in) {
    int cap = 0;
    int n = 0;

    if (!(in >> cap >> n)) {
        throw runtime_error("read error");
    }

    if (cap <= 0 || cap > 10000000 || n < 0 || n > cap) {
        throw runtime_error("invalid hash table size");
    }

    ChainHash temp(cap);

    for (int i = 0; i < n; i++) {
        int key = 0;

        if (!(in >> key)) {
            throw runtime_error("read error");
        }

        if (!temp.add(key)) {
            throw runtime_error("duplicate key");
        }
    }

    swap(buckets, temp.buckets);
    swap(capacity, temp.capacity);
    swap(count, temp.count);
}

void ChainHash::saveBinary(ostream& out) const {
    writeInt(out, capacity);
    writeInt(out, count);

    for (int i = 0; i < capacity; i++) {
        for (const FNode* p = buckets[i]; p; p = p->next) {
            int key = stoi(p->value);

            writeInt(out, key);
        }
    }
}

void ChainHash::loadBinary(istream& in) {
    int cap = readInt(in);
    int n = readInt(in);

    if (cap <= 0 || cap > 10000000 || n < 0 || n > cap) {
        throw runtime_error("invalid hash table size");
    }

    ChainHash temp(cap);

    for (int i = 0; i < n; i++) {
        int key = readInt(in);

        if (!temp.add(key)) {
            throw runtime_error("duplicate key");
        }
    }

    swap(buckets, temp.buckets);
    swap(capacity, temp.capacity);
    swap(count, temp.count);
}
// Сериализация хеширования кукушки

void CuckooHash::saveText(ostream& out) const {
    out << capacity << ' ' << count << ' ' << seed << '\n';

    for (int i = 0; i < capacity; i++) {
        if (first[i].used) {
            out << 1 << ' ' << i << ' ' << first[i].key << '\n';
        }

        if (second[i].used) {
            out << 2 << ' ' << i << ' ' << second[i].key << '\n';
        }
    }

    if (!out) {
        throw runtime_error("write error");
    }
}

void CuckooHash::loadText(istream& in) {
    int cap = 0;
    int n = 0;
    unsigned int salt = 0;

    if (!(in >> cap >> n >> salt)) {
        throw runtime_error("read error");
    }

    if (cap <= 0 || cap > 10000000 || n < 0 || n > cap * 2) {
        throw runtime_error("invalid hash table size");
    }

    CuckooHash temp(cap);
    temp.seed = salt;

    for (int j = 0; j < n; j++) {
        int table = 0;
        int idx = 0;
        int key = 0;

        if (!(in >> table >> idx >> key)) {
            throw runtime_error("read error");
        }

        if (idx < 0 || idx >= cap) {
            throw runtime_error("invalid hash index");
        }

        if (temp.contains(key)) {
            throw runtime_error("duplicate key");
        }

        if (table == 1) {
            if (idx != temp.h1(key, cap, salt) ||
                temp.first[idx].used) {
                throw runtime_error("invalid hash position");
            }

            temp.first[idx].key = key;
            temp.first[idx].used = true;
        }
        else if (table == 2) {
            if (idx != temp.h2(key, cap, salt) ||
                temp.second[idx].used) {
                throw runtime_error("invalid hash position");
            }

            temp.second[idx].key = key;
            temp.second[idx].used = true;
        }
        else {
            throw runtime_error("invalid table number");
        }

        temp.count++;
    }

    swap(first, temp.first);
    swap(second, temp.second);
    swap(capacity, temp.capacity);
    swap(count, temp.count);
    swap(seed, temp.seed);
}

void CuckooHash::saveBinary(ostream& out) const {
    writeInt(out, capacity);
    writeInt(out, count);

    out.write(
        reinterpret_cast<const char*>(&seed),
        sizeof(seed)
    );

    if (!out) {
        throw runtime_error("write error");
    }

    for (int i = 0; i < capacity; i++) {
        if (first[i].used) {
            writeInt(out, 1);
            writeInt(out, i);
            writeInt(out, first[i].key);
        }

        if (second[i].used) {
            writeInt(out, 2);
            writeInt(out, i);
            writeInt(out, second[i].key);
        }
    }
}

void CuckooHash::loadBinary(istream& in) {
    int cap = readInt(in);
    int n = readInt(in);

    unsigned int salt = 0;

    in.read(
        reinterpret_cast<char*>(&salt),
        sizeof(salt)
    );

    if (!in) {
        throw runtime_error("read error");
    }

    if (cap <= 0 || cap > 10000000 || n < 0 || n > cap * 2) {
        throw runtime_error("invalid hash table size");
    }

    CuckooHash temp(cap);
    temp.seed = salt;

    for (int j = 0; j < n; j++) {
        int table = readInt(in);
        int idx = readInt(in);
        int key = readInt(in);

        if (idx < 0 || idx >= cap) {
            throw runtime_error("invalid hash index");
        }

        if (temp.contains(key)) {
            throw runtime_error("duplicate key");
        }

        if (table == 1) {
            if (idx != temp.h1(key, cap, salt) ||
                temp.first[idx].used) {
                throw runtime_error("invalid hash position");
            }

            temp.first[idx].key = key;
            temp.first[idx].used = true;
        }
        else if (table == 2) {
            if (idx != temp.h2(key, cap, salt) ||
                temp.second[idx].used) {
                throw runtime_error("invalid hash position");
            }

            temp.second[idx].key = key;
	    temp.second[idx].used = true;
        }
        else {
            throw runtime_error("invalid table number");
        }

        temp.count++;
    }

    swap(first, temp.first);
    swap(second, temp.second);
    swap(capacity, temp.capacity);
    swap(count, temp.count);
    swap(seed, temp.seed);
}
