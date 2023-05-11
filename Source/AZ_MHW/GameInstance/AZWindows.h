#pragma once
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include "Windows/MinWindows.h"
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <mswsock.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <time.h>
//c library
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock.lib")



//c++ library
//#include <ios>//io_base->ios
//#include <istream>//istream-> iostream <-ostream
#include <iostream>//cin, cout
#include <fstream>//ifstream-> fstream <-ofstream
#include <sstream>//istringstream-> stringstream <-ostringstream
//#include <streambuf>
#include <iomanip>
#include <stdexcept>//#include <exception>
#include <locale>//#include <codecvt>//cvt.from_bytes(), cvt.to_bytes()
#include <memory>//smart_pointer, _pointer_cast
#include <type_traits>
#include <limits>//numeric_limits<T>::min, max, is_specialized

#include <complex>
#include <string>

#include <array>
#include <iterator>
#include <algorithm>// std::min, std::max, sort
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <tuple>

#include <functional>
#include <typeinfo>//typeid(&)
#include <thread>
#include <chrono>
#include <random>
#include <regex>

#include <ctime>
#include <mutex>
#include <utility>
#include <cstring>


#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"










