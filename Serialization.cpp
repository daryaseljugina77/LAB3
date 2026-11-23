#include "Serialization.h"
#include <istream>
#include <ostream>
#include <stdexcept>
#include <limits>

using namespace std;

void writeInt(ostream& out, int value) {
    out.write(
        reinterpret_cast<const char*>(&value),
        sizeof(value)
    );

    if (!out) {
        throw runtime_error("write error");
    }
}

int readInt(istream& in) {
    int value = 0;

    in.read(
        reinterpret_cast<char*>(&value),
        sizeof(value)
    );

    if (!in) {
        throw runtime_error("read error");
    }

    return value;
}

void writeString(ostream& out, const string& value) {
    if (value.size() > static_cast<string::size_type>(
        numeric_limits<int>::max())) {
        throw length_error("string too long");
    }

    int length = static_cast<int>(value.size());

    writeInt(out, length);

    out.write(value.data(), length);

    if (!out) {
        throw runtime_error("write error");
    }
}

string readString(istream& in) {
    int length = readInt(in);

    if (length < 0) {
        throw runtime_error("invalid string length");
    }

    string value(length, '\0');

    if (length > 0) {
        in.read(&value[0], length);
    }

    if (!in) {
        throw runtime_error("read error");
    }

    return value;
}
