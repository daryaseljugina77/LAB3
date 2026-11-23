#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "Array.h"
#include "FList.h"
#include "DList.h"
#include "Stack.h"
#include "Queue.h"
#include "HashLab.h"
#include "CompleteTree.h"

using namespace std;

TEST(ArrayTest, EmptyArray) {
    Array arr;
    string value;

    EXPECT_EQ(arr.getSize(), 0);
    EXPECT_FALSE(arr.get(0, value));
    EXPECT_FALSE(arr.erase(0));
    EXPECT_FALSE(arr.set(0, "A"));
}

TEST(ArrayTest, PushAndGet) {
    Array arr;
    string value;

    arr.push("A");
    arr.push("B");
    arr.push("C");

    EXPECT_EQ(arr.getSize(), 3);

    EXPECT_TRUE(arr.get(0, value));
    EXPECT_EQ(value, "A");

    EXPECT_TRUE(arr.get(1, value));
    EXPECT_EQ(value, "B");

    EXPECT_TRUE(arr.get(2, value));
    EXPECT_EQ(value, "C");
}

TEST(ArrayTest, InsertAndErase) {
    Array arr;
    string value;

    arr.push("A");
    arr.push("C");

    EXPECT_TRUE(arr.insert(1, "B"));
    EXPECT_EQ(arr.getSize(), 3);

    EXPECT_TRUE(arr.get(1, value));
    EXPECT_EQ(value, "B");

    EXPECT_TRUE(arr.erase(1));
    EXPECT_EQ(arr.getSize(), 2);

    EXPECT_TRUE(arr.get(1, value));
    EXPECT_EQ(value, "C");
}

TEST(ArrayTest, SetValue) {
    Array arr;
    string value;

    arr.push("A");

    EXPECT_TRUE(arr.set(0, "B"));
    EXPECT_TRUE(arr.get(0, value));
    EXPECT_EQ(value, "B");
}

TEST(ArrayTest, InvalidIndexes) {
    Array arr;
    string value;

    arr.push("A");

    EXPECT_FALSE(arr.insert(-1, "B"));
    EXPECT_FALSE(arr.insert(2, "B"));

    EXPECT_FALSE(arr.get(-1, value));
    EXPECT_FALSE(arr.get(1, value));

    EXPECT_FALSE(arr.set(-1, "B"));
    EXPECT_FALSE(arr.set(1, "B"));

    EXPECT_FALSE(arr.erase(-1));
    EXPECT_FALSE(arr.erase(1));
}

TEST(ArrayTest, ClearArray) {
    Array arr;

    arr.push("A");
    arr.push("B");

    arr.clear();

    EXPECT_EQ(arr.getSize(), 0);

    arr.push("C");

    EXPECT_EQ(arr.getSize(), 1);
}

TEST(ArrayTest, TextSerialization) {
    Array arr;
    stringstream stream;

    arr.push("A");
    arr.push("Hello world");
    arr.push("");

    arr.saveText(stream);

    Array loaded;
    loaded.loadText(stream);

    EXPECT_EQ(loaded.getSize(), 3);

    string value;

    EXPECT_TRUE(loaded.get(0, value));
    EXPECT_EQ(value, "A");

    EXPECT_TRUE(loaded.get(1, value));
    EXPECT_EQ(value, "Hello world");

    EXPECT_TRUE(loaded.get(2, value));
    EXPECT_EQ(value, "");
}

TEST(ArrayTest, BinarySerialization) {
    Array arr;
    stringstream stream(ios::in | ios::out | ios::binary);

    arr.push("A");
    arr.push("Hello world");
    arr.push("");

    arr.saveBinary(stream);

    Array loaded;
    loaded.loadBinary(stream);

    EXPECT_EQ(loaded.getSize(), 3);

    string value;

    EXPECT_TRUE(loaded.get(0, value));
    EXPECT_EQ(value, "A");

    EXPECT_TRUE(loaded.get(1, value));
    EXPECT_EQ(value, "Hello world");

    EXPECT_TRUE(loaded.get(2, value));
    EXPECT_EQ(value, "");
}

TEST(ArrayTest, InvalidTextData) {
    Array arr;
    arr.push("Original");

    stringstream stream("-1");

    EXPECT_THROW(arr.loadText(stream), runtime_error);

    string value;

    EXPECT_TRUE(arr.get(0, value));
    EXPECT_EQ(value, "Original");
}

TEST(ArrayTest, InvalidBinaryData) {
    Array arr;
    arr.push("Original");

    stringstream stream(ios::in | ios::out | ios::binary);

    int invalidSize = -1;

    stream.write(
        reinterpret_cast<const char*>(&invalidSize),
        sizeof(invalidSize)
    );

    EXPECT_THROW(arr.loadBinary(stream), runtime_error);

    string value;

    EXPECT_TRUE(arr.get(0, value));
    EXPECT_EQ(value, "Original");
}

TEST(FListTest, EmptyList) {
    FList list;

    EXPECT_FALSE(list.find("A"));
    EXPECT_FALSE(list.eraseHead());
    EXPECT_FALSE(list.eraseTail());
    EXPECT_FALSE(list.eraseValue("A"));
    EXPECT_FALSE(list.eraseBefore("A"));
    EXPECT_FALSE(list.eraseAfter("A"));
}

