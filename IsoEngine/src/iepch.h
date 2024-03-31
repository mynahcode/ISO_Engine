#pragma once

/* Necessary C++ Libs */
#include <iostream>
#include <filesystem>
#include <memory>
#include <array>
#include <utility>
#include <algorithm>
#include <sstream>
#include <functional>

/* C Libraries */
#include <stdio.h>
#include <mutex>
#include <ctime>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

/* C++ Data Structures */
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>

/* Debug and Logging */
// TODO: Add ifdef define for debugging/logging and implement in this file and all others that use these classes.
#include "IsoEngine/IsoLogger/IsoLogger.h"
#include "IsoEngine/Debug/Instrumentor.h"

/* Platform Specific Includes*/
#ifdef _IE_PLATFORM_WINDOWS
#include <Windows.h>
#endif