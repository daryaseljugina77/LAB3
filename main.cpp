#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <exception>
#include <stdexcept>

#include "Array.h"
#include "FList.h"
#include "DList.h"
#include "Stack.h"
#include "Queue.h"
#include "HashLab.h"
#include "CompleteTree.h"

using namespace std;

template <typename T>
void loadData(T& data, const string& name) {
    ifstream textFile(name + ".txt");

    if (textFile.is_open()) {
        data.loadText(textFile);
        return;
    }

    ifstream binaryFile(name + ".bin", ios::binary);

    if (binaryFile.is_open()) {
        data.loadBinary(binaryFile);
    }
}

template <typename T>
void saveData(const T& data, const string& name) {
    ofstream textFile(name + ".txt");

    if (!textFile) {
        throw runtime_error("Cannot open text file");
    }

    data.saveText(textFile);
    textFile.close();

    if (!textFile) {
        throw runtime_error("Cannot save text file");
    }

    ofstream binaryFile(name + ".bin", ios::binary);

    if (!binaryFile) {
        throw runtime_error("Cannot open binary file");
    }

    data.saveBinary(binaryFile);
    binaryFile.close();

    if (!binaryFile) {
        throw runtime_error("Cannot save binary file");
    }
}

bool handleArray(Array& a, const string& cmd, istringstream& in) {
    string value;
    int index;

    if (cmd == "push" && in >> quoted(value)) {
        a.push(value);
        return true;
    }
    if (cmd == "insert" && in >> index >> quoted(value)) {
        return a.insert(index, value);
    }
    if (cmd == "get" && in >> index) {
        if (a.get(index, value)) {
            cout << value << endl;
        }
        else {
            cout << "Invalid index" << endl;
        }
        return false;
    }
    if (cmd == "set" && in >> index >> quoted(value)) {
        return a.set(index, value);
    }
    if (cmd == "erase" && in >> index) {
        return a.erase(index);
    }
    if (cmd == "size") {
        cout << a.getSize() << endl;
        return false;
    }
    if (cmd == "print") {
        a.print();
        return false;
    }
    if (cmd == "clear") {
        a.clear();
        return true;
    }

    cout << "Unknown command or invalid arguments" << endl;
    return false;
}

template <typename List>
bool handleList(List& list, const string& cmd, istringstream& in) {
    string value;
    string target;

    if (cmd == "pushback" && in >> quoted(value)) {
        list.pushBack(value);
        return true;
    }
    if (cmd == "pushfront" && in >> quoted(value)) {
        list.pushFront(value);
        return true;
    }
    if (cmd == "before" && in >> quoted(target) >> quoted(value)) {
        return list.before(target, value);
    }
    if (cmd == "after" && in >> quoted(target) >> quoted(value)) {
        return list.after(target, value);
    }
    if (cmd == "erasehead") {
        return list.eraseHead();
    }
    if (cmd == "erasetail") {
        return list.eraseTail();
    }
    if (cmd == "erasebefore" && in >> quoted(target)) {
        return list.eraseBefore(target);
    }
    if (cmd == "eraseafter" && in >> quoted(target)) {
        return list.eraseAfter(target);
    }
    if (cmd == "erasevalue" && in >> quoted(value)) {
        return list.eraseValue(value);
    }
    if (cmd == "find" && in >> quoted(value)) {
        cout << boolalpha << list.find(value) << endl;
        return false;
    }
    if (cmd == "print") {
        list.print();
        return false;
    }
    if (cmd == "reverse") {
        list.printReverse();
        return false;
    }
    if (cmd == "clear") {
        list.clear();
        return true;
    }

    cout << "Unknown command or invalid arguments" << endl;
    return false;
}

template <typename Container>
bool handlePushPop(Container& data, const string& cmd, istringstream& in) {
    string value;

    if (cmd == "push" && in >> quoted(value)) {
        data.push(value);
        return true;
    }
    if (cmd == "pop") {
	    if (data.pop(value)) {
            cout << "Removed: " << value << endl;
            return true;
        }
        cout << "Container is empty" << endl;
        return false;
    }
    if (cmd == "print") {
        data.print();
        return false;
    }
    if (cmd == "clear") {
        data.clear();
        return true;
    }

    cout << "Unknown command or invalid arguments" << endl;
    return false;
}

