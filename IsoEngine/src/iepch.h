#pragma once

/* Necessary C++ Libs */
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <sstream>
#include <functional>

/* C Libraries */
#include <stdio.h>
#include <mutex>
#include <ctime>

/* C++ Data Structures */
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

/* IsoEngine Utilities */
#include "IsoEngine/IsoLogger/IsoLogger.h"

/* Platform Specific Includes*/
#ifdef _IE_PLATFORM_WINDOWS
#include <Windows.h>
#endif