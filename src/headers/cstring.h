#ifndef REX_CSTRING_H
#define REX_CSTRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define string char *

// Source: https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
// Stores the trimmed input string uinto the given output buffer, which must be
// large enough to store the result.  If it is too small, the output is
// truncated.
size_t trimwhitespace(string out, size_t len, const string str)
{
    if (len == 0)
        return 0;

    const string end;
    size_t out_size;

    // Trim leading space
    while (isSpace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
    {
        *out = 0;
        return 1;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isSpace((unsigned char)*end))
        end--;
    end++;

    // Set output size to minimum of trimmed string length and buffer size minus 1
    out_size = (unsigned long)(end - str) < len - 1 ? (end - str) : len - 1;

    // Copy trimmed string and add null terminator
    memcpy(out, str, out_size);
    out[out_size] = 0;

    return out_size;
}

#endif
