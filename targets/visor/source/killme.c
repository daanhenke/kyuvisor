#include "killme.h"

void StartCPU()
{

}

typedef void (*start_cpu_t)();

void CrashMe()
{
    start_cpu_t start_indirect = StartCPU;
    start_indirect();
}