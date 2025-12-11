#ifndef CUSTOMEXCEPTION_H
#define CUSTOMEXCEPTION_H

#include <string>
#include <exception>

// EXCEPTION HANDLING: Custom exception class hierarchy
class GameException : public std::exception {
public:
    enum Type {
        FILE_ERROR,
        RESOURCE_ERROR,
        LOGIC_ERROR,
        MEMORY_ERROR
    };

private:
    Type errorType;
    std::string message;
    std::string fullMessage;

public:
    // Constructor
    GameException(Type type, const std::string& msg) 
        : errorType(type), message(msg) {
        
        std::string typeStr;
        switch(type) {
            case FILE_ERROR:  typeStr = "FILE ERROR"; break;
            case RESOURCE_ERROR: typeStr = "RESOURCE ERROR"; break;
            case LOGIC_ERROR: typeStr = "LOGIC ERROR"; break;
            case MEMORY_ERROR: typeStr = "MEMORY ERROR"; break;
        }
        
        fullMessage = "[" + typeStr + "] " + message;
    }

    // CONST CORRECTNESS: Doesn't modify exception object
    const char* what() const noexcept override {
        return fullMessage.c_str();
    }

    Type getType() const { return errorType; }
    const std::string& getMessage() const { return message; }
};

// Specific exception types (INHERITANCE)
class FileException : public GameException {
public: 
    FileException(const std::string& filename) 
        : GameException(FILE_ERROR, "Cannot access file: " + filename) {}
};

class ResourceException : public GameException {
public:
    ResourceException(const std::string& resource) 
        : GameException(RESOURCE_ERROR, "Failed to load resource: " + resource) {}
};

class LogicException : public GameException {
public: 
    LogicException(const std::string& msg) 
        : GameException(LOGIC_ERROR, msg) {}
};

#endif