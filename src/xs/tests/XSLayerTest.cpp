#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "XSLayer.h"

TEST_CASE("XSLayer")
{
    SECTION("Size")
    {
        unsigned int x = 50;
        unsigned int y = 60;
        XSLayer l(x, y);

        CHECK_EQUAL(x, l.getX());
        CHECK_EQUAL(y, l.getY());
        x = 12;
        y = 34;
        l.Resize(x, y);
        CHECK_EQUAL(x, l.getX());
        CHECK_EQUAL(y, l.getY());
        // TODO:  Check that existing stitches are kept during resize
        XSLayer l2(l);
        CHECK_EQUAL(x, l2.getX());
        CHECK_EQUAL(y, l2.getY());
    }

    SECTION("SetStitch")
    {
    }
}
