//
// EnlyzeWinCompatLib - Let Clang-compiled applications run on older Windows versions
// Copyright (c) 2021 Colin Finck, ENLYZE GmbH <c.finck@enlyze.com>
// SPDX-License-Identifier: MIT
//

// This file implements required APIs not available in Windows XP RTM (NT 5.1).
#include "EnlyzeWinCompatLibInternal.h"

typedef PVOID (WINAPI *PFN_DECODEPOINTER)(PVOID Ptr);
typedef PVOID (WINAPI *PFN_ENCODEPOINTER)(PVOID Ptr);

static PFN_DECODEPOINTER pfnDecodePointer = nullptr;
static PFN_ENCODEPOINTER pfnEncodePointer = nullptr;

static PVOID WINAPI
_CompatDecodePointer(PVOID Ptr)
{
    // Just return the input pointer without any decoding.
    return Ptr;
}

static PVOID WINAPI
_CompatEncodePointer(PVOID Ptr)
{
    // Just return the input pointer without any encoding.
    return Ptr;
}

extern "C" PVOID WINAPI
LibDecodePointer(PVOID Ptr)
{
    if (!pfnDecodePointer)
    {
        HMODULE hKernel32 = GetModuleHandleW(L"kernel32");
        FARPROC proc = GetProcAddress(hKernel32, "DecodePointer");
        if (proc)
        {
            pfnDecodePointer = reinterpret_cast<PFN_DECODEPOINTER>(reinterpret_cast<intptr_t>(proc));
        }
        else
        {
            pfnDecodePointer = _CompatDecodePointer;
        }
    }

    return pfnDecodePointer(Ptr);
}

extern "C" PVOID WINAPI
LibEncodePointer(PVOID Ptr)
{
    if (!pfnEncodePointer)
    {
        HMODULE hKernel32 = GetModuleHandleW(L"kernel32");
        FARPROC proc = GetProcAddress(hKernel32, "EncodePointer");
        if (proc)
        {
            pfnEncodePointer = reinterpret_cast<PFN_ENCODEPOINTER>(reinterpret_cast<intptr_t>(proc));
        }
        else
        {
            pfnEncodePointer = _CompatEncodePointer;
        }
    }

    return pfnEncodePointer(Ptr);
}
