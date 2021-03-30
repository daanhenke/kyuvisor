#include "barelib/string.hh"

int strlen(const char* string)
{
    int i = 0;
    while (string[i] != '\0') i++;
    return i;
}

int strcmp(const char* s1, const char* s2)
{
    while (*s1 == *s2 && *s1 != '\0')
    {
        s1++;
        s2++;
    }

    return (*(unsigned char*) s1) - (*(unsigned char*) s2);
}