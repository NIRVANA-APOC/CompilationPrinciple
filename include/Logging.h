#pragma once

#include <iostream>
#include <fstream>

#define DEBUG 0
#define INFO 1
#define ERR 2
#define __DEBUG__ 1

extern FILE *log_fp;

#define LOG_ON(filename) log_fp = fopen(filename, "w");
#define LOG_OFF fclose(log_fp);

#if __DEBUG__
#define Log(option, msg, ...)                             \
    do                                                    \
    {                                                     \
        if (option == DEBUG)                              \
        {                                                 \
            fprintf(log_fp, "[DEBUG](%s:%d) " msg " \n",  \
                    __FILE__, __LINE__, ##__VA_ARGS__);   \
            printf("[DEBUG] " msg " \n", ##__VA_ARGS__);  \
        }                                                 \
        else if (option == INFO)                          \
        {                                                 \
            fprintf(log_fp, "[INFO] " msg " \n",          \
                    ##__VA_ARGS__);                       \
            printf("[INFO] " msg " \n", ##__VA_ARGS__);   \
        }                                                 \
        else if (option == ERR)                           \
        {                                                 \
            fprintf(log_fp, "[ERROR](%s:%d): " msg " \n", \
                    __FILE__, __LINE__, ##__VA_ARGS__);   \
            printf("[ERROR] " msg " \n", ##__VA_ARGS__);  \
        }                                                 \
    } while (0)
#else
#define Log(option, msg, ...)                          \
    do                                                 \
    {                                                  \
        if (option == INFO)                            \
        {                                              \
            fprintf(log_fp, msg " \n", ##__VA_ARGS__); \
        }                                              \
    } while (0)

#endif
