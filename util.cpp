#include <extdll.h>
#include <enginecallback.h>

#include <stdarg.h>
#include <stdio.h>

void Log(const char *fmt, ...)
{
    char buffer[1024];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    char finalBuffer[1100];
    snprintf(finalBuffer, sizeof(finalBuffer),
        "[xash_redirect] %s\n", buffer);

    g_engfuncs.pfnServerPrint(finalBuffer);
}