TEST(FListTest, PushFrontAndBack) {
    FList list;

    list.pushBack("B");
    list.pushFront("A");
    list.pushBack("C");

    EXPECT_TRUE(list.find("A"));
    EXPECT_TRUE(list.find("B"));
    EXPECT_TRUE(list.find("C"));
    EXPECT_FALSE(list.find("D"));
}

TEST(FListTest, InsertBeforeAndAfter) {
    FList list;

    list.pushBack("A");
    list.pushBack("C");

    EXPECT_TRUE(list.before("C", "B"));
    EXPECT_TRUE(list.after("C", "D"));

    EXPECT_TRUE(list.find("B"));
    EXPECT_TRUE(list.find("D"));

    EXPECT_FALSE(list.before("X", "E"));
    EXPECT_FALSE(list.after("X", "E"));
    EXPECT_FALSE(list.find("E"));
}

TEST(FListTest, EraseHeadAndTail) {
    FList list;

    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");

    EXPECT_TRUE(list.eraseHead());
    EXPECT_FALSE(list.find("A"));

    EXPECT_TRUE(list.eraseTail());
    EXPECT_FALSE(list.find("C"));

    EXPECT_TRUE(list.find("B"));

    EXPECT_TRUE(list.eraseTail());
    EXPECT_FALSE(list.eraseTail());
}

TEST(FListTest, EraseBeforeAndAfter) {
    FList list;

    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");
    list.pushBack("D");

    EXPECT_TRUE(list.eraseBefore("C"));
    EXPECT_FALSE(list.find("B"));

    EXPECT_TRUE(list.eraseAfter("C"));
    EXPECT_FALSE(list.find("D"));

    EXPECT_TRUE(list.find("A"));
    EXPECT_TRUE(list.find("C"));

    EXPECT_FALSE(list.eraseBefore("A"));
    EXPECT_FALSE(list.eraseAfter("C"));
    EXPECT_FALSE(list.eraseBefore("X"));
    EXPECT_FALSE(list.eraseAfter("X"));
}

TEST(FListTest, EraseByValue) {
    FList list;

    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");

    EXPECT_TRUE(list.eraseValue("B"));
    EXPECT_FALSE(list.find("B"));

    EXPECT_TRUE(list.eraseValue("A"));
    EXPECT_TRUE(list.eraseValue("C"));

    EXPECT_FALSE(list.eraseValue("A"));
}

TEST(FListTest, ClearList) {
    FList list;

    list.pushBack("A");
    list.pushBack("B");

    list.clear();

    EXPECT_FALSE(list.find("A"));
    EXPECT_FALSE(list.find("B"));

    list.pushBack("C");

    EXPECT_TRUE(list.find("C"));
}

TEST(FListTest, TextSerialization) {
    FList list;
    stringstream stream;

    list.pushBack("A");
    list.pushBack("Hello world");
    list.pushBack("");

    list.saveText(stream);

    FList loaded;
    loaded.loadText(stream);

    stringstream result;
    loaded.saveText(result);

    EXPECT_EQ(result.str(), stream.str());
}

TEST(FListTest, BinarySerialization) {
    FList list;
    stringstream stream(ios::in | ios::out | ios::binary);

    list.pushBack("A");
    list.pushBack("Hello world");
    list.pushBack("");

    list.saveBinary(stream);

    FList loaded;
    loaded.loadBinary(stream);

    stringstream result(ios::in | ios::out | ios::binary);
    loaded.saveBinary(result);

    EXPECT_EQ(result.str(), stream.str());
}

TEST(FListTest, InvalidTextData) {
    FList list;

    list.pushBack("Original");

    stringstream stream("-1");

    EXPECT_THROW(list.loadText(stream), runtime_error);

    EXPECT_TRUE(list.find("Original"));
}

TEST(FListTest, InvalidBinaryData) {
    FList list;

    list.pushBack("Original");

    stringstream stream(ios::in | ios::out | ios::binary);

    int invalidSize = -1;

    stream.write(
        reinterpret_cast<const char*>(&invalidSize),
        sizeof(invalidSize)
    );

    EXPECT_THROW(list.loadBinary(stream), runtime_error);

    EXPECT_TRUE(list.find("Original"));
}

// Тестирование двусвязного списка

TEST(DListTest, EmptyList) {
    DList list;

    EXPECT_FALSE(list.find("A"));
    EXPECT_FALSE(list.eraseHead());
    EXPECT_FALSE(list.eraseTail());
    EXPECT_FALSE(list.eraseValue("A"));
    EXPECT_FALSE(list.eraseBefore("A"));
    EXPECT_FALSE(list.eraseAfter("A"));
}

