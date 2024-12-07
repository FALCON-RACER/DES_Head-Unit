#ifndef CANBUSEXCEPTION_H
#define CANBUSEXCEPTION_H

#include <exception>
#include <string>

/**
 * @brief The CanBusException class
 *
 * For the handling Can Bus error
 */
class CanBusException : public std::exception
{
public:
    explicit CanBusException(const std::string& message) : message_(message) {}

    virtual const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

#endif
