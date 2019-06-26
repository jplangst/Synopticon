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


//#include "ThirdParty/OpenSSL/1.0.2g/include/Win64/VS2015/openssl/pem.h"
//#include "openssl/ssl.h"
//#include "ThirdParty/OpenSSL/1.0.2g/include/Win64/VS2015/openssl/rsa.h"
//#include "ThirdParty/OpenSSL/1.0.2g/include/Win64/VS2015/openssl/evp.h"
//#include "ThirdParty/OpenSSL/1.0.2g/include/Win64/VS2015/openssl/bio.h"
//#include "ThirdParty/OpenSSL/1.0.2g/include/Win64/VS2015/openssl/err.h"
//#include "ThirdParty/OpenSSL/1.0.2g/include/Win64/VS2015/openssl/bn.h"
//#include "ThirdParty/OpenSSL/1.0.2g/include/Win64/VS2015/openssl/x509.h"
//#include "openssl/hmac.h"


//#include "MacroUtility/UndefineMacros_UE_4.20.h"
#include <autobahn.hpp>
#include <wamp_websocketpp_websocket_transport.hpp>


//#include "MacroUtility/RedefineMacros_UE_4.20.h"
#if PLATFORM_WINDOWS
#include "HideWindowsPlatformAtomics.h"
#include "HideWindowsPlatformTypes.h"
#endif

using namespace autobahn;
//#include "AllowWindowsPlatformTypes.h"
//#include "AllowWindowsPlatformAtomics.h"
//#include <wamp_websocketpp_websocket_transport.hpp>
//#include <autobahn.hpp>
//#include "HideWindowsPlatformAtomics.h"
//#include "HideWindowsPlatformTypes.h"

THIRD_PARTY_INCLUDES_END
#undef UI

#pragma warning(pop)