TEST(DListTest, PushFrontAndBack) {
    DList list;

    list.pushBack("B");
    list.pushFront("A");
    list.pushBack("C");

    EXPECT_TRUE(list.find("A"));
    EXPECT_TRUE(list.find("B"));
    EXPECT_TRUE(list.find("C"));
    EXPECT_FALSE(list.find("D"));

    stringstream stream;
    list.saveText(stream);

    EXPECT_EQ(stream.str(), "3\n\"A\"\n\"B\"\n\"C\"\n");
}

TEST(DListTest, InsertBeforeAndAfter) {
    DList list;

    list.pushBack("A");
    list.pushBack("C");

    EXPECT_TRUE(list.before("C", "B"));
    EXPECT_TRUE(list.after("C", "D"));

    EXPECT_TRUE(list.before("A", "Start"));
    EXPECT_TRUE(list.after("D", "End"));

    EXPECT_TRUE(list.find("Start"));
    EXPECT_TRUE(list.find("B"));
    EXPECT_TRUE(list.find("D"));
    EXPECT_TRUE(list.find("End"));

    EXPECT_FALSE(list.before("X", "E"));
    EXPECT_FALSE(list.after("X", "E"));
}

TEST(DListTest, EraseHeadAndTail) {
    DList list;

    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");

    EXPECT_TRUE(list.eraseHead());
    EXPECT_FALSE(list.find("A"));

    EXPECT_TRUE(list.eraseTail());
    EXPECT_FALSE(list.find("C"));

    EXPECT_TRUE(list.find("B"));

    EXPECT_TRUE(list.eraseHead());

    EXPECT_FALSE(list.eraseHead());
    EXPECT_FALSE(list.eraseTail());

    list.pushBack("D");

    EXPECT_TRUE(list.find("D"));
}

TEST(DListTest, EraseBeforeAndAfter) {
    DList list;

    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");
    list.pushBack("D");

    EXPECT_TRUE(list.eraseBefore("C"));
    EXPECT_FALSE(list.find("B"));

    EXPECT_TRUE(list.eraseAfter("C"));
    EXPECT_FALSE(list.find("D"));

    EXPECT_TRUE(list.find("A"));
    EXPECT_TRUE(list.find("C"));

    EXPECT_FALSE(list.eraseBefore("A"));
    EXPECT_FALSE(list.eraseAfter("C"));

    EXPECT_FALSE(list.eraseBefore("X"));
    EXPECT_FALSE(list.eraseAfter("X"));
}

TEST(DListTest, EraseByValue) {
    DList list;

    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");

    EXPECT_TRUE(list.eraseValue("B"));
    EXPECT_FALSE(list.find("B"));

    EXPECT_TRUE(list.eraseValue("A"));
    EXPECT_TRUE(list.eraseValue("C"));

    EXPECT_FALSE(list.eraseValue("A"));

    list.pushBack("D");

    EXPECT_TRUE(list.find("D"));
}

TEST(DListTest, ClearList) {
    DList list;

    list.pushBack("A");
    list.pushBack("B");

    list.clear();

    EXPECT_FALSE(list.find("A"));
    EXPECT_FALSE(list.find("B"));

    list.pushFront("C");

    EXPECT_TRUE(list.find("C"));
}

TEST(DListTest, TextSerialization) {
    DList list;
    stringstream stream;

    list.pushBack("A");
    list.pushBack("Hello world");
    list.pushBack("");

    list.saveText(stream);

    DList loaded;
    loaded.loadText(stream);

    stringstream result;
    loaded.saveText(result);

    EXPECT_EQ(result.str(), stream.str());

    EXPECT_TRUE(loaded.find("A"));
    EXPECT_TRUE(loaded.find("Hello world"));
    EXPECT_TRUE(loaded.find(""));
}

TEST(DListTest, BinarySerialization) {
    DList list;
    stringstream stream(ios::in | ios::out | ios::binary);

    list.pushBack("A");
    list.pushBack("Hello world");
    list.pushBack("");

    list.saveBinary(stream);

    DList loaded;
    loaded.loadBinary(stream);

    stringstream result(ios::in | ios::out | ios::binary);
    loaded.saveBinary(result);

    EXPECT_EQ(result.str(), stream.str());
}

TEST(DListTest, InvalidTextData) {
    DList list;

    list.pushBack("Original");

    stringstream stream("-1");

    EXPECT_THROW(list.loadText(stream), runtime_error);

    EXPECT_TRUE(list.find("Original"));
}

TEST(DListTest, InvalidBinaryData) {
    DList list;

    list.pushBack("Original");
    stringstream stream(ios::in | ios::out | ios::binary);

    int invalidSize = -1;

    stream.write(
        reinterpret_cast<const char*>(&invalidSize),
        sizeof(invalidSize)
    );

    EXPECT_THROW(list.loadBinary(stream), runtime_error);

    EXPECT_TRUE(list.find("Original"));
}

// Тестирование стека

TEST(StackTest, EmptyStack) {
    Stack stack;
    string value;

    EXPECT_FALSE(stack.pop(value));
}

TEST(StackTest, PushAndPop) {
    Stack stack;
    string value;

    stack.push("A");
    stack.push("B");
    stack.push("C");

    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, "C");

    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, "B");

    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, "A");

    EXPECT_FALSE(stack.pop(value));
}

