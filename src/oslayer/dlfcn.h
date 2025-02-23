#pragma once

#include <rll/global/platform_definitions.h>

#if defined(RLL_OS_WINDOWS)
#  include "win/dlfcn_win32.h"
#else
#  include <dlfcn.h>
#endif
