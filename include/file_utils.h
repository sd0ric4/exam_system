#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <cjson/cJSON.h>

cJSON *load_question_bank(const char *filename);

#endif // FILE_UTILS_H