TEST(StackTest, OneElement) {
    Stack stack;
    string value;

    stack.push("A");

    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, "A");

    EXPECT_FALSE(stack.pop(value));

    stack.push("B");

    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, "B");
}

TEST(StackTest, ClearStack) {
    Stack stack;
    string value;

    stack.push("A");
    stack.push("B");

    stack.clear();

    EXPECT_FALSE(stack.pop(value));

    stack.push("C");

    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, "C");
}

TEST(StackTest, TextSerialization) {
    Stack stack;
    stringstream stream;

    stack.push("A");
    stack.push("Hello world");
    stack.push("C");

    stack.saveText(stream);

    Stack loaded;
    loaded.loadText(stream);

    string value;

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "C");

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "Hello world");

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "A");

    EXPECT_FALSE(loaded.pop(value));
}

TEST(StackTest, BinarySerialization) {
    Stack stack;
    stringstream stream(ios::in | ios::out | ios::binary);

    stack.push("A");
    stack.push("Hello world");
    stack.push("C");

    stack.saveBinary(stream);

    Stack loaded;
    loaded.loadBinary(stream);

    string value;

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "C");

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "Hello world");

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "A");

    EXPECT_FALSE(loaded.pop(value));
}

TEST(StackTest, EmptySerialization) {
    Stack stack;

    stringstream textStream;

    stack.saveText(textStream);

    Stack loadedText;
    loadedText.loadText(textStream);

    string value;

    EXPECT_FALSE(loadedText.pop(value));

    stringstream binaryStream(
        ios::in | ios::out | ios::binary
    );

    stack.saveBinary(binaryStream);

    Stack loadedBinary;
    loadedBinary.loadBinary(binaryStream);

    EXPECT_FALSE(loadedBinary.pop(value));
}

TEST(StackTest, InvalidTextData) {
    Stack stack;

    stack.push("Original");

    stringstream stream("-1");

    EXPECT_THROW(stack.loadText(stream), runtime_error);

    string value;

    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, "Original");
}

TEST(StackTest, InvalidBinaryData) {
    Stack stack;

    stack.push("Original");

    stringstream stream(ios::in | ios::out | ios::binary);

    int invalidSize = -1;

    stream.write(
        reinterpret_cast<const char*>(&invalidSize),
        sizeof(invalidSize)
    );

    EXPECT_THROW(stack.loadBinary(stream), runtime_error);

    string value;

    EXPECT_TRUE(stack.pop(value));
    EXPECT_EQ(value, "Original");
}

// Тестирование очереди

TEST(QueueTest, EmptyQueue) {
    Queue queue;
    string value;

    EXPECT_FALSE(queue.pop(value));
}

TEST(QueueTest, PushAndPop) {
    Queue queue;
    string value;

    queue.push("A");
    queue.push("B");
    queue.push("C");

    EXPECT_TRUE(queue.pop(value));
    EXPECT_EQ(value, "A");

    EXPECT_TRUE(queue.pop(value));
    EXPECT_EQ(value, "B");

    EXPECT_TRUE(queue.pop(value));
    EXPECT_EQ(value, "C");

    EXPECT_FALSE(queue.pop(value));
}

TEST(QueueTest, OneElement) {
    Queue queue;
    string value;

    queue.push("A");

    EXPECT_TRUE(queue.pop(value));
    EXPECT_EQ(value, "A");

    EXPECT_FALSE(queue.pop(value));

    queue.push("B");

    EXPECT_TRUE(queue.pop(value));
    EXPECT_EQ(value, "B");
}

TEST(QueueTest, ClearQueue) {
    Queue queue;
    string value;

    queue.push("A");
    queue.push("B");

    queue.clear();

    EXPECT_FALSE(queue.pop(value));

    queue.push("C");

    EXPECT_TRUE(queue.pop(value));
    EXPECT_EQ(value, "C");
}

TEST(QueueTest, TextSerialization) {
    Queue queue;
    stringstream stream;

    queue.push("A");
    queue.push("Hello world");
    queue.push("C");

    queue.saveText(stream);

    Queue loaded;
    loaded.loadText(stream);

    string value;

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "A");

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "Hello world");

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "C");

    EXPECT_FALSE(loaded.pop(value));
}

TEST(QueueTest, BinarySerialization) {
    Queue queue;
    stringstream stream(ios::in | ios::out | ios::binary);

    queue.push("A");
    queue.push("Hello world");
    queue.push("C");

    queue.saveBinary(stream);

    Queue loaded;
    loaded.loadBinary(stream);

    string value;

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "A");

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "Hello world");

    EXPECT_TRUE(loaded.pop(value));
    EXPECT_EQ(value, "C");

    EXPECT_FALSE(loaded.pop(value));
}

TEST(QueueTest, EmptySerialization) {
    Queue queue;
    string value;

    stringstream textStream;

    queue.saveText(textStream);

    Queue loadedText;
    loadedText.loadText(textStream);

    EXPECT_FALSE(loadedText.pop(value));

    stringstream binaryStream(
        ios::in | ios::out | ios::binary
    );

    queue.saveBinary(binaryStream);

    Queue loadedBinary;
    loadedBinary.loadBinary(binaryStream);

    EXPECT_FALSE(loadedBinary.pop(value));
}

