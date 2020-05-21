#include "hkxutils.h"

/* ----------FILE* COMPATIBLITY VS2008---------------*/

#if defined(_M_IX86)

#define GET_CURRENT_CONTEXT(c, contextFlags) \
    do { \
    (c).ContextFlags = contextFlags; \
    __asm    call x \
    __asm x: pop eax \
    __asm    mov c.Eip, eax \
    __asm    mov c.Ebp, ebp \
    __asm    mov c.Esp, esp \
    } while(0);

#endif

extern "C" FILE * __cdecl __iob_func(void)
{
    CONTEXT c = { 0 };
    STACKFRAME64 s = {{0}};
    DWORD imageType;

    GET_CURRENT_CONTEXT(c, CONTEXT_FULL)

#ifdef _M_IX86
    imageType = IMAGE_FILE_MACHINE_I386;
    s.AddrPC.Offset = c.Eip;
    s.AddrPC.Mode = AddrModeFlat;
    s.AddrFrame.Offset = c.Ebp;
    s.AddrFrame.Mode = AddrModeFlat;
    s.AddrStack.Offset = c.Esp;
    s.AddrStack.Mode = AddrModeFlat;
#else
#error "Platform not supported!"
#endif
    /*
    if (!StackWalk64(imageType, hProcess, hThread, &s, &c, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
        return nullptr;
*/
    if (s.AddrReturn.Offset == 0)
        return nullptr;

    {
        auto const * assembly = (unsigned char const *)(s.AddrReturn.Offset);

        if (*assembly == 0x83 && *(assembly + 1) == 0xC0 && (*(assembly + 2) == 0x20 || *(assembly + 2) == 0x40))
        {
            if (*(assembly + 2) == 32)
            {
                return (FILE*)((unsigned char *)stdout - 32);
            }
            if (*(assembly + 2) == 64)
            {
                return (FILE*)((unsigned char *)stderr - 64);
            }

        }
        else
        {
            return stdin;
        }
    }
    return nullptr;
}

/* -----END FILE COMPATIBLITY VS2008---------------*/
