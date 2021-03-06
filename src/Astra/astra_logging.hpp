#ifndef ASTRA_LOGGING_H
#define ASTRA_LOGGING_H

// TODO valgrind will go bananas with the default crash handler enabled
// #define ELPP_DISABLE_DEFAULT_CRASH_HANDLING
#define ELPP_NO_DEFAULT_LOG_FILE

// enable stacktraces for GCC/Clang on *nixes
#if ! defined(__ANDROID__) && ! defined(_MSC_VER)
#define ELPP_STACKTRACE_ON_CRASH
#endif

#include "vendor/easylogging++.h"
#include <Astra/astra_types.h>

#define INITIALIZE_LOGGING INITIALIZE_EASYLOGGINGPP

namespace astra {
    void initialize_logging(const char* logFilePath, astra_log_severity_t severity);
}

#endif /* ASTRA_LOGGING_H */
