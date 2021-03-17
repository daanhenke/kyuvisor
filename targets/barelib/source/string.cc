#include "barelib/string.hh"

int string_length(const char* string)
{
    int i = 0;
    while (string[i] != '\0') i++;
    return i;
}