/**
 * @file serial_parser.h
 * @brief Serial command parser for ROSEspBridge.
 *
 * This module provides the serialParser class, which parses incoming serial data
 * into structured SerialCommand objects. It supports commands with up to two
 * numeric arguments, error reporting, and is designed for use with Arduino Stream
 * objects (such as Serial).
 *
 * Usage:
 *   1. Call serialParser::init() in setup().
 *   2. In loop(), call parser.poll(cmd) to process incoming data.
 *   3. If poll() returns true and cmd.valid is true, pass cmd to the executor.
 *
 * Example:
 *   serialParser parser;
 *   SerialCommand cmd;
 *   void setup() { parser.init(); }
 *   void loop() {
 *     if (parser.poll(cmd)) {
 *       if (cmd.valid) { #execute command# }
 *       else { Serial.println(parser.getError()); }
 *     }
 *   }
 */

#pragma once
#include "config.h"
#include "serial_command.h"

#ifndef TEST_DESKTOP
#include <Stream.h>
#endif

/**
 * @brief Checks if a character is a valid command.
 * 
 * @param c The command character to check.
 * @return true if valid, false otherwise.
 */
bool isValidCommand(char c);

class serialParser
{
    public:
    
    /**
     * @brief Initializes the serial communication system (safe to call multiple times).
     * 
     * Call this in setup() before using the parser.
     */
    static void init()
    {
        static bool initialised = 0;
        if (!initialised)
        {
            init2();
            initialised = true;
        }
    }

    /**
     * @brief Processes incoming serial data and parses commands.
     * 
     * Call this frequently (e.g., in loop()) to process incoming serial data.
     * Accepts any Stream (default: Serial).
     * 
     * @param outCmd Reference to a SerialCommand struct to fill with parsed data.
     * @param input  Stream to read from (default: Serial).
     * @return true if a complete command was parsed, false otherwise.
     */
    bool poll(SerialCommand &outCmd, Stream &input = Serial);


    private:
    // Tracks number of command arguments (0,1 or 2 in current implimentation)
    int argCount = 0;

    // Index for filling argument character arrays (argv*)
    int argIndex = 0; 
    
    // Holds the most recently read character from Serial
    char serialCommandChar = '\0';  

    // Stores the single-character command identifier
    char cmd = '\0';                

    // True if a command character has been fully received
    bool cmdComplete = false;        

    // Buffers for the first and second command arguments (as strings)
    char argv1[MAX_ARG_LENGTH];
    char argv2[MAX_ARG_LENGTH];

    // Parsed numeric values of argv1 and argv2
    long arg1;
    long arg2;


    bool errorStateOverFlow = false;

    /**
     * @brief Initializes the serial port (called once internally).
     */
    static void init2();

    /**
     * @brief Resets all parsing state and buffers for the next command.
     */
    void reset();



};