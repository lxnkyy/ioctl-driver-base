#pragma once
#include <Windows.h>
#include <iostream>

#include <TlHelp32.h>
#include <cstdint>

#include <dependencies/additional/globals.hpp>

#include <dependencies/communication/driver.hpp>

#define e_log( fmt, ... ) printf( " [usermode] -> " fmt "\n", ##__VA_ARGS__ )