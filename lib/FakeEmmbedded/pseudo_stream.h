#pragma once
#include <cstring>
class pSerial {
public:
    pSerial(const char* input) : buffer(input), pos(0), length(std::strlen(input)) {}

    int available() {
        // Return number of remaining characters
        return (pos < length) ? (length - pos) : 0;
    }

    int read() {
        if (pos >= length) return -1;  // Check bounds
        return buffer[pos++];
    }

    int peek() {
        return (pos < length) ? buffer[pos] : -1;
    }

    void flush() {}

    // Dummy write to match Arduino Stream interface
    size_t write(char) { return 1; }

    // Reset position for repeated tests
    void reset() { pos = 0; }

private:
    const char* buffer;
    size_t pos;
    size_t length;  // <-- Store length
};




/*
// Example test usage
        FakeSerial fake("A 123,456\n");
        serialParser parser;
        SerialCommand cmd;
    if (parser.poll(cmd, fake)) {
        // Check cmd fields here
}
*/