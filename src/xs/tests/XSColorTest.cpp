#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "XSColor.h"

TEST_CASE("XSColor")
{
    SECTION("Constructor")
    {
        XSColor a(0x12, 0x89, 0xab, 0x8f);
        XSColor b(0x12, 0x89, 0xab, 0x8f);
        XSColor c(0x12, 0x89, 0xac, 0x8f);

        CHECK(a == b);
        CHECK_FALSE(a == c);
        CHECK(0x12 == a.red);
        CHECK(0x89 == a.green);
        CHECK(0xab == a.blue);
        CHECK(0x8f == a.alpha);
    }
}
