#ifndef LOGGING_LOGGING_BASE_H
#define LOGGING_LOGGING_BASE_H

#include <stdexcept>

namespace logging {
class LoggingBase {
public:
    void throw_if_disposed() const
    {
        if (_isDisposed) {
            throw std::runtime_error("Object disposed.");
        }
    }

    

private:
    bool _isDisposed = false;
};
}  // namespace logging

#endif  // LOGGING_LOGGING_BASE_H
