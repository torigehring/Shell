#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG_ENABLE
    #define debug(...) do { \
            printf("[%12s:%3d]%-18s(): ", __FILE__, __LINE__, __func__); \
            printf(__VA_ARGS__); \
        } while(0)
#else
    #define debug(...) do{} while(0)
#endif // DEBUG

#endif // DEBUG_H