template <typename Hash>
bool handleHash(Hash& table, const string& cmd, istringstream& in) {
    int key;

    if (cmd == "add" && in >> key) {
        return table.add(key);
    }
    if (cmd == "remove" && in >> key) {
        return table.remove(key);
    }
    if (cmd == "contains" && in >> key) {
        cout << boolalpha << table.contains(key) << endl;
        return false;
    }
    if (cmd == "print") {
        table.print(cout);
        cout << endl;
        return false;
    }
    if (cmd == "table") {
        table.printTable(cout);
        return false;
    }
    if (cmd == "clear") {
        table.clear();
        return true;
    }

    cout << "Unknown command or invalid arguments" << endl;
    return false;
}

bool handleTree(CompleteTree& tree, const string& cmd, istringstream& in) {
    string value;

    if (cmd == "insert" && in >> quoted(value)) {
        tree.insert(value);
        return true;
    }
    if (cmd == "find" && in >> quoted(value)) {
        cout << boolalpha << tree.find(value) << endl;
        return false;
    }
    if (cmd == "complete") {
        cout << boolalpha << tree.isComplete() << endl;
        return false;
    }
    if (cmd == "size") {
        cout << tree.getSize() << endl;
        return false;
    }
    if (cmd == "pre") {
        tree.printPre();
        return false;
    }
    if (cmd == "in") {
        tree.printIn();
        return false;
    }
    if (cmd == "post") {
        tree.printPost();
        return false;
    }
    if (cmd == "level" || cmd == "print") {
        tree.printLevel();
        return false;
    }
    if (cmd == "clear") {
        tree.clear();
        return true;
    }

    cout << "Unknown command or invalid arguments" << endl;
    return false;
}

template <typename T, typename Handler>
void runSession(T& data, const string& name, const string& commands, Handler handler) {
    try {
        loadData(data, name);
    }
    catch (const exception& e) {
        cout << "Load error: " << e.what() << endl;
    }

    cout << "Commands: " << commands << endl;
    cout << "Use quotes for strings with spaces, e.g. push \"Hello world\"" << endl;
    cout << "Type back to choose another structure" << endl;

    string line;

    while (true) {
        cout << name << "> ";

        if (!getline(cin, line)) {
            return;
        }

        istringstream input(line);
        string command;
        input >> command;

        if (command == "back") {
            return;
        }
        if (command == "help") {
            cout << "Commands: " << commands << endl;
            continue;
        }
        if (command.empty()) {
            continue;
        }

        try {
            bool changed = handler(data, command, input);

            if (changed) {
                saveData(data, name);
                cout << "Saved" << endl;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
}

int main() {
    cout << "Data structures - LAB3" << endl;
    cout << "array, flist, dlist, stack, queue, chain, cuckoo, tree" << endl;
    cout << "Type exit to finish" << endl;

    string name;

    while (true) {
        cout << "Select structure: ";

        if (!getline(cin, name) || name == "exit") {
            break;
        }

        if (name == "array") {
            Array data;
            runSession(data, "array", "push, insert, get, set, erase, size, print, clear, back", handleArray);
        }
        else if (name == "flist") {
            FList data;
	    runSession(data, "flist", "pushback, pushfront, before, after, erasehead, erasetail, erasebefore, eraseafter, erasevalue, find, print, reverse, clear, back", handleList<FList>);
        }
        else if (name == "dlist") {
            DList data;
            runSession(data, "dlist", "pushback, pushfront, before, after, erasehead, erasetail, erasebefore, eraseafter, erasevalue, find, print, reverse, clear, back", handleList<DList>);
        }
        else if (name == "stack") {
            Stack data;
            runSession(data, "stack", "push, pop, print, clear, back", handlePushPop<Stack>);
        }
        else if (name == "queue") {
            Queue data;
            runSession(data, "queue", "push, pop, print, clear, back", handlePushPop<Queue>);
        }
        else if (name == "chain") {
            ChainHash data;
            runSession(data, "chain", "add, remove, contains, print, table, clear, back", handleHash<ChainHash>);
        }
        else if (name == "cuckoo") {
            CuckooHash data;
            runSession(data, "cuckoo", "add, remove, contains, print, table, clear, back", handleHash<CuckooHash>);
        }
        else if (name == "tree") {
            CompleteTree data;
            runSession(data, "tree", "insert, find, complete, size, pre, in, post, level, print, clear, back", handleTree);
        }
        else {
            cout << "Unknown structure" << endl;
        }
    }

    return 0;
}
