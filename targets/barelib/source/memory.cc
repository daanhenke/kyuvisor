#include "barelib/memory.hh"

extern "C" void* memcpy(void* dst, const void* src, unsigned int cnt)
{
    char *pszDest = (char *)dst;
    const char *pszSource =( const char*)src;
    if((pszDest!= nullptr) && (pszSource!= nullptr))
    {
        while(cnt) //till cnt
        {
            //Copy byte by byte
            *(pszDest++)= *(pszSource++);
            --cnt;
        }
    }
    return dst;
}


extern "C" void* memset(void* address, char value, unsigned int cnt)
{
    char* dest = (char*) address;

    if (dest != nullptr)
    {
        while (cnt)
        {
            *(dest++) = value;
            cnt--;
        }
    }

    return address;
}
