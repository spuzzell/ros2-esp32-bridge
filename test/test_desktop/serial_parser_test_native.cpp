#ifdef TEST_DESKTOP
#include <serial_parser.h>
#include <serial_command.h>
#include <unity.h>

#include <ArduinoFake.h>
using namespace fakeit;


serialParser parser;


void setUp(void) {}
void tearDown(void) {}

void VALID_COMMAND_NO_ARG_TEST(void) {
    // Simulate "e\n" (ECHO command, no arguments)
    static const char input[] = "a\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    SerialCommand cmd{};
    cmd.valid = true;
    parser.poll(cmd);

    TEST_ASSERT_EQUAL_CHAR('a', cmd.cmd);
    TEST_ASSERT_EQUAL(0, cmd.arg1);
    TEST_ASSERT_EQUAL(0, cmd.arg2);
    TEST_ASSERT_TRUE(cmd.valid);
    TEST_ASSERT_EQUAL(SerialError::NONE, cmd.error);
}

void VALID_COMMAND_ONE_ARG_TEST(void) {
    // Simulate "a 42\n" (ANALOG_READ with one argument)
    static const char input[] = "a 42\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    SerialCommand cmd{};
    cmd.valid = true;
    parser.poll(cmd);

    TEST_ASSERT_EQUAL_CHAR('a', cmd.cmd);
    TEST_ASSERT_EQUAL(42, cmd.arg1);
    TEST_ASSERT_EQUAL(0, cmd.arg2);
    TEST_ASSERT_TRUE(cmd.valid);
    TEST_ASSERT_EQUAL(SerialError::NONE, cmd.error);
}

void VALID_COMMAND_TWO_ARG_TEST(void) {
    // Simulate "m 10 20\n" (MOTOR_SPEEDS with two arguments)
    static const char input[] = "m 10 20\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    SerialCommand cmd{};
    cmd.valid = true;
    parser.poll(cmd);

    TEST_ASSERT_EQUAL_CHAR('m', cmd.cmd);
    TEST_ASSERT_EQUAL(10, cmd.arg1);
    TEST_ASSERT_EQUAL(20, cmd.arg2);
    TEST_ASSERT_TRUE(cmd.valid);
    TEST_ASSERT_EQUAL(SerialError::NONE, cmd.error);
}

void VALID_COMMAND_MULTI_CMD_TEST(void) {
    // First command: "m 10 20\n"
    static const char input1[] = "m 10 20\n";
    static int idx1 = 0;
    idx1 = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input1[idx1] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input1[idx1] != '\0') ? (int)input1[idx1++] : -1;
    });

    SerialCommand cmd{};
    cmd.valid = true;
    parser.poll(cmd);

    TEST_ASSERT_EQUAL_CHAR('m', cmd.cmd);
    TEST_ASSERT_EQUAL(10, cmd.arg1);
    TEST_ASSERT_EQUAL(20, cmd.arg2);
    TEST_ASSERT_TRUE(cmd.valid);
    TEST_ASSERT_EQUAL(SerialError::NONE, cmd.error);

    // Second command: "a\n"
    static const char input2[] = "a\n";
    static int idx2 = 0;
    idx2 = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input2[idx2] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input2[idx2] != '\0') ? (int)input2[idx2++] : -1;
    });

    // Reset parser state if needed
    cmd = SerialCommand{};
    cmd.valid = true;
    parser.poll(cmd);

    TEST_ASSERT_EQUAL_CHAR('a', cmd.cmd);
    TEST_ASSERT_EQUAL(0, cmd.arg1);
    TEST_ASSERT_EQUAL(0, cmd.arg2);
    TEST_ASSERT_TRUE(cmd.valid);
    TEST_ASSERT_EQUAL(SerialError::NONE, cmd.error);
}

void INVALID_COMMAND_TEST(void) {
    // Simulate "z 1 2\n" (invalid command 'z')
    static const char input[] = "z 1 2\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    SerialCommand cmd{};
    parser.poll(cmd);

    TEST_ASSERT_FALSE(cmd.valid);
    TEST_ASSERT_EQUAL(SerialError::INVALID_COMMAND, cmd.error);
}

void ARG_OVERFLOW_TEST(void) {
    // Simulate "a " + long arg + "\n"
    char longArg[MAX_ARG_LENGTH + 5];
    memset(longArg, '9', MAX_ARG_LENGTH + 4);
    longArg[MAX_ARG_LENGTH + 4] = '\0';
    std::string inputStr = std::string("a ") + longArg + "\n";
    static std::string input = inputStr;
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    SerialCommand cmd{};
    cmd.valid = true;
    parser.poll(cmd);

    TEST_ASSERT_FALSE(cmd.valid);
    TEST_ASSERT_EQUAL(SerialError::ARG_OVERFLOW, cmd.error);
}

