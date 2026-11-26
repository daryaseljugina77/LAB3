#include "HashLab.h"
#include "Serialization.h"
#include <istream>
#include <ostream>
#include <iomanip>
#include <stdexcept>
#include <utility>

using namespace std;

unsigned int foldHash(const string& key) {
    unsigned int hash = 0;
    for (unsigned char c : key) {
        hash = hash * 31U + c;
    }
    return hash;
}

// Хеш-таблица методом цепочек.
ChainHash::HashNode::HashNode(const string& k, const string& v)
    : key(k), value(v), next(nullptr) {
}

ChainHash::ChainHash(int cap)
    : buckets(nullptr), capacity(cap), count(0) {
    if (cap <= 0 || cap > 10000000) {
        throw invalid_argument("Недопустимая вместимость таблицы");
    }
    buckets = new HashNode*[capacity]();
}

ChainHash::~ChainHash() {
    clear();
    delete[] buckets;
}

void ChainHash::clear() {
    for (int i = 0; i < capacity; i++) {
        HashNode* p = buckets[i];
        while (p) {
            HashNode* next = p->next;
            delete p;
            p = next;
        }
        buckets[i] = nullptr;
    }
    count = 0;
}

int ChainHash::index(const string& key) const {
    return static_cast<int>(foldHash(key) % static_cast<unsigned int>(capacity));
}

bool ChainHash::find(const string& key, string& value) const {
    for (const HashNode* p = buckets[index(key)]; p; p = p->next) {
        if (p->key == key) {
            value = p->value;
            return true;
        }
    }
    return false;
}

bool ChainHash::contains(const string& key) const {
    string value;
    return find(key, value);
}

