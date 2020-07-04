// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Polyfills/gsl/not_null.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantContent.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

#include <stdint.h>  // int8_t, int16_t

namespace ARDUINOJSON_NAMESPACE {

//typedef conditional<sizeof(void*) <= 2, int8_t, int16_t>::type VariantSlotDiff;
#define FLAG_MASK 0xF
#define KEY_FLAG_MASK 0x3

#if ARDUINOJSON_EMBEDDED_MODE
typedef int16_t VariantSlotDiff; 
#define NEXT_MASK 0xFFF0
typedef int16_t VariantKey;
//typedef conditional<sizeof(void*) <= 2, int8_t, int16_t>::type VariantKeyOffset;
#define KEY_MASK 0x3FFF
//#if __SIZEOF_POINTER__ = 2
#else 
typedef int32_t VariantSlotOffset;
#define NEXT_MASK 0xFFFFFFF0
typedef int32_t VariantKey;
#define KEY_MASK 0x3FFFFFFF
#endif

class VariantSlot {
  // CAUTION: same layout as VariantData
  // we cannot use composition because it adds padding
  // (+20% on ESP8266 for example)
 public:
  VariantContent _content;
  //uint8_t _flags;
	union {
		uint8_t _flags;	// first 4 bits
		VariantSlotOffset _next;	// offset shifted by 4
	};
	VariantKey _key;
  //const char* _key;

 //public:
  // Must be a POD!
  // - no constructor
  // - no destructor
  // - no virtual
  // - no inheritance

	inline uint8_t flags() {
		return _flags & FLAG_MASK;
	}
	
	inline VariantData* data() {
    return reinterpret_cast<VariantData*>(&_content);
  }

  inline const VariantData* data() const {
    return reinterpret_cast<const VariantData*>(&_content);
  }

  inline VariantSlot* next() {
    //return _next ? this + _next : 0;
		VariantSlotOffset offset = _next & NEXT_MASK; // strip lower 5 (flags) bits
		return (offset) ? this - (offset >> 4) : 0;
  }

  inline const VariantSlot* next() const {
    return const_cast<VariantSlot*>(this)->next();
  }

  VariantSlot* next(size_t distance) {
    VariantSlot* slot = this;
		VariantSlotOffset offset;
    while (distance--) {
			offset = slot->_next >> 4;
      if (!offset)
        return 0;
      slot -= offset;
    }
    return slot;
  }

  const VariantSlot* next(size_t distance) const {
    return const_cast<VariantSlot*>(this)->next(distance);
  }

  inline void setNext(VariantSlot* slot) {
    _next = VariantSlotOffset(slot ? _next | ((this - slot) << 4) : 0);
  }

  void setNextNotNull(VariantSlot* slot) {
    ARDUINOJSON_ASSERT(slot != 0);
    _next |= VariantSlotOffset((this - slot) << 4);
  }

  //void setOwnedKey(not_null<const char*> k) {
	inline void setOwnedKey(not_null<VariantKey> k) {
		//_flags |= KEY_IS_OWNED;
    //_key = k.get();
		_key = k.get() <<  2;
  }

	//void setLinkedKey(not_null<const char*> k) {
	inline void setLinkedKey(not_null<VariantKey> k) {
    //_flags &= VALUE_MASK;
    //_key = k.get();
		_key = (k.get() << 2) | KEY_IS_LINKED;
  }

	inline VariantKey key() const {
		return _key >> 2;
  }

	inline uint8_t keyFlags() const {
		return (uint8_t)(_key & KEY_FLAG_MASK);
	}

  inline bool ownsKey() const {
    return (_key & KEY_FLAG_MASK) == KEY_IS_OWNED;
  }

  void clear() {
    _next = 0;
    //_flags = 0;
    _key = 0;
  }

  void movePointers(ptrdiff_t stringDistance, ptrdiff_t variantDistance) {
		/*
    if (_flags & KEY_IS_OWNED)
      _key += stringDistance;
    if (_flags & VALUE_IS_OWNED)
      _content.asString += stringDistance;
    if (_flags & COLLECTION_MASK)
      _content.asCollection.movePointers(stringDistance, variantDistance);
				*/
  }

};

}  // namespace ARDUINOJSON_NAMESPACE
