#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <string>
#include <iosfwd>

void writeInt(std::ostream& out, int value);
int readInt(std::istream& in);

void writeString(std::ostream& out, const std::string& value);
std::string readString(std::istream& in);

#endif
