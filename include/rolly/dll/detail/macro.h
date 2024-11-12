#pragma once

#include "../../global.h"

#define ___rolly_dll_proc_name___ __rolly_dll_plugin_entry_procedure
#define ___rolly_dll_proc_name_string___ ___rolly_stringify___(__rolly_dll_plugin_entry_procedure)
#ifdef ROLLY_OS_WINDOWS
#  define ___rolly_dll_api___ __declspec(dllexport)
#else
#  define ___rolly_dll_api___
#endif
#define DECLARE_PLUGIN(ID) extern "C" ___rolly_dll_api___ rolly::dll::plugin* ___rolly_dll_proc_name___ () { return reinterpret_cast<rolly::dll::plugin*>(new ID); }