TEST(QueueTest, InvalidTextData) {
    Queue queue;

    queue.push("Original");

    stringstream stream("-1");

    EXPECT_THROW(queue.loadText(stream), runtime_error);

    string value;

    EXPECT_TRUE(queue.pop(value));
    EXPECT_EQ(value, "Original");
}

TEST(QueueTest, InvalidBinaryData) {
    Queue queue;

    queue.push("Original");

    stringstream stream(ios::in | ios::out | ios::binary);

    int invalidSize = -1;

    stream.write(
        reinterpret_cast<const char*>(&invalidSize),
        sizeof(invalidSize)
    );

    EXPECT_THROW(queue.loadBinary(stream), runtime_error);

    string value;

    EXPECT_TRUE(queue.pop(value));
    EXPECT_EQ(value, "Original");
}

// Тестирование метода цепочек

TEST(ChainHashTest, EmptyTable) {
    ChainHash table;

    EXPECT_FALSE(table.contains(10));
    EXPECT_FALSE(table.remove(10));
}

TEST(ChainHashTest, AddAndContains) {
    ChainHash table;

    EXPECT_TRUE(table.add(10));
    EXPECT_TRUE(table.add(20));
    EXPECT_TRUE(table.add(-5));

    EXPECT_TRUE(table.contains(10));
    EXPECT_TRUE(table.contains(20));
    EXPECT_TRUE(table.contains(-5));

    EXPECT_FALSE(table.contains(100));
}

TEST(ChainHashTest, DuplicateKeys) {
    ChainHash table;

    EXPECT_TRUE(table.add(10));
    EXPECT_FALSE(table.add(10));

    EXPECT_TRUE(table.contains(10));
}

TEST(ChainHashTest, CollisionsAndRemoval) {
    ChainHash table(101);

    EXPECT_TRUE(table.add(10));
    EXPECT_TRUE(table.add(111));
    EXPECT_TRUE(table.add(212));

    EXPECT_TRUE(table.contains(10));
    EXPECT_TRUE(table.contains(111));
    EXPECT_TRUE(table.contains(212));

    EXPECT_TRUE(table.remove(111));

    EXPECT_FALSE(table.contains(111));
    EXPECT_TRUE(table.contains(10));
    EXPECT_TRUE(table.contains(212));

    EXPECT_FALSE(table.remove(111));
}

TEST(ChainHashTest, GrowTable) {
    ChainHash table(3);

    EXPECT_TRUE(table.add(1));
    EXPECT_TRUE(table.add(2));
    EXPECT_TRUE(table.add(3));
    EXPECT_TRUE(table.add(4));

    EXPECT_TRUE(table.contains(1));
    EXPECT_TRUE(table.contains(2));
    EXPECT_TRUE(table.contains(3));
    EXPECT_TRUE(table.contains(4));
}

TEST(ChainHashTest, ClearTable) {
    ChainHash table;

    table.add(10);
    table.add(20);

    table.clear();

    EXPECT_FALSE(table.contains(10));
    EXPECT_FALSE(table.contains(20));

    EXPECT_TRUE(table.add(30));
    EXPECT_TRUE(table.contains(30));
}

TEST(ChainHashTest, InvalidCapacity) {
    EXPECT_THROW(ChainHash table(0), invalid_argument);
    EXPECT_THROW(ChainHash table(-1), invalid_argument);
}

TEST(ChainHashTest, TextSerialization) {
    ChainHash table;
    stringstream stream;

    table.add(10);
    table.add(111);
    table.add(212);
    table.add(-5);

    table.saveText(stream);

    ChainHash loaded;
    loaded.loadText(stream);

    EXPECT_TRUE(loaded.contains(10));
    EXPECT_TRUE(loaded.contains(111));
    EXPECT_TRUE(loaded.contains(212));
    EXPECT_TRUE(loaded.contains(-5));

    EXPECT_FALSE(loaded.contains(999));
}

TEST(ChainHashTest, BinarySerialization) {
    ChainHash table;
    stringstream stream(ios::in | ios::out | ios::binary);

    table.add(10);
    table.add(111);
    table.add(-5);

    table.saveBinary(stream);

    ChainHash loaded;
    loaded.loadBinary(stream);

    EXPECT_TRUE(loaded.contains(10));
    EXPECT_TRUE(loaded.contains(111));
    EXPECT_TRUE(loaded.contains(-5));

    EXPECT_FALSE(loaded.contains(999));
}

TEST(ChainHashTest, InvalidTextData) {
    ChainHash table;

    table.add(10);

    stringstream stream("0 1\n10\n");

    EXPECT_THROW(table.loadText(stream), runtime_error);

    EXPECT_TRUE(table.contains(10));
}

