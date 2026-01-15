#include "common/utils/string_utils.h"

#include <stdbool.h>
#include <string.h>

bool StringIsNullOrEmpty(const char *str)
{
    return (str == NULL || strlen(str) == 0);
}
