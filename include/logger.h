#ifndef LOGGER_H
#define LOGGER_H

#define PRINT(fmt, x) printf(#x " = " fmt "\n", (x))

#define LOG(...) printf(__VA_ARGS__)
#define LOG_INFO(...) printf("[INFO] " __VA_ARGS__)
#define LOG_WARN(...) printf("[WARN] " __VA_ARGS__)
#define LOG_ERROR(...) printf("[ERROR] " __VA_ARGS__)

#define DEBUG()                                                     \
    printf("FILE: %s\nLINE: %d\nFUNC: %s\n\n",                      \
        __FILE__,                                                   \
        __LINE__,                                                   \
        __func__)


#endif // LOGGER_H