TEST(ChainHashTest, InvalidBinaryData) {
    ChainHash table;

    table.add(10);

    stringstream stream(ios::in | ios::out | ios::binary);

    int invalidCapacity = 0;
    int count = 1;

    stream.write(
        reinterpret_cast<const char*>(&invalidCapacity),
        sizeof(invalidCapacity)
    );

    stream.write(
        reinterpret_cast<const char*>(&count),
        sizeof(count)
    );

    EXPECT_THROW(table.loadBinary(stream), runtime_error);

    EXPECT_TRUE(table.contains(10));
}

// Тестирование хеширования кукушки

TEST(CuckooHashTest, EmptyTable) {
    CuckooHash table;

    EXPECT_FALSE(table.contains(10));
    EXPECT_FALSE(table.remove(10));
}

TEST(CuckooHashTest, AddAndContains) {
    CuckooHash table;

    EXPECT_TRUE(table.add(10));
    EXPECT_TRUE(table.add(20));
    EXPECT_TRUE(table.add(-5));

    EXPECT_TRUE(table.contains(10));
    EXPECT_TRUE(table.contains(20));
    EXPECT_TRUE(table.contains(-5));

    EXPECT_FALSE(table.contains(100));
}

TEST(CuckooHashTest, DuplicateKeys) {
    CuckooHash table;

    EXPECT_TRUE(table.add(10));
    EXPECT_FALSE(table.add(10));

    EXPECT_TRUE(table.contains(10));
}

TEST(CuckooHashTest, Collisions) {
    CuckooHash table;

    EXPECT_TRUE(table.add(10));
    EXPECT_TRUE(table.add(111));
    EXPECT_TRUE(table.add(212));

    EXPECT_TRUE(table.contains(10));
    EXPECT_TRUE(table.contains(111));
    EXPECT_TRUE(table.contains(212));
}

TEST(CuckooHashTest, RemoveKeys) {
    CuckooHash table;

    table.add(10);
    table.add(111);
    table.add(212);

    EXPECT_TRUE(table.remove(111));
    EXPECT_FALSE(table.contains(111));

    EXPECT_TRUE(table.contains(10));
    EXPECT_TRUE(table.contains(212));

    EXPECT_FALSE(table.remove(111));
}

TEST(CuckooHashTest, ClearTable) {
    CuckooHash table;

    table.add(10);
    table.add(20);

    table.clear();

    EXPECT_FALSE(table.contains(10));
    EXPECT_FALSE(table.contains(20));

    EXPECT_TRUE(table.add(30));
    EXPECT_TRUE(table.contains(30));
}

TEST(CuckooHashTest, InvalidCapacity) {
    EXPECT_THROW(CuckooHash table(0), invalid_argument);
    EXPECT_THROW(CuckooHash table(-1), invalid_argument);
}

TEST(CuckooHashTest, TextSerialization) {
    CuckooHash table;
    stringstream stream;

    table.add(10);
    table.add(111);
    table.add(212);
    table.add(-5);

    table.saveText(stream);

    CuckooHash loaded;
    loaded.loadText(stream);

    EXPECT_TRUE(loaded.contains(10));
    EXPECT_TRUE(loaded.contains(111));
    EXPECT_TRUE(loaded.contains(212));
    EXPECT_TRUE(loaded.contains(-5));

    EXPECT_FALSE(loaded.contains(999));
}

TEST(CuckooHashTest, BinarySerialization) {
    CuckooHash table;
    stringstream stream(ios::in | ios::out | ios::binary);

    table.add(10);
    table.add(111);
    table.add(212);
    table.add(-5);

    table.saveBinary(stream);

    CuckooHash loaded;
    loaded.loadBinary(stream);

    EXPECT_TRUE(loaded.contains(10));
    EXPECT_TRUE(loaded.contains(111));
    EXPECT_TRUE(loaded.contains(212));
    EXPECT_TRUE(loaded.contains(-5));
}

TEST(CuckooHashTest, InvalidTextData) {
    CuckooHash table;

    table.add(10);

    stringstream stream("0 1 0\n");

    EXPECT_THROW(table.loadText(stream), runtime_error);

    EXPECT_TRUE(table.contains(10));
}

TEST(CuckooHashTest, InvalidBinaryData) {
    CuckooHash table;

    table.add(10);

    stringstream stream(ios::in | ios::out | ios::binary);

    int invalidCapacity = 0;
    int count = 1;
    unsigned int seed = 0;

    stream.write(
        reinterpret_cast<const char*>(&invalidCapacity),
        sizeof(invalidCapacity)
    );

    stream.write(
        reinterpret_cast<const char*>(&count),
        sizeof(count)
    );

    stream.write(
        reinterpret_cast<const char*>(&seed),
        sizeof(seed)
    );

    EXPECT_THROW(table.loadBinary(stream), runtime_error);

    EXPECT_TRUE(table.contains(10));
}

// Тестирование полного бинарного дерева

TEST(CompleteTreeTest, EmptyTree) {
    CompleteTree tree;

    EXPECT_EQ(tree.getSize(), 0);
    EXPECT_FALSE(tree.find("A"));
    EXPECT_TRUE(tree.isComplete());
}

