// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Misc/Visitable.hpp>
#include <ArduinoJson/Polyfills/attributes.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantAs.hpp>

namespace ARDUINOJSON_NAMESPACE {

enum CompareResult {
  COMPARE_RESULT_DIFFER = 0,
  COMPARE_RESULT_EQUAL = 1,
  COMPARE_RESULT_GREATER = 2,
  COMPARE_RESULT_LESS = 4,

  COMPARE_RESULT_GREATER_OR_EQUAL = 3,
  COMPARE_RESULT_LESS_OR_EQUAL = 5
};

template <typename TVariant>
struct VariantOperators {
  // Returns the default value if the VariantRef is undefined of incompatible
  template <typename T>
  friend T operator|(const TVariant &variant, const T &defaultValue) {
    if (variant.template is<T>())
      return variant.template as<T>();
    else
      return defaultValue;
  }

  // Returns the default value if the VariantRef is undefined of incompatible
  // Special case for string: null is treated as undefined
  friend const char *operator|(const TVariant &variant,
                               const char *defaultValue) {
    const char *value = variant.template as<const char *>();
    return value ? value : defaultValue;
  }

  // value == TVariant
  template <typename T>
  friend bool operator==(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) == COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator==(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) == COMPARE_RESULT_EQUAL;
  }

  // TVariant == value
  template <typename T>
  friend bool operator==(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) == COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator==(
      TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) == COMPARE_RESULT_EQUAL;
  }

  // value != TVariant
  template <typename T>
  friend bool operator!=(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) != COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend bool operator!=(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) != COMPARE_RESULT_EQUAL;
  }

  // TVariant != value
  template <typename T>
  friend bool operator!=(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) != COMPARE_RESULT_EQUAL;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator!=(
      TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) != COMPARE_RESULT_EQUAL;
  }

  // value < TVariant
  template <typename T>
  friend bool operator<(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) == COMPARE_RESULT_GREATER;
  }
  template <typename T>
  friend bool operator<(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) == COMPARE_RESULT_GREATER;
  }

  // TVariant < value
  template <typename T>
  friend bool operator<(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) == COMPARE_RESULT_LESS;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator<(
      TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) == COMPARE_RESULT_LESS;
  }

  // value <= TVariant
  template <typename T>
  friend bool operator<=(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) & COMPARE_RESULT_GREATER_OR_EQUAL;
  }
  template <typename T>
  friend bool operator<=(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) & COMPARE_RESULT_GREATER_OR_EQUAL;
  }

  // TVariant <= value
  template <typename T>
  friend bool operator<=(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) & COMPARE_RESULT_LESS_OR_EQUAL;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator<=(
      TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) & COMPARE_RESULT_LESS_OR_EQUAL;
  }

  // value > TVariant
  template <typename T>
  friend bool operator>(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) == COMPARE_RESULT_LESS;
  }
  template <typename T>
  friend bool operator>(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) == COMPARE_RESULT_LESS;
  }

  // TVariant > value
  template <typename T>
  friend bool operator>(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) == COMPARE_RESULT_GREATER;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator>(
      TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) == COMPARE_RESULT_GREATER;
  }

  // value >= TVariant
  template <typename T>
  friend bool operator>=(T *lhs, TVariant rhs) {
    return rhs.compare(lhs) & COMPARE_RESULT_LESS_OR_EQUAL;
  }
  template <typename T>
  friend bool operator>=(const T &lhs, TVariant rhs) {
    return rhs.compare(lhs) & COMPARE_RESULT_LESS_OR_EQUAL;
  }

  // TVariant >= value
  template <typename T>
  friend bool operator>=(TVariant lhs, T *rhs) {
    return lhs.compare(rhs) & COMPARE_RESULT_GREATER_OR_EQUAL;
  }
  template <typename T>
  friend typename enable_if<!IsVisitable<T>::value, bool>::type operator>=(
      TVariant lhs, const T &rhs) {
    return lhs.compare(rhs) & COMPARE_RESULT_GREATER_OR_EQUAL;
  }
};
}  // namespace ARDUINOJSON_NAMESPACE
