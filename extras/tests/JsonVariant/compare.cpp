// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#include <ArduinoJson.h>
#include <catch.hpp>

template <typename T>
void checkEquals(T a, T b) {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();
  variant.set(a);

  REQUIRE(b == variant);
  REQUIRE(variant == b);
  REQUIRE(b <= variant);
  REQUIRE(variant <= b);
  REQUIRE(b >= variant);
  REQUIRE(variant >= b);

  REQUIRE_FALSE(b != variant);
  REQUIRE_FALSE(variant != b);
  REQUIRE_FALSE(b > variant);
  REQUIRE_FALSE(variant > b);
  REQUIRE_FALSE(b < variant);
  REQUIRE_FALSE(variant < b);
}

template <typename T>
void checkGreater(T a, T b) {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();
  variant.set(a);

  REQUIRE(variant > b);
  REQUIRE(b < variant);
  REQUIRE(variant != b);
  REQUIRE(b != variant);

  REQUIRE_FALSE(variant < b);
  REQUIRE_FALSE(b > variant);
  REQUIRE_FALSE(variant == b);
  REQUIRE_FALSE(b == variant);
}

template <typename T>
void checkLower(T a, T b) {
  DynamicJsonDocument doc(4096);
  JsonVariant variant = doc.to<JsonVariant>();
  variant.set(a);

  REQUIRE(variant < b);
  REQUIRE(b > variant);
  REQUIRE(variant != b);
  REQUIRE(b != variant);

  REQUIRE_FALSE(variant > b);
  REQUIRE_FALSE(b < variant);
  REQUIRE_FALSE(variant == b);
  REQUIRE_FALSE(b == variant);
}

template <typename T>
void checkComparisons(T low, T mid, T high) {
  checkEquals(mid, mid);
  checkGreater(mid, low);
  checkLower(mid, high);
}

TEST_CASE("JsonVariant comparisons") {
  static const char* null = 0;

  SECTION("Double") {
    checkComparisons<double>(123.44, 123.45, 123.46);
  }

  SECTION("Float") {
    checkComparisons<float>(123.44f, 123.45f, 123.46f);
  }

  SECTION("SChar") {
    checkComparisons<signed char>(122, 123, 124);
  }

  SECTION("SInt") {
    checkComparisons<signed int>(122, 123, 124);
  }

  SECTION("SLong") {
    checkComparisons<signed long>(122L, 123L, 124L);
  }

  SECTION("SShort") {
    checkComparisons<signed short>(122, 123, 124);
  }

  SECTION("UChar") {
    checkComparisons<unsigned char>(122, 123, 124);
  }

  SECTION("UInt") {
    checkComparisons<unsigned int>(122, 123, 124);
  }

  SECTION("ULong") {
    checkComparisons<unsigned long>(122L, 123L, 124L);
  }

  SECTION("UShort") {
    checkComparisons<unsigned short>(122, 123, 124);
  }

  SECTION("null") {
    DynamicJsonDocument doc(4096);
    JsonVariant variant = doc.to<JsonVariant>();
    variant.set(null);

    REQUIRE(variant == variant);
    REQUIRE_FALSE(variant != variant);

    REQUIRE(variant == null);
    REQUIRE_FALSE(variant != null);

    REQUIRE(variant != "null");
    REQUIRE_FALSE(variant == "null");
  }

  SECTION("StringLiteral") {
    DynamicJsonDocument doc(4096);
    deserializeJson(doc, "\"hello\"");
    JsonVariant variant = doc.as<JsonVariant>();

    REQUIRE(variant == variant);
    REQUIRE_FALSE(variant != variant);

    REQUIRE(variant == "hello");
    REQUIRE_FALSE(variant != "hello");

    REQUIRE(variant != "world");
    REQUIRE_FALSE(variant == "world");

    REQUIRE(variant != null);
    REQUIRE_FALSE(variant == null);

    REQUIRE("hello" == variant);
    REQUIRE_FALSE("hello" != variant);

    REQUIRE("world" != variant);
    REQUIRE_FALSE("world" == variant);

    REQUIRE(null != variant);
    REQUIRE_FALSE(null == variant);
  }

  SECTION("String") {
    DynamicJsonDocument doc(4096);
    JsonVariant variant = doc.to<JsonVariant>();
    variant.set("hello");

    REQUIRE(variant == variant);
    REQUIRE_FALSE(variant != variant);

    REQUIRE(variant == std::string("hello"));
    REQUIRE_FALSE(variant != std::string("hello"));

    REQUIRE(variant != std::string("world"));
    REQUIRE_FALSE(variant == std::string("world"));

    REQUIRE(variant != null);
    REQUIRE_FALSE(variant == null);

    REQUIRE(std::string("hello") == variant);
    REQUIRE_FALSE(std::string("hello") != variant);

    REQUIRE(std::string("world") != variant);
    REQUIRE_FALSE(std::string("world") == variant);

    REQUIRE(null != variant);
    REQUIRE_FALSE(null == variant);
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("VLA equals") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonDocument doc(4096);
    JsonVariant variant = doc.to<JsonVariant>();
    variant.set("hello");

    REQUIRE((vla == variant));
    REQUIRE((variant == vla));
    REQUIRE_FALSE((vla != variant));
    REQUIRE_FALSE((variant != vla));
  }

  SECTION("VLA differs") {
    int i = 16;
    char vla[i];
    strcpy(vla, "hello");

    DynamicJsonDocument doc(4096);
    JsonVariant variant = doc.to<JsonVariant>();
    variant.set("world");

    REQUIRE((vla != variant));
    REQUIRE((variant != vla));
    REQUIRE_FALSE((vla == variant));
    REQUIRE_FALSE((variant == vla));
  }
#endif
}

