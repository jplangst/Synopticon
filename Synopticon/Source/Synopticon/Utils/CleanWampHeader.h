#pragma once

#pragma warning(push)
#pragma warning(disable: 4668)
#pragma warning(disable: 4191)
#pragma warning(disable: 4005)
#pragma warning(disable: 4503)
#pragma warning(disable: 4643)


#include "AllowWindowsPlatformTypes.h"
#include "AllowWindowsPlatformAtomics.h"
//#include "MacroUtility/UndefineMacros_UE_4.20.h"
#include <autobahn.hpp>
#include <wamp_websocketpp_websocket_transport.hpp>


//#include "MacroUtility/RedefineMacros_UE_4.20.h"
#include "HideWindowsPlatformAtomics.h"
#include "HideWindowsPlatformTypes.h"

using namespace autobahn;
//#include "AllowWindowsPlatformTypes.h"
//#include "AllowWindowsPlatformAtomics.h"
//#include <wamp_websocketpp_websocket_transport.hpp>
//#include <autobahn.hpp>
//#include "HideWindowsPlatformAtomics.h"
//#include "HideWindowsPlatformTypes.h"

#pragma warning(pop)
