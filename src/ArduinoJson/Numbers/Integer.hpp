// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Namespace.hpp>

#include <stdint.h>  // int64_t

namespace ARDUINOJSON_NAMESPACE {

#if ARDUINOJSON_USE_LONG_LONG
typedef int64_t Integer;
//typedef uint64_t UInt;
typedef int64_t UInt;
#else
typedef long Integer;
//typedef unsigned long UInt;
typedef long UInt;
#endif
}  // namespace ARDUINOJSON_NAMESPACE
