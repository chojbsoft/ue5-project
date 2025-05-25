#pragma once


// I/O
#include <iostream>

// utils
#include <chrono>

// container
#include <vector>
#include <thread>
#include <map>
#include <unordered_map>
#include <string>
#include <queue>

// boost
#include "boost\asio.hpp"
using boost::asio::ip::tcp;
using Socket = boost::asio::ip::tcp::socket;
#include "boost\pool\pool.hpp"

// assert
#include <assert.h>
#define CHECK(p) assert(SUCCEEDED(p))

// types
using namespace std;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// path
extern wstring SOLUTION_DIR;
extern wstring SERVER_DIR;

extern string IP;
extern uint16 MAIN_SERVER_PORT;
extern uint16 DEDI_SERVER_PORT;