TEST(CompleteTreeTest, InsertAndFind) {
    CompleteTree tree;

    tree.insert("A");
    tree.insert("B");
    tree.insert("C");
    tree.insert("D");

    EXPECT_EQ(tree.getSize(), 4);

    EXPECT_TRUE(tree.find("A"));
    EXPECT_TRUE(tree.find("B"));
    EXPECT_TRUE(tree.find("C"));
    EXPECT_TRUE(tree.find("D"));

    EXPECT_FALSE(tree.find("X"));

    EXPECT_TRUE(tree.isComplete());
}

TEST(CompleteTreeTest, Traversals) {
    CompleteTree tree;

    tree.insert("A");
    tree.insert("B");
    tree.insert("C");
    tree.insert("D");
    tree.insert("E");
    tree.insert("F");

    testing::internal::CaptureStdout();
    tree.printPre();
    string preorder = testing::internal::GetCapturedStdout();

    testing::internal::CaptureStdout();
    tree.printIn();
    string inorder = testing::internal::GetCapturedStdout();

    testing::internal::CaptureStdout();
    tree.printPost();
    string postorder = testing::internal::GetCapturedStdout();

    testing::internal::CaptureStdout();
    tree.printLevel();
    string levelorder = testing::internal::GetCapturedStdout();

    EXPECT_EQ(preorder, "A B D E C F \n");
    EXPECT_EQ(inorder, "D B E A F C \n");
    EXPECT_EQ(postorder, "D E B F C A \n");
    EXPECT_EQ(levelorder, "A B C D E F \n");
}

TEST(CompleteTreeTest, ClearAndReuse) {
    CompleteTree tree;

    tree.insert("A");
    tree.insert("B");
    tree.insert("C");

    tree.clear();

    EXPECT_EQ(tree.getSize(), 0);
    EXPECT_FALSE(tree.find("A"));
    EXPECT_TRUE(tree.isComplete());

    tree.insert("D");

    EXPECT_EQ(tree.getSize(), 1);
    EXPECT_TRUE(tree.find("D"));
}

TEST(CompleteTreeTest, DuplicateValues) {
    CompleteTree tree;

    tree.insert("A");
    tree.insert("A");
    tree.insert("B");

    EXPECT_EQ(tree.getSize(), 3);

    EXPECT_TRUE(tree.find("A"));
    EXPECT_TRUE(tree.find("B"));
    EXPECT_TRUE(tree.isComplete());
}

TEST(CompleteTreeTest, TextSerialization) {
    CompleteTree tree;
    stringstream stream;

    tree.insert("A");
    tree.insert("Hello world");
    tree.insert("C");
    tree.insert("");

    tree.saveText(stream);

    CompleteTree loaded;
    loaded.loadText(stream);

    EXPECT_EQ(loaded.getSize(), 4);
    EXPECT_TRUE(loaded.isComplete());
    EXPECT_TRUE(loaded.find("Hello world"));
    EXPECT_TRUE(loaded.find(""));

    stringstream result;
    loaded.saveText(result);

    EXPECT_EQ(result.str(), stream.str());
}

TEST(CompleteTreeTest, BinarySerialization) {
    CompleteTree tree;

    tree.insert("A");
    tree.insert("Hello world");
    tree.insert("C");
    tree.insert("");

    stringstream stream(ios::in | ios::out | ios::binary);

    tree.saveBinary(stream);

    CompleteTree loaded;
    loaded.loadBinary(stream);

    EXPECT_EQ(loaded.getSize(), 4);
    EXPECT_TRUE(loaded.isComplete());
    EXPECT_TRUE(loaded.find("Hello world"));
    EXPECT_TRUE(loaded.find(""));

    stringstream result(ios::in | ios::out | ios::binary);

    loaded.saveBinary(result);

    EXPECT_EQ(result.str(), stream.str());
}

TEST(CompleteTreeTest, EmptySerialization) {
    CompleteTree tree;

    stringstream textStream;

    tree.saveText(textStream);

    CompleteTree loadedText;
    loadedText.loadText(textStream);

    EXPECT_EQ(loadedText.getSize(), 0);
    EXPECT_TRUE(loadedText.isComplete());

    stringstream binaryStream(
        ios::in | ios::out | ios::binary
    );

    tree.saveBinary(binaryStream);

    CompleteTree loadedBinary;
    loadedBinary.loadBinary(binaryStream);

    EXPECT_EQ(loadedBinary.getSize(), 0);
    EXPECT_TRUE(loadedBinary.isComplete());
}

TEST(CompleteTreeTest, InvalidTextSize) {
    CompleteTree tree;

    tree.insert("Original");

    stringstream stream("-1");

    EXPECT_THROW(tree.loadText(stream), runtime_error);
    EXPECT_EQ(tree.getSize(), 1);
    EXPECT_TRUE(tree.find("Original"));
}

TEST(CompleteTreeTest, InvalidBinarySize) {
    CompleteTree tree;

    tree.insert("Original");

    stringstream stream(ios::in | ios::out | ios::binary);

    int invalidSize = -1;

    stream.write(
        reinterpret_cast<const char*>(&invalidSize),
        sizeof(invalidSize)
    );

    EXPECT_THROW(tree.loadBinary(stream), runtime_error);

    EXPECT_EQ(tree.getSize(), 1);
    EXPECT_TRUE(tree.find("Original"));
}

