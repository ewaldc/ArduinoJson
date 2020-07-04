// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Strings/String.hpp>
#include <ArduinoJson/Variant/VariantRef.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>

namespace ARDUINOJSON_NAMESPACE {
// A key value pair for CollectionData.
class Pair {
 public:
  Pair(MemoryPool* pool, VariantSlot* slot) {
    if (slot) {
      _key = String(pool->toCharPtr(slot->key()), !slot->ownsKey());
      _value = VariantRef(pool, slot->data());
    }
  }

  String key() const {
    return _key;
  }

  VariantRef value() const {
    return _value;
  }

 private:
  String _key;
  VariantRef _value;
};

class PairConst {
 public:
  PairConst(MemoryPool* pool, const VariantSlot* slot) {
    if (slot) {
      _key = String(pool->toCharPtr(slot->key()), !slot->ownsKey());
      _value = VariantConstRef(pool, slot->data());
    }
  }

  String key() const {
    return _key;
  }

  VariantConstRef value() const {
    return _value;
  }

 private:
  String _key;
  VariantConstRef _value;
};
}  // namespace ARDUINOJSON_NAMESPACE
