#include <iostream>
#include <string>
#include <fstream>
#include "Array.h"
#include "FList.h"
#include "DList.h"
#include "Stack.h"
#include "Queue.h"
#include "HashLab.h"
#include "CompleteTree.h"

using namespace std;

int main() {
    cout << "ARRAY" << endl;

    Array arr;

    arr.push("A");
    arr.push("B");
    arr.push("C");
    arr.print();

    cout << endl;


    cout << "FORWARD LIST" << endl;

    FList flist;

    flist.pushBack("A");
    flist.pushBack("B");
    flist.pushFront("C");
    flist.print();
    flist.printReverse();

    cout << endl;


    cout << "DOUBLY LINKED LIST" << endl;

    DList dlist;

    dlist.pushBack("A");
    dlist.pushBack("B");
    dlist.pushFront("C");
    dlist.print();
    dlist.printReverse();

    cout << endl;


    cout << "STACK" << endl;

    Stack stack;
    string value;

    stack.push("A");
    stack.push("B");
    stack.push("C");
    stack.print();

    if (stack.pop(value)) {
        cout << "Removed: " << value << endl;
    }

    stack.print();

    cout << endl;


    cout << "QUEUE" << endl;

    Queue queue;

    queue.push("A");
    queue.push("B");
    queue.push("C");
    queue.print();

    if (queue.pop(value)) {
        cout << "Removed: " << value << endl;
    }

    queue.print();

    cout << endl;


    cout << "CHAIN HASH" << endl;

    ChainHash chain;

    chain.add(10);
    chain.add(111);
    chain.add(212);
    chain.printTable(cout);

    cout << endl;


    cout << "CUCKOO HASH" << endl;

    CuckooHash cuckoo;

    cuckoo.add(10);
    cuckoo.add(111);
    cuckoo.add(212);
    cuckoo.printTable(cout);

    cout << endl;


    cout << "COMPLETE BINARY TREE" << endl;

    CompleteTree tree;

    tree.insert("A");
    tree.insert("B");
    tree.insert("C");
    tree.insert("D");
    tree.insert("E");
    tree.insert("F");

    cout << "Preorder: ";
    tree.printPre();

    cout << "Inorder: ";
    tree.printIn();

    cout << "Postorder: ";
    tree.printPost();

    cout << "Level order: ";
    tree.printLevel();

    cout << "Size: " << tree.getSize() << endl;
    cout << "Complete: " << boolalpha
         << tree.isComplete() << endl;
    cout << endl;
    cout << "ARRAY SERIALIZATION" << endl;

    Array original;

    original.push("Hello");
    original.push("Hello world");
    original.push("");

    cout << "Original array:" << endl;
    original.print();

    // Текстовая сериализация

    ofstream textOut("array.txt");

    original.saveText(textOut);
    textOut.close();

    Array fromText;

    ifstream textIn("array.txt");

    fromText.loadText(textIn);
    textIn.close();

    cout << "Loaded from text:" << endl;
    fromText.print();
    cout << "Size: " << fromText.getSize() << endl;

    // Бинарная сериализация

    ofstream binaryOut("array.bin", ios::binary);

    original.saveBinary(binaryOut);
    binaryOut.close();

    Array fromBinary;

    ifstream binaryIn("array.bin", ios::binary);

    fromBinary.loadBinary(binaryIn);
    binaryIn.close();

    cout << "Loaded from binary:" << endl;
    fromBinary.print();
    cout << "Size: " << fromBinary.getSize() << endl;

    // Проверка совпадения элементов

    bool equal = original.getSize() == fromText.getSize() &&
                 original.getSize() == fromBinary.getSize();

    for (int i = 0; i < original.getSize() && equal; i++) {
        string a;
        string b;
        string c;

        original.get(i, a);
        fromText.get(i, b);
        fromBinary.get(i, c);

        if (a != b || a != c) {
            equal = false;
        }
    }

    cout << "Data restored correctly: "
         << boolalpha << equal << endl;
    cout << endl;
    cout << "FLIST SERIALIZATION" << endl;

    FList originalList;

    originalList.pushBack("A");
    originalList.pushBack("Hello world");
    originalList.pushBack("");

    cout << "Original list:" << endl;
    originalList.print();

    // Текстовая сериализация

    ofstream listTextOut("flist.txt");

    originalList.saveText(listTextOut);
    listTextOut.close();

    FList listFromText;

    ifstream listTextIn("flist.txt");

    listFromText.loadText(listTextIn);
    listTextIn.close();

    cout << "Loaded from text:" << endl;
    listFromText.print();

    // Бинарная сериализация

    ofstream listBinaryOut("flist.bin", ios::binary);

    originalList.saveBinary(listBinaryOut);
    listBinaryOut.close();

    FList listFromBinary;

    ifstream listBinaryIn("flist.bin", ios::binary);

    listFromBinary.loadBinary(listBinaryIn);
    listBinaryIn.close();

    cout << "Loaded from binary:" << endl;
    listFromBinary.print();

    cout << "Text contains Hello world: "
         << boolalpha
         << listFromText.find("Hello world")
         << endl;

    cout << "Binary contains Hello world: "
         << boolalpha
         << listFromBinary.find("Hello world")
         << endl;
    cout << endl;
    cout << "DLIST SERIALIZATION" << endl;

    DList originalDList;

    originalDList.pushBack("A");
    originalDList.pushBack("Hello world");
    originalDList.pushBack("C");

    cout << "Original forward:" << endl;
    originalDList.print();

    cout << "Original backward:" << endl;
    originalDList.printReverse();

    // Текстовая сериализация

    ofstream dlistTextOut("dlist.txt");

    originalDList.saveText(dlistTextOut);
    dlistTextOut.close();

    DList dlistFromText;

    ifstream dlistTextIn("dlist.txt");

    dlistFromText.loadText(dlistTextIn);
    dlistTextIn.close();

    cout << "Loaded from text (forward):" << endl;
    dlistFromText.print();

    cout << "Loaded from text (backward):" << endl;
    dlistFromText.printReverse();

    // Бинарная сериализация

    ofstream dlistBinaryOut("dlist.bin", ios::binary);

    originalDList.saveBinary(dlistBinaryOut);
    dlistBinaryOut.close();

    DList dlistFromBinary;

    ifstream dlistBinaryIn("dlist.bin", ios::binary);

    dlistFromBinary.loadBinary(dlistBinaryIn);
    dlistBinaryIn.close();

    cout << "Loaded from binary (forward):" << endl;
    dlistFromBinary.print();

    cout << "Loaded from binary (backward):" << endl;
    dlistFromBinary.printReverse();

    cout << "Text contains Hello world: "
         << boolalpha
         << dlistFromText.find("Hello world")
         << endl;

    cout << "Binary contains Hello world: "
         << boolalpha
         << dlistFromBinary.find("Hello world")
         << endl;
    cout << endl;
    cout << "STACK SERIALIZATION" << endl;

    Stack originalStack;

    originalStack.push("A");
    originalStack.push("Hello world");
    originalStack.push("C");

    cout << "Original stack:" << endl;
    originalStack.print();

    // Текстовая сериализация

    ofstream stackTextOut("stack.txt");

    originalStack.saveText(stackTextOut);
    stackTextOut.close();

    Stack stackFromText;

    ifstream stackTextIn("stack.txt");

    stackFromText.loadText(stackTextIn);
    stackTextIn.close();

    cout << "Loaded from text:" << endl;
    stackFromText.print();

    // Бинарная сериализация

    ofstream stackBinaryOut("stack.bin", ios::binary);

    originalStack.saveBinary(stackBinaryOut);
    stackBinaryOut.close();

    Stack stackFromBinary;

    ifstream stackBinaryIn("stack.bin", ios::binary);

    stackFromBinary.loadBinary(stackBinaryIn);
    stackBinaryIn.close();

    cout << "Loaded from binary:" << endl;
    stackFromBinary.print();

    // Проверка порядка удаления

    string textValue;
    string binaryValue;

    bool textPop = stackFromText.pop(textValue);
    bool binaryPop = stackFromBinary.pop(binaryValue);

    cout << "Text pop: ";

    if (textPop) {
        cout << textValue << endl;
    }
    else {
        cout << "Stack is empty" << endl;
    }

    cout << "Binary pop: ";

    if (binaryPop) {
        cout << binaryValue << endl;
    }
    else {
        cout << "Stack is empty" << endl;
    }

    bool correct = textPop && binaryPop &&
                   textValue == "C" &&
                   binaryValue == "C";

    cout << "Stack order restored correctly: "
         << boolalpha << correct << endl;
    cout << endl;
    cout << "QUEUE SERIALIZATION" << endl;

    Queue originalQueue;

    originalQueue.push("A");
    originalQueue.push("Hello world");
    originalQueue.push("C");

    cout << "Original queue:" << endl;
    originalQueue.print();

    // Текстовая сериализация

    ofstream queueTextOut("queue.txt");

    originalQueue.saveText(queueTextOut);
    queueTextOut.close();

    Queue queueFromText;

    ifstream queueTextIn("queue.txt");

    queueFromText.loadText(queueTextIn);
    queueTextIn.close();

    cout << "Loaded from text:" << endl;
    queueFromText.print();

    // Бинарная сериализация

    ofstream queueBinaryOut("queue.bin", ios::binary);

    originalQueue.saveBinary(queueBinaryOut);
    queueBinaryOut.close();

    Queue queueFromBinary;

    ifstream queueBinaryIn("queue.bin", ios::binary);

    queueFromBinary.loadBinary(queueBinaryIn);
    queueBinaryIn.close();

    cout << "Loaded from binary:" << endl;
    queueFromBinary.print();

    // Проверка порядка удаления

    string textQueueValue;
    string binaryQueueValue;

    bool textQueuePop = queueFromText.pop(textQueueValue);
    bool binaryQueuePop = queueFromBinary.pop(binaryQueueValue);

    cout << "Text pop: ";

    if (textQueuePop) {
        cout << textQueueValue << endl;
    }
    else {
        cout << "Queue is empty" << endl;
    }

    cout << "Binary pop: ";

    if (binaryQueuePop) {
        cout << binaryQueueValue << endl;
    }
    else {
        cout << "Queue is empty" << endl;
    }

    bool queueCorrect = textQueuePop && binaryQueuePop &&
                        textQueueValue == "A" &&
                        binaryQueueValue == "A";

    cout << "Queue order restored correctly: "
         << boolalpha << queueCorrect << endl;
    cout << endl;
    cout << "CHAIN HASH SERIALIZATION" << endl;

    ChainHash originalChain;

    originalChain.add(10);
    originalChain.add(111);
    originalChain.add(212);
    originalChain.add(-5);

    cout << "Original table:" << endl;
    originalChain.printTable(cout);

    ofstream chainTextOut("chain.txt");
    originalChain.saveText(chainTextOut);
    chainTextOut.close();

    ChainHash chainFromText;

    ifstream chainTextIn("chain.txt");
    chainFromText.loadText(chainTextIn);
    chainTextIn.close();

    cout << "Loaded from text:" << endl;
    chainFromText.printTable(cout);

    ofstream chainBinaryOut("chain.bin", ios::binary);
    originalChain.saveBinary(chainBinaryOut);
    chainBinaryOut.close();

    ChainHash chainFromBinary;

    ifstream chainBinaryIn("chain.bin", ios::binary);
    chainFromBinary.loadBinary(chainBinaryIn);
    chainBinaryIn.close();

    cout << "Loaded from binary:" << endl;
    chainFromBinary.printTable(cout);

    bool chainCorrect =
        chainFromText.contains(10) &&
        chainFromText.contains(111) &&
        chainFromText.contains(212) &&
        chainFromText.contains(-5) &&
        chainFromBinary.contains(10) &&
        chainFromBinary.contains(111) &&
        chainFromBinary.contains(212) &&
        chainFromBinary.contains(-5);

    cout << "Chain hash restored correctly: "
         << boolalpha << chainCorrect << endl;

    cout << endl;
    cout << "CUCKOO HASH SERIALIZATION" << endl;

    CuckooHash originalCuckoo;

    originalCuckoo.add(10);
    originalCuckoo.add(111);
    originalCuckoo.add(212);
    originalCuckoo.add(-5);

    cout << "Original table:" << endl;
    originalCuckoo.printTable(cout);

    ofstream cuckooTextOut("cuckoo.txt");
    originalCuckoo.saveText(cuckooTextOut);
    cuckooTextOut.close();

    CuckooHash cuckooFromText;

    ifstream cuckooTextIn("cuckoo.txt");
    cuckooFromText.loadText(cuckooTextIn);
    cuckooTextIn.close();

    cout << "Loaded from text:" << endl;
    cuckooFromText.printTable(cout);

    ofstream cuckooBinaryOut("cuckoo.bin", ios::binary);
    originalCuckoo.saveBinary(cuckooBinaryOut);
    cuckooBinaryOut.close();

    CuckooHash cuckooFromBinary;

    ifstream cuckooBinaryIn("cuckoo.bin", ios::binary);
    cuckooFromBinary.loadBinary(cuckooBinaryIn);
    cuckooBinaryIn.close();

    cout << "Loaded from binary:" << endl;
    cuckooFromBinary.printTable(cout);

    bool cuckooCorrect =
        cuckooFromText.contains(10) &&
        cuckooFromText.contains(111) &&
        cuckooFromText.contains(212) &&
        cuckooFromText.contains(-5) &&
        cuckooFromBinary.contains(10) &&
        cuckooFromBinary.contains(111) &&
        cuckooFromBinary.contains(212) &&
        cuckooFromBinary.contains(-5);

    cout << "Cuckoo hash restored correctly: "
         << boolalpha << cuckooCorrect << endl;

    cout << endl;
    cout << "COMPLETE TREE SERIALIZATION" << endl;

    CompleteTree originalTree;

    originalTree.insert("A");
    originalTree.insert("B");
    originalTree.insert("C");
    originalTree.insert("D");
    originalTree.insert("Hello world");
    originalTree.insert("F");

    cout << "Original tree:" << endl;

    cout << "Level order: ";
    originalTree.printLevel();

    cout << "Preorder: ";
    originalTree.printPre();

    cout << "Inorder: ";
    originalTree.printIn();

    cout << "Postorder: ";
    originalTree.printPost();

    // Текстовая сериализация

    ofstream treeTextOut("tree.txt");

    originalTree.saveText(treeTextOut);
    treeTextOut.close();

    CompleteTree treeFromText;

    ifstream treeTextIn("tree.txt");

    treeFromText.loadText(treeTextIn);
    treeTextIn.close();

    cout << "Loaded from text:" << endl;

    cout << "Level order: ";
    treeFromText.printLevel();

    cout << "Preorder: ";
    treeFromText.printPre();

    cout << "Inorder: ";
    treeFromText.printIn();

    cout << "Postorder: ";
    treeFromText.printPost();

    // Бинарная сериализация

    ofstream treeBinaryOut("tree.bin", ios::binary);

    originalTree.saveBinary(treeBinaryOut);
    treeBinaryOut.close();

    CompleteTree treeFromBinary;

    ifstream treeBinaryIn("tree.bin", ios::binary);

    treeFromBinary.loadBinary(treeBinaryIn);
    treeBinaryIn.close();

    cout << "Loaded from binary:" << endl;

    cout << "Level order: ";
    treeFromBinary.printLevel();

    cout << "Preorder: ";
    treeFromBinary.printPre();

    cout << "Inorder: ";
    treeFromBinary.printIn();

    cout << "Postorder: ";
    treeFromBinary.printPost();

    bool treeCorrect =
        treeFromText.getSize() == 6 &&
        treeFromBinary.getSize() == 6 &&
        treeFromText.isComplete() &&
        treeFromBinary.isComplete() &&
        treeFromText.find("Hello world") &&
        treeFromBinary.find("Hello world");

    cout << "Tree restored correctly: "
         << boolalpha << treeCorrect << endl;

    return 0;
}