TEST(CompleteTreeTest, TruncatedTextData) {
    CompleteTree tree;

    tree.insert("Original");

    stringstream stream;

    stream << "3\n";
    stream << "\"A\"\n";

    EXPECT_THROW(tree.loadText(stream), runtime_error);

    EXPECT_EQ(tree.getSize(), 1);
    EXPECT_TRUE(tree.find("Original"));
    EXPECT_FALSE(tree.find("A"));
}

TEST(CompleteTreeTest, TruncatedBinaryData) {
    CompleteTree tree;

    tree.insert("Original");

    stringstream stream(ios::in | ios::out | ios::binary);

    int count = 3;

    stream.write(
        reinterpret_cast<const char*>(&count),
        sizeof(count)
    );

    EXPECT_THROW(tree.loadBinary(stream), runtime_error);

    EXPECT_EQ(tree.getSize(), 1);
    EXPECT_TRUE(tree.find("Original"));
}

TEST(ChainHashTest, PrintEmptyTable) {
    ChainHash table(5);
    stringstream out;

    table.print(out);
    EXPECT_TRUE(out.str().empty());

    table.printTable(out);

    EXPECT_NE(out.str().find("(table is empty)"), string::npos);
    EXPECT_NE(out.str().find("Keys: 0"), string::npos);
}

TEST(ChainHashTest, PrintFilledTable) {
    ChainHash table(101);
    stringstream out;

    table.add(10);
    table.add(111);
    table.add(212);

    table.print(out);

    EXPECT_NE(out.str().find("10"), string::npos);
    EXPECT_NE(out.str().find("111"), string::npos);
    EXPECT_NE(out.str().find("212"), string::npos);

    out.str("");
    out.clear();

    table.printTable(out);

    EXPECT_NE(out.str().find("212 -> 111 -> 10"), string::npos);
    EXPECT_NE(out.str().find("Keys: 3"), string::npos);
}

TEST(CuckooHashTest, PrintEmptyTable) {
    CuckooHash table(5);
    stringstream out;

    table.print(out);
    EXPECT_TRUE(out.str().empty());

    table.printTable(out);

    EXPECT_NE(out.str().find("(table is empty)"), string::npos);
    EXPECT_NE(out.str().find("Keys: 0"), string::npos);
}

TEST(CuckooHashTest, PrintFilledTable) {
    CuckooHash table(101);
    stringstream out;

    table.add(10);
    table.add(111);
    table.add(212);

    table.print(out);

    EXPECT_NE(out.str().find("10"), string::npos);
    EXPECT_NE(out.str().find("111"), string::npos);
    EXPECT_NE(out.str().find("212"), string::npos);

    out.str("");
    out.clear();

    table.printTable(out);

    EXPECT_NE(out.str().find("Table 1"), string::npos);
    EXPECT_NE(out.str().find("Table 2"), string::npos);
    EXPECT_NE(out.str().find("Keys: 3"), string::npos);
}

TEST(CuckooHashTest, RebuildTable) {
    CuckooHash table(1);

    EXPECT_TRUE(table.add(10));
    EXPECT_TRUE(table.add(11));
    EXPECT_TRUE(table.add(12));

    EXPECT_TRUE(table.contains(10));
    EXPECT_TRUE(table.contains(11));
    EXPECT_TRUE(table.contains(12));

    stringstream out;
    table.printTable(out);

    EXPECT_NE(
        out.str().find("Keys: 3"),
        string::npos
    );

    EXPECT_NE(
        out.str().find("capacity of each table: 3"),
        string::npos
    );
}

TEST(ChainHashTest, DuplicateKeysInTextFile) {
    ChainHash table;

    table.add(50);

    stringstream stream(
        "101 2\n"
        "10\n"
        "10\n"
    );

    EXPECT_THROW(table.loadText(stream), runtime_error);

    EXPECT_TRUE(table.contains(50));
    EXPECT_FALSE(table.contains(10));
}

TEST(ChainHashTest, TruncatedTextFile) {
    ChainHash table;

    table.add(50);

    stringstream stream(
        "101 3\n"
        "10\n"
    );

    EXPECT_THROW(table.loadText(stream), runtime_error);

    EXPECT_TRUE(table.contains(50));
    EXPECT_FALSE(table.contains(10));
}

TEST(CuckooHashTest, InvalidHashPosition) {
    CuckooHash table;

    table.add(50);

    stringstream stream(
        "101 1 0\n"
        "1 0 10\n"
    );

    EXPECT_THROW(table.loadText(stream), runtime_error);

    EXPECT_TRUE(table.contains(50));
    EXPECT_FALSE(table.contains(10));
}

TEST(CuckooHashTest, InvalidTableNumber) {
    CuckooHash table;

    table.add(50);

    stringstream stream(
        "101 1 0\n"
        "3 10 10\n"
    );

    EXPECT_THROW(table.loadText(stream), runtime_error);

    EXPECT_TRUE(table.contains(50));
    EXPECT_FALSE(table.contains(10));
}
