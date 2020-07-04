// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <stddef.h>  // size_t

#include <ArduinoJson/Collection/CollectionData.hpp>
#include <ArduinoJson/Numbers/Float.hpp>
#include <ArduinoJson/Numbers/Integer.hpp>

#if ARDUINOJSON_EMBEDDED_MODE
typedef int16_t VariantString;
typedef int16_t VariantStringLength;
#else 
typedef int32_t VariantString;
typedef int32_t VariantStringLength;
#endif

namespace ARDUINOJSON_NAMESPACE {

//
/*enum {
  VALUE_MASK = 0x7F,

  VALUE_IS_OWNED = 0x01,
  VALUE_IS_NULL = 0,
  VALUE_IS_LINKED_RAW = 0x02,
  VALUE_IS_OWNED_RAW = 0x03,
  VALUE_IS_LINKED_STRING = 0x04,
  VALUE_IS_OWNED_STRING = 0x05,

  // CAUTION: no VALUE_IS_OWNED below
  VALUE_IS_BOOLEAN = 0x06,
  VALUE_IS_POSITIVE_INTEGER = 0x08,
  VALUE_IS_NEGATIVE_INTEGER = 0x0A,
  VALUE_IS_FLOAT = 0x0C,

  COLLECTION_MASK = 0x60,
  VALUE_IS_OBJECT = 0x20,
  VALUE_IS_ARRAY = 0x40,

  KEY_IS_OWNED = 0x80
};*/
	enum {

		VALUE_IS_OWNED = 0x01,
		VALUE_IS_NULL = 0x00,
		VALUE_IS_LINKED_RAW = 0x02,
		VALUE_IS_OWNED_RAW = 0x03,
		VALUE_IS_LINKED_STRING = 0x04,
		VALUE_IS_OWNED_STRING = 0x05,

		// CAUTION: no VALUE_IS_OWNED below
		VALUE_IS_BOOLEAN = 0x06,
		VALUE_IS_INTEGER = 0x07,
		VALUE_IS_POSITIVE_INTEGER = 0x07,
		VALUE_IS_NEGATIVE_INTEGER = 0x07,
		VALUE_IS_FLOAT = 0x01,

		// Collections
		COLLECTION_MASK = 0x08,
		VALUE_IS_OBJECT = 0x0C,
		VALUE_IS_ARRAY = 0x0A,
		VALUE_IS_OWNED_OBJECT = 0x0B,
		VALUE_IS_OWNED_ARRAY = 0x0D,
		VALUE_IS_LINKED_OBJECT = 0x0A,
		VALUE_IS_LINKED_ARRAY = 0x0C,

		KEY_IS_OWNED = 0x00,
		KEY_IS_IN_JSON_STRING = 0x01,
		KEY_IS_IN_FLASH = 0x10,
		KEY_IS_LINKED = 0x11
};

struct RawData {
  const char *data;
  size_t size;
};

union VariantContent {
  Float asFloat;
  UInt asInteger;
  CollectionData asCollection;
  const char *asString;
	struct {
		VariantString data;
		VariantStringLength size;
	} asOwnedString;
	/*
	struct {
		VariantString data;
		VariantStringLength size;
	} asRaw;
	*/
  struct {
    const char *data;
    size_t size;
  } asRaw;
};
}  // namespace ARDUINOJSON_NAMESPACE