TEST_CASE("Compare JsonVariant with JsonVariant") {
  StaticJsonDocument<256> doc;
  JsonVariant a = doc.addElement();
  JsonVariant b = doc.addElement();

  SECTION("null vs ...") {
    SECTION("null vs null") {
      REQUIRE(a == b);
      REQUIRE(a <= b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a > b);
    }

    SECTION("null vs 0") {
      b.set(0);

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("null vs -1") {
      b.set(-1);

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("null vs 'hello'") {
      b.set("hello");

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("null vs {}") {
      b.to<JsonObject>();

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("null vs []") {
      b.to<JsonArray>();

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }
  }

  SECTION("1 vs ...") {
    a.set(1);

    SECTION("1 vs 2") {
      b.set(2);

      REQUIRE(a != b);
      REQUIRE(a < b);
      REQUIRE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("1 vs 1.1") {
      b.set(1.1);

      REQUIRE(a != b);
      REQUIRE(a < b);
      REQUIRE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("1 vs -1") {
      b.set(-1);

      REQUIRE(a != b);
      REQUIRE(a > b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
    }

    SECTION("1 vs 'hello'") {
      b.set("hello");

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }
  }

  SECTION("1.1 vs ...") {
    a.set(1.1);

    SECTION("1.1 vs 2") {
      b.set(2);

      REQUIRE(a != b);
      REQUIRE(a < b);
      REQUIRE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("1.1 vs 1.0") {
      b.set(1.0);

      REQUIRE(a != b);
      REQUIRE(a > b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
    }

    SECTION("1.1 vs 1.1") {
      b.set(1.1);

      REQUIRE(a <= b);
      REQUIRE(a == b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a > b);
    }

    SECTION("1.1 vs 1.2") {
      b.set(1.2);

      REQUIRE(a != b);
      REQUIRE(a < b);
      REQUIRE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("1.1 vs -1") {
      b.set(-1);

      REQUIRE(a != b);
      REQUIRE(a > b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
    }

    SECTION("1.1 vs 'hello'") {
      b.set("hello");

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }
  }

  SECTION("-2 vs ...") {
    a.set(-2);

    SECTION("-2 vs null") {
      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("-2 vs 0") {
      b.set(0);

      REQUIRE(a != b);
      REQUIRE(a < b);
      REQUIRE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("-2 vs -1.9") {
      b.set(-1.9);

      REQUIRE(a != b);
      REQUIRE(a < b);
      REQUIRE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("-2 vs -2.0") {
      b.set(-2.0);

      REQUIRE(a <= b);
      REQUIRE(a == b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a > b);
    }

    SECTION("-2 vs -2.1") {
      b.set(-2.1);

      REQUIRE(a != b);
      REQUIRE(a > b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
    }

    SECTION("-2 vs -1") {
      b.set(-1);

      REQUIRE(a != b);
      REQUIRE(a < b);
      REQUIRE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("-2 vs -2") {
      b.set(-2);

      REQUIRE(a <= b);
      REQUIRE(a == b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a > b);
    }

    SECTION("-2 vs -3") {
      b.set(-3);

      REQUIRE(a != b);
      REQUIRE(a > b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
    }

    SECTION("-2 vs 'hello'") {
      b.set("hello");

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }
  }

  SECTION("[1] vs ...") {
    a.add(1);

    SECTION("[1] vs null") {
      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("[1] vs [1]") {
      b.add(1);

      REQUIRE(a <= b);
      REQUIRE(a == b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a > b);
    }

    SECTION("[1] vs [2]") {
      b.add(2);

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("[1] vs 'hello'") {
      b.set("hello");

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }
  }

  SECTION("{x:1} vs ...") {
    a["x"] = 1;

    SECTION("{x:1} vs null") {
      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("{x:1} vs {x:1}") {
      b["x"] = 1;

      REQUIRE(a <= b);
      REQUIRE(a == b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a > b);
    }

    SECTION("{x:1} vs {x:2}") {
      b["x"] = 2;

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("{x:1} vs {y:1}") {
      b["y"] = 2;

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("{x:1} vs 'hello'") {
      b.set("hello");

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }
  }

  SECTION("'hello' vs ...") {
    a.set("hello");

    SECTION("'hello' vs null") {
      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("'hello' vs 'hello'") {
      b.set("hello");

      REQUIRE(a <= b);
      REQUIRE(a == b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a > b);
    }

    SECTION("'hello' vs 'world'") {
      b.set("world");

      REQUIRE(a != b);
      REQUIRE(a < b);
      REQUIRE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("'hello' vs 'abcd'") {
      b.set("abcd");

      REQUIRE(a != b);
      REQUIRE(a > b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
    }

    SECTION("'hello' vs {}") {
      b.to<JsonObject>();

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }
  }

  SECTION("serialized('hello') vs ...") {
    a.set(serialized("hello"));

    SECTION("serialized('hello') vs null") {
      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("serialized('hello') vs 'hello'") {
      b.set("hello");

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("serialized('hello') vs serialized('hello')") {
      b.set(serialized("hello"));

      REQUIRE(a <= b);
      REQUIRE(a == b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a > b);
    }

    SECTION("serialized('hello') vs serialized('world')") {
      b.set(serialized("world"));

      REQUIRE(a != b);
      REQUIRE(a < b);
      REQUIRE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }

    SECTION("serialized('hello') vs serialized('abcd')") {
      b.set(serialized("abcd"));

      REQUIRE(a != b);
      REQUIRE(a > b);
      REQUIRE(a >= b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
    }

    SECTION("serialized('hello') vs {}") {
      b.to<JsonObject>();

      REQUIRE(a != b);
      REQUIRE_FALSE(a < b);
      REQUIRE_FALSE(a <= b);
      REQUIRE_FALSE(a == b);
      REQUIRE_FALSE(a > b);
      REQUIRE_FALSE(a >= b);
    }
  }
}
