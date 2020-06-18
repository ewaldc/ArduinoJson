// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Configuration.hpp>
#include <ArduinoJson/Misc/Visitable.hpp>
#include <ArduinoJson/Numbers/Float.hpp>
#include <ArduinoJson/Numbers/Integer.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Strings/IsString.hpp>

namespace ARDUINOJSON_NAMESPACE {

class CollectionData;

template <typename T, typename Enable = void>
struct Comparer;

template <typename T>
struct Comparer<T, typename enable_if<IsString<T>::value>::type> {
  T rhs;
  int result;

  explicit Comparer(T value) : rhs(value), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *lhs) {
    result = -adaptString(rhs).compare(lhs);
  }
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {
    result = adaptString(rhs).compare(NULL);
  }
};

template <typename T>
typename enable_if<is_signed<T>::value, int>::type sign(const T &value) {
  return value < 0 ? -1 : value > 0 ? 1 : 0;
}

template <typename T>
typename enable_if<is_unsigned<T>::value, int>::type sign(const T &value) {
  return value > 0 ? 1 : 0;
}

template <typename T>
struct Comparer<T, typename enable_if<is_integral<T>::value ||
                                      is_floating_point<T>::value>::type> {
  T rhs;
  int result;

  explicit Comparer(T value) : rhs(value), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float lhs) {
    result = sign(lhs - static_cast<Float>(rhs));
  }
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt lhs) {
    result = -sign(static_cast<T>(lhs) + rhs);
  }
  void visitPositiveInteger(UInt lhs) {
    result = static_cast<T>(lhs) < rhs ? -1 : static_cast<T>(lhs) > rhs ? 1 : 0;
  }
  void visitBoolean(bool) {}
  void visitNull() {}
};

template <>
struct Comparer<bool, void> {
  bool rhs;
  int result;

  explicit Comparer(bool value) : rhs(value), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool lhs) {
    result = static_cast<int>(lhs - rhs);
  }
  void visitNull() {}
};

struct NullComparer {
  int result;

  explicit NullComparer() : result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {
    result = 0;
  }
};

#if ARDUINOJSON_HAS_NULLPTR
template <>
struct Comparer<decltype(nullptr), void> : NullComparer {
  explicit Comparer(decltype(nullptr)) : NullComparer() {}
};
#endif

struct ArrayComparer {
  const CollectionData *_rhs;
  int result;

  explicit ArrayComparer(const CollectionData &rhs) : _rhs(&rhs), result(1) {}

  void visitArray(const CollectionData &lhs) {
    result = lhs.equalsArray(*_rhs) ? 0 : 1;
  }

  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {}
};

struct NegativeIntegerComparer {
  UInt _rhs;
  int result;

  explicit NegativeIntegerComparer(UInt rhs) : _rhs(rhs), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float lhs) {
    result = sign(lhs + static_cast<Float>(_rhs));
  }
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt lhs) {
    result = lhs < _rhs ? -1 : lhs > _rhs ? 1 : 0;
  }
  void visitPositiveInteger(UInt) {
    result = 1;
  }
  void visitBoolean(bool) {}
  void visitNull() {}
};

struct ObjectComparer {
  const CollectionData *_rhs;
  int result;

  explicit ObjectComparer(const CollectionData &rhs) : _rhs(&rhs), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &lhs) {
    result = lhs.equalsObject(*_rhs) ? 0 : 1;
  }
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {}
};

struct RawComparer {
  const char *_rhsData;
  size_t _rhsSize;
  int result;

  explicit RawComparer(const char *rhsData, size_t rhsSize)
      : _rhsData(rhsData), _rhsSize(rhsSize), result(1) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *lhsData, size_t lhsSize) {
    size_t size = _rhsSize < lhsSize ? _rhsSize : lhsSize;
    result = memcmp(lhsData, _rhsData, size);
  }
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {}
};

template <typename T>
struct Comparer<T, typename enable_if<IsVisitable<T>::value>::type> {
  T rhs;
  int result;

  explicit Comparer(T value) : rhs(value), result(1) {}

  void visitArray(const CollectionData &lhs) {
    ArrayComparer comparer(lhs);
    rhs.accept(comparer);
    result = -comparer.result;
  }

  void visitObject(const CollectionData &lhs) {
    ObjectComparer comparer(lhs);
    rhs.accept(comparer);
    result = -comparer.result;
  }

  void visitFloat(Float lhs) {
    Comparer<Float> comparer(lhs);
    accept(comparer);
  }

  void visitString(const char *lhs) {
    Comparer<const char *> comparer(lhs);
    accept(comparer);
  }

  void visitRawJson(const char *lhsData, size_t lhsSize) {
    RawComparer comparer(lhsData, lhsSize);
    accept(comparer);
  }

  void visitNegativeInteger(UInt lhs) {
    NegativeIntegerComparer comparer(lhs);
    accept(comparer);
  }

  void visitPositiveInteger(UInt lhs) {
    Comparer<UInt> comparer(lhs);
    accept(comparer);
  }

  void visitBoolean(bool lhs) {
    Comparer<bool> comparer(lhs);
    accept(comparer);
  }

  void visitNull() {
    NullComparer comparer;
    accept(comparer);
  }

 private:
  template <typename TComparer>
  void accept(TComparer &comparer) {
    rhs.accept(comparer);
    result = -comparer.result;
  }
};

template <typename TData>
template <typename T>
int VariantRefBase<TData>::compare(const T &rhs) const {
  Comparer<T> comparer(rhs);
  variantAccept(_data, comparer);
  return comparer.result;
}

inline int variantCompare(const VariantData *a, const VariantData *b) {
  return VariantConstRef(a).compare(VariantConstRef(b));
}

}  // namespace ARDUINOJSON_NAMESPACE