void ChainHash::grow() {
    if (capacity > 4999999) {
        throw length_error("Слишком большая хеш-таблица");
    }
    int nextCap = capacity * 2 + 1;
    HashNode** fresh = new HashNode*[nextCap]();
    for (int i = 0; i < capacity; i++) {
        HashNode* p = buckets[i];
        while (p) {
            HashNode* next = p->next;
            int idx = static_cast<int>(
                foldHash(p->key) % static_cast<unsigned int>(nextCap)
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

bool ChainHash::add(const string& key, const string& value) {
    int idx = index(key);
    for (HashNode* p = buckets[idx]; p; p = p->next) {
        if (p->key == key) {
            p->value = value;
            return false;
        }
    }
    if (count >= capacity * 3 / 4) {
        grow();
        idx = index(key);
    }
    HashNode* p = new HashNode(key, value);
    p->next = buckets[idx];
    buckets[idx] = p;
    count++;
    return true;
}

bool ChainHash::remove(const string& key) {
    HashNode** link = &buckets[index(key)];
    while (*link) {
        if ((*link)->key == key) {
            HashNode* old = *link;
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
        for (const HashNode* p = buckets[i]; p; p = p->next) {
            out << ' ' << p->key << ':' << p->value;
        }
    }
}

void ChainHash::printTable(ostream& out) const {
    out << "CHAIN (метод цепочек)\n";
    out << "Индекс | Пары ключ-значение\n";
    out << "-------+--------------------------------\n";
    bool hasRows = false;
    for (int i = 0; i < capacity; i++) {
        if (!buckets[i]) {
            continue;
        }
        hasRows = true;
        out << setw(6) << i << " | ";
        for (const HashNode* p = buckets[i]; p; p = p->next) {
            if (p != buckets[i]) {
                out << " -> ";
            }
            out << '[' << p->key << ": " << p->value << ']';
        }
        out << '\n';
    }
    if (!hasRows) {
        out << "(таблица пуста)\n";
    }
    out << "Записей: " << count << ", вместимость: " << capacity << '\n';
}

// Сериализация метода цепочек.
void ChainHash::saveText(ostream& out) const {
    out << capacity << ' ' << count << '\n';
    for (int i = 0; i < capacity; i++) {
        for (const HashNode* p = buckets[i]; p; p = p->next) {
            out << quoted(p->key) << ' ' << quoted(p->value) << '\n';
        }
    }
    if (!out) {
        throw runtime_error("Ошибка записи");
    }
}

void ChainHash::loadText(istream& in) {
    int cap = 0;
    int n = 0;
    if (!(in >> cap >> n)) {
        throw runtime_error("Ошибка чтения");
    }
    if (cap <= 0 || cap > 10000000 || n < 0 || n > cap) {
        throw runtime_error("Недопустимый размер таблицы");
    }
    ChainHash temp(cap);
    for (int i = 0; i < n; i++) {
        string key;
        string value;
        if (!(in >> quoted(key) >> quoted(value))) {
            throw runtime_error("Ошибка чтения пары");
        }
        if (temp.contains(key)) {
            throw runtime_error("Повторяющийся ключ");
        }
        temp.add(key, value);
    }
    swap(buckets, temp.buckets);
    swap(capacity, temp.capacity);
    swap(count, temp.count);
}

void ChainHash::saveBinary(ostream& out) const {
    writeInt(out, capacity);
    writeInt(out, count);
    for (int i = 0; i < capacity; i++) {
        for (const HashNode* p = buckets[i]; p; p = p->next) {
            writeString(out, p->key);
            writeString(out, p->value);
        }
    }
}

void ChainHash::loadBinary(istream& in) {
    int cap = readInt(in);
    int n = readInt(in);
    if (cap <= 0 || cap > 10000000 || n < 0 || n > cap) {
        throw runtime_error("Недопустимый размер таблицы");
    }
    ChainHash temp(cap);
    for (int i = 0; i < n; i++) {
        string key = readString(in);
        string value = readString(in);
        if (temp.contains(key)) {
            throw runtime_error("Повторяющийся ключ");
        }
        temp.add(key, value);
    }
    swap(buckets, temp.buckets);
    swap(capacity, temp.capacity);
    swap(count, temp.count);
}

// Кукушкино хеширование.
CuckooHash::CuckooHash(int cap)
    : first(nullptr), second(nullptr), capacity(cap), count(0), seed(0) {
    if (cap <= 0 || cap > 10000000) {
        throw invalid_argument("Недопустимая вместимость таблицы");
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
        first[i] = Slot();
        second[i] = Slot();
    }
    count = 0;
    seed = 0;
}

int CuckooHash::h1(const string& key, int cap, unsigned int salt) const {
    return static_cast<int>(
        (foldHash(key) + salt) % static_cast<unsigned int>(cap)
    );
}

int CuckooHash::h2(const string& key, int cap, unsigned int salt) const {
    unsigned int hash = 2166136261U;
    for (unsigned char c : key) {
        hash = (hash ^ c) * 16777619U;
    }
    return static_cast<int>(
        (hash + salt) % static_cast<unsigned int>(cap)
    );
}

bool CuckooHash::find(const string& key, string& value) const {
    int a = h1(key, capacity, seed);
    int b = h2(key, capacity, seed);
    if (first[a].used && first[a].key == key) {
        value = first[a].value;
        return true;
    }
    if (second[b].used && second[b].key == key) {
        value = second[b].value;
        return true;
    }
    return false;
}

bool CuckooHash::contains(const string& key) const {
    string value;
    return find(key, value);
}

bool CuckooHash::place(Slot item, Slot* a, Slot* b, int cap, unsigned int salt) const {
    bool useFirst = true;
    for (int step = 0; step < cap * 2; step++) {
        Slot* slot;
        if (useFirst) {
            slot = &a[h1(item.key, cap, salt)];
        }
        else {
            slot = &b[h2(item.key, cap, salt)];
        }
        if (!slot->used) {
            *slot = item;
            slot->used = true;
            return true;
	    }
        swap(item, *slot);
        useFirst = !useFirst;
    }
    return false;
}

void CuckooHash::rebuild(const string& key, const string& value) {
    int total = count + 1;
    Slot* items = new Slot[total];
    int pos = 0;
    for (int i = 0; i < capacity; i++) {
        if (first[i].used) {
            items[pos++] = first[i];
        }
        if (second[i].used) {
            items[pos++] = second[i];
        }
    }
    items[pos].key = key;
    items[pos].value = value;
    items[pos].used = true;

    int cap = capacity;
    unsigned int salt = seed;
    try {
        while (true) {
            if (cap > 4999999) {
                throw length_error("Слишком большая хеш-таблица");
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
                if (!place(items[j], a, b, cap, salt)) {
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
        delete[] items;
        throw;
    }
    delete[] items;
}

bool CuckooHash::add(const string& key, const string& value) {
    int a = h1(key, capacity, seed);
    int b = h2(key, capacity, seed);
    if (first[a].used && first[a].key == key) {
        first[a].value = value;
        return false;
    }
    if (second[b].used && second[b].key == key) {
        second[b].value = value;
        return false;
    }

    Slot item;
    item.key = key;
    item.value = value;
    item.used = true;
    if (!first[a].used) {
        first[a] = item;
        count++;
        return true;
    }
    if (!second[b].used) {
        second[b] = item;
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
    bool placed = place(item, copyFirst, copySecond, capacity, seed);
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
    rebuild(key, value);
    return true;
}

bool CuckooHash::remove(const string& key) {
    int a = h1(key, capacity, seed);
    int b = h2(key, capacity, seed);
    if (first[a].used && first[a].key == key) {
        first[a] = Slot();
        count--;
        return true;
    }
    if (second[b].used && second[b].key == key) {
        second[b] = Slot();
        count--;
        return true;
    }
    return false;
}

void CuckooHash::print(ostream& out) const {
    for (int i = 0; i < capacity; i++) {
        if (first[i].used) {
            out << ' ' << first[i].key << ':' << first[i].value;
        }
        if (second[i].used) {
            out << ' ' << second[i].key << ':' << second[i].value;
        }
    }
}

void CuckooHash::printTable(ostream& out) const {
    out << "CUCKOO (кукушкино хеширование)\n";
    out << "Индекс | Таблица 1             | Таблица 2\n";
    out << "-------+-----------------------+-----------------------\n";
    bool hasRows = false;
    for (int i = 0; i < capacity; i++) {
        if (!first[i].used && !second[i].used) {
            continue;
        }
	hasRows = true;
        out << setw(6) << i << " | ";
        if (first[i].used) {
            out << '[' << first[i].key << ": " << first[i].value << ']';
        }
        else {
            out << '-';
        }
        out << " | ";
        if (second[i].used) {
            out << '[' << second[i].key << ": " << second[i].value << ']';
        }
        else {
            out << '-';
        }
        out << '\n';
    }
    if (!hasRows) {
        out << "(таблица пуста)\n";
    }
    out << "Записей: " << count << ", вместимость каждой таблицы: "
        << capacity << '\n';
}

// Сериализация кукушкиного хеширования.
void CuckooHash::saveText(ostream& out) const {
    out << capacity << ' ' << count << ' ' << seed << '\n';
    for (int i = 0; i < capacity; i++) {
        if (first[i].used) {
            out << 1 << ' ' << i << ' ' << quoted(first[i].key)
                << ' ' << quoted(first[i].value) << '\n';
        }
        if (second[i].used) {
            out << 2 << ' ' << i << ' ' << quoted(second[i].key)
                << ' ' << quoted(second[i].value) << '\n';
        }
    }
    if (!out) {
        throw runtime_error("Ошибка записи");
    }
}

void CuckooHash::loadText(istream& in) {
    int cap = 0;
    int n = 0;
    unsigned int salt = 0;
    if (!(in >> cap >> n >> salt)) {
        throw runtime_error("Ошибка чтения");
    }
    if (cap <= 0 || cap > 10000000 || n < 0 || n > cap * 2) {
        throw runtime_error("Недопустимый размер таблицы");
    }
    CuckooHash temp(cap);
    temp.seed = salt;
    for (int j = 0; j < n; j++) {
        int table = 0;
        int idx = 0;
        string key;
        string value;
        if (!(in >> table >> idx >> quoted(key) >> quoted(value))) {
            throw runtime_error("Ошибка чтения пары");
        }
        if (idx < 0 || idx >= cap || temp.contains(key)) {
            throw runtime_error("Некорректный индекс или повтор ключа");
        }
        if (table == 1) {
            if (idx != temp.h1(key, cap, salt) || temp.first[idx].used) {
                throw runtime_error("Некорректная позиция ключа");
            }
            temp.first[idx] = Slot{key, value, true};
        }
        else if (table == 2) {
            if (idx != temp.h2(key, cap, salt) || temp.second[idx].used) {
                throw runtime_error("Некорректная позиция ключа");
            }
            temp.second[idx] = Slot{key, value, true};
        }
        else {
            throw runtime_error("Некорректный номер таблицы");
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
    out.write(reinterpret_cast<const char*>(&seed), sizeof(seed));
    if (!out) {
        throw runtime_error("Ошибка записи");
    }
    for (int i = 0; i < capacity; i++) {
        if (first[i].used) {
            writeInt(out, 1);
            writeInt(out, i);
            writeString(out, first[i].key);
            writeString(out, first[i].value);
        }
        if (second[i].used) {
            writeInt(out, 2);
            writeInt(out, i);
            writeString(out, second[i].key);
            writeString(out, second[i].value);
        }
    }
}

void CuckooHash::loadBinary(istream& in) {
    int cap = readInt(in);
    int n = readInt(in);
    unsigned int salt = 0;
    in.read(reinterpret_cast<char*>(&salt), sizeof(salt));
    if (!in) {
        throw runtime_error("Ошибка чтения");
    }
    if (cap <= 0 || cap > 10000000 || n < 0 || n > cap * 2) {
        throw runtime_error("Недопустимый размер таблицы");
    }
    CuckooHash temp(cap);
    temp.seed = salt;
    for (int j = 0; j < n; j++) {
        int table = readInt(in);
        int idx = readInt(in);
        string key = readString(in);
        string value = readString(in);
	if (idx < 0 || idx >= cap || temp.contains(key)) {
            throw runtime_error("Некорректный индекс или повтор ключа");
        }
        if (table == 1) {
            if (idx != temp.h1(key, cap, salt) || temp.first[idx].used) {
                throw runtime_error("Некорректная позиция ключа");
            }
            temp.first[idx] = Slot{key, value, true};
        }
        else if (table == 2) {
            if (idx != temp.h2(key, cap, salt) || temp.second[idx].used) {
                throw runtime_error("Некорректная позиция ключа");
            }
            temp.second[idx] = Slot{key, value, true};
        }
        else {
            throw runtime_error("Некорректный номер таблицы");
        }
        temp.count++;
    }
    swap(first, temp.first);
    swap(second, temp.second);
    swap(capacity, temp.capacity);
    swap(count, temp.count);
    swap(seed, temp.seed);
}
