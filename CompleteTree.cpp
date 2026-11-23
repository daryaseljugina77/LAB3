#include "CompleteTree.h"
#include "Serialization.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <utility>
#include <limits>

using namespace std;

CompleteTree::TNode::TNode(const string& v)
    : value(v), left(nullptr), right(nullptr) {
}

CompleteTree::CompleteTree() : root(nullptr), count(0) {
}

CompleteTree::~CompleteTree() {
    clear();
}

void CompleteTree::destroy(TNode* p) {
    if (!p) {
        return;
    }

    destroy(p->left);
    destroy(p->right);

    delete p;
}

void CompleteTree::clear() {
    destroy(root);

    root = nullptr;
    count = 0;
}

void CompleteTree::insert(const string& v) {
    if (count == numeric_limits<int>::max()) {
        throw length_error("tree too large");
    }

    TNode* n = new TNode(v);

    if (!root) {
        root = n;
        count = 1;

        return;
    }

    TNode** nodes = nullptr;

    try {
        nodes = new TNode*[count];
    }
    catch (...) {
        delete n;

        throw;
    }

    int a = 0;
    int b = 0;

    nodes[b] = root;
    b++;

    while (a < b) {
        TNode* p = nodes[a];
        a++;

        if (!p->left) {
            p->left = n;
            count++;

            delete[] nodes;

            return;
        }

        nodes[b] = p->left;
        b++;

        if (!p->right) {
            p->right = n;
            count++;

            delete[] nodes;

            return;
        }

        nodes[b] = p->right;
        b++;
    }

    delete[] nodes;
    delete n;
}

bool CompleteTree::find(const string& v) const {
    if (!root) {
        return false;
    }

    const TNode** nodes = new const TNode*[count];

    int a = 0;
    int b = 0;

    nodes[b] = root;
    b++;

    while (a < b) {
        const TNode* p = nodes[a];
        a++;

        if (p->value == v) {
            delete[] nodes;

            return true;
        }

        if (p->left) {
            nodes[b] = p->left;
            b++;
        }

        if (p->right) {
            nodes[b] = p->right;
            b++;
        }
    }

    delete[] nodes;

    return false;
}

bool CompleteTree::isComplete() const {
    if (!root) {
        return true;
    }

    const TNode** nodes = new const TNode*[count];

    int a = 0;
    int b = 0;

    bool gap = false;

    nodes[b] = root;
    b++;

    while (a < b) {
        const TNode* p = nodes[a];
        a++;

        if (p->left) {
            if (gap) {
                delete[] nodes;

                return false;
            }

            nodes[b] = p->left;
            b++;
        }
        else {
            gap = true;
        }

        if (p->right) {
            if (gap) {
                delete[] nodes;

                return false;
            }

            nodes[b] = p->right;
            b++;
        }
        else {
            gap = true;
        }
    }

    delete[] nodes;

    return true;
}

int CompleteTree::getSize() const {
    return count;
}

void CompleteTree::printPre(const TNode* p) const {
    if (!p) {
        return;
    }

    cout << p->value << ' ';

    printPre(p->left);
    printPre(p->right);
}

void CompleteTree::printIn(const TNode* p) const {
    if (!p) {
        return;
    }

    printIn(p->left);

    cout << p->value << ' ';

    printIn(p->right);
}

void CompleteTree::printPost(const TNode* p) const {
    if (!p) {
        return;
    }

    printPost(p->left);
    printPost(p->right);

    cout << p->value << ' ';
}

void CompleteTree::printPre() const {
    printPre(root);

    cout << endl;
}

void CompleteTree::printIn() const {
    printIn(root);

    cout << endl;
}

void CompleteTree::printPost() const {
    printPost(root);

    cout << endl;
}

void CompleteTree::printLevel() const {
    if (!root) {
        cout << endl;

        return;
    }

    const TNode** nodes = new const TNode*[count];

    int a = 0;
    int b = 0;

    nodes[b] = root;
    b++;

    while (a < b) {
        const TNode* p = nodes[a];
        a++;

        cout << p->value << ' ';

        if (p->left) {
            nodes[b] = p->left;
            b++;
        }

        if (p->right) {
            nodes[b] = p->right;
            b++;
        }
    }
    delete[] nodes;

    cout << endl;
}
void CompleteTree::saveText(ostream& out) const {
    out << count << '\n';

    if (!out) {
        throw runtime_error("write error");
    }

    if (!root) {
        return;
    }

    const TNode** nodes = new const TNode*[count];

    int a = 0;
    int b = 0;

    nodes[b] = root;
    b++;

    try {
        while (a < b) {
            const TNode* p = nodes[a];
            a++;

            out << quoted(p->value) << '\n';

            if (!out) {
                throw runtime_error("write error");
            }

            if (p->left) {
                nodes[b] = p->left;
                b++;
            }

            if (p->right) {
                nodes[b] = p->right;
                b++;
            }
        }
    }
    catch (...) {
        delete[] nodes;

        throw;
    }

    delete[] nodes;
}

void CompleteTree::loadText(istream& in) {
    int n = 0;

    if (!(in >> n) || n < 0) {
        throw runtime_error("invalid tree size");
    }

    CompleteTree temp;

    for (int i = 0; i < n; i++) {
        string value;

        if (!(in >> quoted(value))) {
            throw runtime_error("read error");
        }

        temp.insert(value);
    }

    swap(root, temp.root);
    swap(count, temp.count);
}

void CompleteTree::saveBinary(ostream& out) const {
    writeInt(out, count);

    if (!root) {
        return;
    }

    const TNode** nodes = new const TNode*[count];

    int a = 0;
    int b = 0;

    nodes[b] = root;
    b++;

    try {
        while (a < b) {
            const TNode* p = nodes[a];
            a++;

            writeString(out, p->value);

            if (p->left) {
                nodes[b] = p->left;
                b++;
            }

            if (p->right) {
                nodes[b] = p->right;
                b++;
            }
        }
    }
    catch (...) {
        delete[] nodes;

        throw;
    }

    delete[] nodes;
}

void CompleteTree::loadBinary(istream& in) {
    int n = readInt(in);

    if (n < 0) {
        throw runtime_error("invalid tree size");
    }

    CompleteTree temp;

    for (int i = 0; i < n; i++) {
        string value = readString(in);

        temp.insert(value);
    }

    swap(root, temp.root);
    swap(count, temp.count);
}
