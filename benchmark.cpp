#include <benchmark/benchmark.h>
#include <string>

#include "Array.h"
#include "FList.h"
#include "DList.h"
#include "Stack.h"
#include "Queue.h"
#include "HashLab.h"
#include "CompleteTree.h"

using namespace std;

// Динамический массив

static void BM_ArrayPush(benchmark::State& state) {
    int n = static_cast<int>(state.range(0));

    for (auto _ : state) {
        Array arr;

        for (int i = 0; i < n; i++) {
            arr.push("A");
        }

        benchmark::DoNotOptimize(arr.getSize());
    }
}

BENCHMARK(BM_ArrayPush)->Arg(100)->Arg(500);


// Односвязный список

static void BM_FListPushFront(benchmark::State& state) {
    int n = static_cast<int>(state.range(0));

    for (auto _ : state) {
        FList list;

        for (int i = 0; i < n; i++) {
            list.pushFront("A");
        }

        benchmark::DoNotOptimize(&list);
    }
}

BENCHMARK(BM_FListPushFront)->Arg(100)->Arg(500);


// Двусвязный список

static void BM_DListPushBack(benchmark::State& state) {
    int n = static_cast<int>(state.range(0));

    for (auto _ : state) {
        DList list;

        for (int i = 0; i < n; i++) {
            list.pushBack("A");
        }

        benchmark::DoNotOptimize(&list);
    }
}

BENCHMARK(BM_DListPushBack)->Arg(100)->Arg(500);


// Стек

static void BM_StackPush(benchmark::State& state) {
    int n = static_cast<int>(state.range(0));

    for (auto _ : state) {
        Stack stack;

        for (int i = 0; i < n; i++) {
            stack.push("A");
        }

        benchmark::DoNotOptimize(&stack);
    }
}

BENCHMARK(BM_StackPush)->Arg(100)->Arg(500);


// Очередь

static void BM_QueuePush(benchmark::State& state) {
    int n = static_cast<int>(state.range(0));

    for (auto _ : state) {
        Queue queue;

        for (int i = 0; i < n; i++) {
            queue.push("A");
        }

        benchmark::DoNotOptimize(&queue);
    }
}

BENCHMARK(BM_QueuePush)->Arg(100)->Arg(500);


// Хеш-таблица методом цепочек

static void BM_ChainHashAdd(benchmark::State& state) {
    int n = static_cast<int>(state.range(0));

    for (auto _ : state) {
        ChainHash table;
        for (int i = 0; i < n; i++) {
            table.add(to_string(i), "значение" + to_string(i));
        }
        benchmark::DoNotOptimize(&table);
    }
}
BENCHMARK(BM_ChainHashAdd)->Arg(100)->Arg(500);

static void BM_ChainHashContains(benchmark::State& state) {
    ChainHash table;
    for (int i = 0; i < 100; i++) {
        table.add(to_string(i), "значение" + to_string(i));
    }

    for (auto _ : state) {
        string value;
        bool found = table.find("50", value);
        benchmark::DoNotOptimize(found);
        benchmark::DoNotOptimize(value);
    }
}
BENCHMARK(BM_ChainHashContains);

// Хеширование кукушки

static void BM_CuckooHashAdd(benchmark::State& state) {
    for (auto _ : state) {
        CuckooHash table;
        table.add("10", "первое");
        table.add("111", "второе");
        table.add("212", "третье");
        benchmark::DoNotOptimize(&table);
    }
}
BENCHMARK(BM_CuckooHashAdd);

static void BM_CuckooHashContains(benchmark::State& state) {
    CuckooHash table;
    table.add("10", "первое");
    table.add("111", "второе");
    table.add("212", "третье");

    for (auto _ : state) {
        string value;
        bool found = table.find("111", value);
        benchmark::DoNotOptimize(found);
        benchmark::DoNotOptimize(value);
    }
}
BENCHMARK(BM_CuckooHashContains);


// Полное бинарное дерево

static void BM_CompleteTreeInsert(benchmark::State& state) {
    int n = static_cast<int>(state.range(0));

    for (auto _ : state) {
        CompleteTree tree;

        for (int i = 0; i < n; i++) {
            tree.insert("A");
        }

        benchmark::DoNotOptimize(tree.getSize());
    }
}

BENCHMARK(BM_CompleteTreeInsert)->Arg(100)->Arg(500);


static void BM_CompleteTreeFind(benchmark::State& state) {
    CompleteTree tree;

    for (int i = 0; i < 100; i++) {
        tree.insert(to_string(i));
    }

    for (auto _ : state) {
        bool found = tree.find("99");

        benchmark::DoNotOptimize(found);
    }
}

BENCHMARK(BM_CompleteTreeFind);