void ARG_NOT_NUMERIC_TEST(void) {
    // Simulate "a foo\n"
    static const char input[] = "a foo\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    SerialCommand cmd{};
    cmd.valid = true;
    parser.poll(cmd);

    TEST_ASSERT_FALSE(cmd.valid);
    TEST_ASSERT_EQUAL(SerialError::ARG_NOT_NUMERIC, cmd.error);
}

void DELIMITER_COMMA_TEST(void) {
    // Simulate "m,5,6\n"
    static const char input[] = "m,5,6\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    SerialCommand cmd{};
    cmd.valid = true;
    parser.poll(cmd);

    TEST_ASSERT_EQUAL_CHAR('m', cmd.cmd);
    TEST_ASSERT_EQUAL(5, cmd.arg1);
    TEST_ASSERT_EQUAL(6, cmd.arg2);
    TEST_ASSERT_TRUE(cmd.valid);
    TEST_ASSERT_EQUAL(SerialError::NONE, cmd.error);
}

void FLUSH_CMD_BUFFER_TEST(void) {
    // Simulate buffer with data, flush, ensure buffer is empty
    static const char input[] = "a 1 2\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    SerialCommand cmd{};
    cmd.valid = true;
    parser.poll(cmd);

  
    static const char input2[] = "a 23 45\n";
    static int idx2 = 0;
    idx2 = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input2[idx2] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input2[idx2] != '\0') ? (int)input2[idx2++] : -1;
    });

    parser.flushCmdBuffer(cmd, Serial);

    parser.poll(cmd);

    // After flush, buffer should be empty
    TEST_ASSERT_EQUAL_CHAR('\0', cmd.cmd);
    TEST_ASSERT_EQUAL(0, cmd.arg1);
    TEST_ASSERT_EQUAL(0, cmd.arg2);
    TEST_ASSERT_EQUAL(0,Serial.available());
}

void FLUSH_CMD_BUFFER_LIL_TEST(void) {
    // Simulate buffer with data, flush, ensure buffer is empty
    static const char input[] = "a 1 2\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    SerialCommand cmd{};
    cmd.valid = true;

    parser.flushCmdBuffer(cmd, Serial);


    // After flush, buffer should be empty
    TEST_ASSERT_EQUAL_CHAR('\0', cmd.cmd);
    TEST_ASSERT_EQUAL(0, cmd.arg1);
    TEST_ASSERT_EQUAL(0, cmd.arg2);
    TEST_ASSERT_EQUAL(0,Serial.available());
}

void ALL_CLEAR_SINGLE_CHAR_TEST(void) {
    // Simulate ALL_CLEAR character
    static const char input[] = "C";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    bool cleared = parser.allClear(Serial);
    TEST_ASSERT_TRUE(cleared);
}

void ALL_CLEAR_MULTI_CHAR_TEST(void) {
    // Simulate ALL_CLEAR character
    static const char input[] ="Cfder\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    bool cleared = parser.allClear(Serial);
    TEST_ASSERT_TRUE(cleared);
    TEST_ASSERT_EQUAL(0,Serial.available());
}

void ALL_CLEAR_FAIL_CHAR_TEST(void) {
    // Simulate ALL_CLEAR character
    static const char input[] ="fder\n";
    static int idx = 0;
    idx = 0;
    When(Method(ArduinoFake(Serial), available)).AlwaysDo([&]() {
        return (int)(input[idx] != '\0');
    });
    When(Method(ArduinoFake(Serial), read)).AlwaysDo([&]() {
        return (input[idx] != '\0') ? (int)input[idx++] : -1;
    });

    bool cleared = parser.allClear(Serial);
    TEST_ASSERT_FALSE(cleared);
    TEST_ASSERT_EQUAL(1,Serial.available());
}


int main(int argc, char **argv){
    UNITY_BEGIN();

    RUN_TEST(VALID_COMMAND_NO_ARG_TEST);
    RUN_TEST(VALID_COMMAND_ONE_ARG_TEST);
    RUN_TEST(VALID_COMMAND_TWO_ARG_TEST);
    RUN_TEST(VALID_COMMAND_MULTI_CMD_TEST);
    RUN_TEST(INVALID_COMMAND_TEST);
    RUN_TEST(ARG_OVERFLOW_TEST);
    RUN_TEST(ARG_NOT_NUMERIC_TEST);
    RUN_TEST(DELIMITER_COMMA_TEST);
    RUN_TEST(FLUSH_CMD_BUFFER_TEST);
    RUN_TEST(FLUSH_CMD_BUFFER_LIL_TEST);
    RUN_TEST(ALL_CLEAR_SINGLE_CHAR_TEST);
    RUN_TEST(ALL_CLEAR_MULTI_CHAR_TEST);
    RUN_TEST(ALL_CLEAR_FAIL_CHAR_TEST);

    UNITY_END();

    return 0;
}
#endif


















