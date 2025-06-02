#define TEST_DESKTOP

#include <serial_parser.h>
#include <iostream>
#include <unity.h>
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



enum class SerialError {
    NONE = 0,
    INVALID_COMMAND,
    ARG1_NOT_NUMERIC,
    ARG2_NOT_NUMERIC,
    ARG1_TOO_LONG,
    ARG2_TOO_LONG,
    OVERFLOW,
    EXECUTION_FAILED,
    UNKNOWN
};

struct SerialCommand {
    char cmd = '\0';
    long arg1 = 0;
    long arg2 = 0;
    bool valid = false;
    SerialError error = SerialError::NONE;
};


void setUp(void) {}
void tearDown(void) {}

void ANALOG_READ_RIGHT_TEST(void){
    std::cout<<"1\n";
    pSerial fake("a\n");
    std::cout<<"2\n";
    SerialCommand cmd;
    std::cout<<"3\n";
    serialParser parser;
    std::cout<<"4\n";
    parser.poll(cmd);
    std::cout<<"5\n";

    char expectedCmd = 'a';
    long expectedArg1 = 0;
    long expectedArg2 = 0;
    bool expectedValid = false;

    int actual = fake.available();
    std::cout<<actual;
    TEST_ASSERT_EQUAL_CHAR(expectedCmd,cmd.cmd);
}

int main(int argc, char **argv){
    UNITY_BEGIN();

    RUN_TEST(ANALOG_READ_RIGHT_TEST);

    UNITY_END();


    return 0;
}






