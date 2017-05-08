#pragma once
#include "targetver.h"

// Runtime headers
#include <stdint.h>

// STL headers
#include <array>
#include <cstdio>
#include <exception>

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

// Windows headers
#include <Windows.h>
#include <Wininet.h>
#include <Msi.h>
