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
  CompareResult result;

  explicit Comparer(T value) : rhs(value), result(COMPARE_RESULT_DIFFER) {}

  void visitString(const char *lhs) {
    int i = adaptString(rhs).compare(lhs);
    if (i < 0)
      result = COMPARE_RESULT_GREATER;
    else if (i > 0)
      result = COMPARE_RESULT_LESS;
    else
      result = COMPARE_RESULT_EQUAL;
  }

  void visitNull() {
    if (adaptString(rhs).isNull())
      result = COMPARE_RESULT_EQUAL;
  }

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
};

template <typename T>
struct Comparer<T, typename enable_if<is_integral<T>::value ||
                                      is_floating_point<T>::value>::type> {
  T rhs;
  CompareResult result;

  explicit Comparer(T value) : rhs(value), result(COMPARE_RESULT_DIFFER) {}

  void visitFloat(Float lhs) {
    Float diff = lhs - static_cast<Float>(rhs);
    if (diff < 0)
      result = COMPARE_RESULT_LESS;
    else if (diff > 0)
      result = COMPARE_RESULT_GREATER;
    else
      result = COMPARE_RESULT_EQUAL;
  }

  void visitNegativeInteger(UInt lhs) {
    if (static_cast<T>(lhs) + rhs < 0)
      result = COMPARE_RESULT_GREATER;
    else if (static_cast<T>(lhs) + rhs > 0)
      result = COMPARE_RESULT_LESS;
    else
      result = COMPARE_RESULT_EQUAL;
  }

  void visitPositiveInteger(UInt lhs) {
    if (static_cast<T>(lhs) < rhs)
      result = COMPARE_RESULT_LESS;
    else if (static_cast<T>(lhs) > rhs)
      result = COMPARE_RESULT_GREATER;
    else
      result = COMPARE_RESULT_EQUAL;
  }

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitBoolean(bool) {}
  void visitNull() {}
};

template <>
struct Comparer<bool, void> {
  bool rhs;
  CompareResult result;

  explicit Comparer(bool value) : rhs(value), result(COMPARE_RESULT_DIFFER) {}

  void visitBoolean(bool lhs) {
    if (lhs < rhs)
      result = COMPARE_RESULT_LESS;
    else if (lhs > rhs)
      result = COMPARE_RESULT_GREATER;
    else
      result = COMPARE_RESULT_EQUAL;
  }

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitNull() {}
};

struct NullComparer {
  CompareResult result;

  explicit NullComparer() : result(COMPARE_RESULT_DIFFER) {}

  void visitNull() {
    result = COMPARE_RESULT_EQUAL;
  }

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
};

#if ARDUINOJSON_HAS_NULLPTR
template <>
struct Comparer<decltype(nullptr), void> : NullComparer {
  explicit Comparer(decltype(nullptr)) : NullComparer() {}
};
#endif

struct ArrayComparer {
  const CollectionData *_rhs;
  CompareResult result;

  explicit ArrayComparer(const CollectionData &rhs)
      : _rhs(&rhs), result(COMPARE_RESULT_DIFFER) {}

  void visitArray(const CollectionData &lhs) {
    if (lhs.equalsArray(*_rhs))
      result = COMPARE_RESULT_EQUAL;
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
  CompareResult result;

  explicit NegativeIntegerComparer(UInt rhs)
      : _rhs(rhs), result(COMPARE_RESULT_DIFFER) {}

  void visitFloat(Float lhs) {
    if (lhs < static_cast<Float>(_rhs))
      result = COMPARE_RESULT_LESS;
    else if (lhs > static_cast<Float>(_rhs))
      result = COMPARE_RESULT_GREATER;
    else
      result = COMPARE_RESULT_EQUAL;
  }

  void visitNegativeInteger(UInt lhs) {
    if (lhs < _rhs)
      result = COMPARE_RESULT_LESS;
    else if (lhs > _rhs)
      result = COMPARE_RESULT_GREATER;
    else
      result = COMPARE_RESULT_EQUAL;
  }

  void visitPositiveInteger(UInt) {
    result = COMPARE_RESULT_LESS;
  }

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitString(const char *) {}
  void visitRawJson(const char *, size_t) {}
  void visitBoolean(bool) {}
  void visitNull() {}
};

struct ObjectComparer {
  const CollectionData *_rhs;
  CompareResult result;

  explicit ObjectComparer(const CollectionData &rhs)
      : _rhs(&rhs), result(COMPARE_RESULT_DIFFER) {}

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &lhs) {
    if (lhs.equalsObject(*_rhs))
      result = COMPARE_RESULT_EQUAL;
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
  CompareResult result;

  explicit RawComparer(const char *rhsData, size_t rhsSize)
      : _rhsData(rhsData), _rhsSize(rhsSize), result(COMPARE_RESULT_DIFFER) {}

  void visitRawJson(const char *lhsData, size_t lhsSize) {
    size_t size = _rhsSize < lhsSize ? _rhsSize : lhsSize;
    int n = memcmp(lhsData, _rhsData, size);
    if (n < 0)
      result = COMPARE_RESULT_LESS;
    else if (n > 0)
      result = COMPARE_RESULT_GREATER;
    else
      result = COMPARE_RESULT_EQUAL;
  }

  void visitArray(const CollectionData &) {}
  void visitObject(const CollectionData &) {}
  void visitFloat(Float) {}
  void visitString(const char *) {}
  void visitNegativeInteger(UInt) {}
  void visitPositiveInteger(UInt) {}
  void visitBoolean(bool) {}
  void visitNull() {}
};

template <typename T>
struct Comparer<T, typename enable_if<IsVisitable<T>::value>::type> {
  T rhs;
  CompareResult result;

  explicit Comparer(T value) : rhs(value), result(COMPARE_RESULT_DIFFER) {}

  void visitArray(const CollectionData &lhs) {
    ArrayComparer comparer(lhs);
    accept(comparer);
  }

  void visitObject(const CollectionData &lhs) {
    ObjectComparer comparer(lhs);
    accept(comparer);
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
    switch (comparer.result) {
      case COMPARE_RESULT_GREATER:
        result = COMPARE_RESULT_LESS;
        break;
      case COMPARE_RESULT_LESS:
        result = COMPARE_RESULT_GREATER;
        break;
      default:
        result = comparer.result;
        break;
    }
  }
};

template <typename T1, typename T2>
CompareResult compare(const T1 &lhs, const T2 &rhs) {
  Comparer<T2> comparer(rhs);
  lhs.accept(comparer);
  return comparer.result;
}

inline int variantCompare(const VariantData *a, const VariantData *b) {
  return compare(VariantConstRef(a), VariantConstRef(b));
}

}  // namespace ARDUINOJSON_NAMESPACE
