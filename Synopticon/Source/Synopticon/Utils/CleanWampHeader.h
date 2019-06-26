#pragma once

#pragma warning(push)
#pragma warning(disable: 4668)
#pragma warning(disable: 4191)
#pragma warning(disable: 4005)
#pragma warning(disable: 4503)
#pragma warning(disable: 4643)

#define UI UI_ST
THIRD_PARTY_INCLUDES_START

#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#include "AllowWindowsPlatformTypes.h"
#include "AllowWindowsPlatformAtomics.h"
#endif

#include <autobahn.hpp>
#include <wamp_websocketpp_websocket_transport.hpp>

#if PLATFORM_WINDOWS
#include "HideWindowsPlatformAtomics.h"
#include "HideWindowsPlatformTypes.h"
#endif

using namespace autobahn;

THIRD_PARTY_INCLUDES_END
#undef UI

#pragma warning(pop)
