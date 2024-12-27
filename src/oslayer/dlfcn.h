#pragma once

#include <rolly/global/platform_definitions.h>

#if defined(ROLLY_OS_WINDOWS)
# include "win/dlfcn_win32.h"
#else
# include <dlfcn.h>
#endif