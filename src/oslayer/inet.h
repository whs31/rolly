#pragma once

#include <rolly/global/platform_definitions.h>

#if defined(ROLLY_OS_WINDOWS)
# include <winsock2.h>
#else
# include <arpa/inet.h>
#endif