// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Memory/StringSlot.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/mpl/max.hpp>
#include <ArduinoJson/Variant/VariantSlot.hpp>

#include <string.h>  // memmove

namespace ARDUINOJSON_NAMESPACE {

// _begin                                   _end
// v                                           v
// +-------------+--------------+--------------+
// | strings...  |   (free)     |  ...variants |
// +-------------+--------------+--------------+
//               ^              ^
//             _left          _right

class MemoryPool {
 public:
	 MemoryPool(char* buf, size_t capa)
		 : _begin(buf), _end(buf ? buf + capa : 0) {		 
		clear();
    ARDUINOJSON_ASSERT(isAligned(_begin));
    ARDUINOJSON_ASSERT(isAligned(_right));
    ARDUINOJSON_ASSERT(isAligned(_end));
  }

	inline VariantSlot* toSlot(VariantSlotOffset offset) {
		return (VariantSlot *)(_end - offset);
	}

	inline const char *toCharPtr(VariantKey keyOffset) {
		return (const char *)_begin + keyOffset;
	}

	inline VariantKey toVariantKey(const char *key) {
		return (VariantKey)(key - _begin);
	}

  void* buffer() {
    return _begin;
  }
	
	const char* left() {
		return (const char*)_left;
	}

	inline VariantSlot* rootSlot() {
		return (VariantSlot *)_end - 1;
	}

	inline VariantData* rootVariant() {
		return rootSlot()->data();
	}

	// Gets the capacity of the memoryPool in bytes
  size_t capacity() const {
    return size_t(_end - _begin);
  }

  size_t size() const {
    return size_t(_left - _begin + _end - _right);
  }

  VariantSlot* allocVariant() {
    return allocRight<VariantSlot>();
  }

  char* allocFrozenString(size_t n) {
    if (!canAlloc(n))
      return 0;
    char* s = _left;
    _left += n;
    checkInvariants();
    return s;
  }

  StringSlot allocExpandableString() {
    StringSlot s;
    s.value = _left;
    s.size = size_t(_right - _left);
    _left = _right;
    checkInvariants();
    return s;
  }

	VariantKey allocLinkedKey(char *key) {
		if (!canAlloc(sizeof(char *)))
			return 0;
		char *p = (char *) memcpy(_left, &key, sizeof(char *));
		checkInvariants();
		_left -= sizeof(char *);
		return (VariantKey)(p - _begin);
	}

	VariantKey allocOwnedKey(char *key, size_t len) {
		if (!canAlloc(++len))
			return 0;
		char *p = (char *) memcpy(_left, key, len);
		checkInvariants();
		_left -= len;
		return (VariantKey)(p - _begin);
	}

  void freezeString(StringSlot& s, size_t newSize) {
    _left -= (s.size - newSize);
    s.size = newSize;
    checkInvariants();
  }

  inline void reclaimLastString(const char* s) {
    _left = const_cast<char*>(s);
  }

  void clear() {
    _left = _begin + (sizeof(char *) * 2); // placeholders for sharedPool/jsonString, ensures valid string/key offset is never 0 
		_right = _end - sizeof(VariantSlot);	// placeholder for rootVariant
		((VariantSlot *)_right)->clear();			// make sure rootVariant slot is cleared
  }

  inline bool canAlloc(size_t bytes) const {
    return _left + bytes <= _right;
  }

  bool owns(void* p) const {
    return _begin <= p && p < _end;
  }

  template <typename T>
  T* allocRight() {
    return reinterpret_cast<T*>(allocRight(sizeof(T)));
  }

  void* allocRight(size_t bytes) {
    if (!canAlloc(bytes))
      return 0;
    _right -= bytes;
    return _right;
  }

  // Workaround for missing placement new
  void* operator new(size_t, void* p) {
    return p;
  }

  // Squash the free space between strings and variants
  //
  // _begin                    _end
  // v                            v
  // +-------------+--------------+
  // | strings...  |  ...variants |
  // +-------------+--------------+
  //               ^
  //          _left _right
  //
  // This funcion is called before a realloc.
  ptrdiff_t squash() {
    char* new_right = addPadding(_left);
    if (new_right >= _right)
      return 0;

    size_t right_size = static_cast<size_t>(_end - _right);
    memmove(new_right, _right, right_size);

    ptrdiff_t bytes_reclaimed = _right - new_right;
    _right = new_right;
    _end = new_right + right_size;
    return bytes_reclaimed;
  }

  // Move all pointers together
  // This funcion is called after a realloc.
  void movePointers(ptrdiff_t offset) {
    _begin += offset;
    _left += offset;
    _right += offset;
    _end += offset;
  }


 private:
  StringSlot* allocStringSlot() {
    return allocRight<StringSlot>();
  }

  void checkInvariants() {
    ARDUINOJSON_ASSERT(_begin <= _left);
    ARDUINOJSON_ASSERT(_left <= _right);
    ARDUINOJSON_ASSERT(_right <= _end);
    ARDUINOJSON_ASSERT(isAligned(_right));
  }

  char *_begin, *_left, *_right, *_end;
};

}  // namespace ARDUINOJSON_NAMESPACE
