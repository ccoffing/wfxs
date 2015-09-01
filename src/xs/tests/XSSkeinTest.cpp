#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "XSSkein.h"

TEST_CASE("XSSkein") {

    SECTION("Constructor") {
        XSColor c(0xff, 0, 0);

        XSSkein s(FM_ANCHOR, FPL_LINEN, "123ABC", "Linen XYZ", c, false, FB_NONE);

        CHECK(FM_ANCHOR == s.m_maker);
        CHECK(FPL_LINEN == s.m_productLine);
        CHECK("123ABC" == s.m_id.c_str());
        CHECK("Linen XYZ" == s.m_description.c_str());
        CHECK(1U == s.m_colors.size());
        CHECK(c == s.m_colors[0]);
        CHECK(false == s.m_discontinued);
        CHECK(FB_NONE == s.m_blend);
    }

    SECTION("Unserialize") {
        XSSkein skein;

        // 8: maker
        // 8: number
        // cstr: floss-id
        // cstr: floss-desc
        // 8: flags
        // 8: num-colors
        // num-colors:  8: r
        //              8: g
        //              8: b
        std::string raw("\x02\x02 \x01", 4);
        std::string buf(raw);
        // std::istream is(buf);
        // skein.Unserialize(is);

        // TODO
    }

    SECTION("Serialize") {
        XSSkein skeinA;

        skeinA.m_maker = FM_JPCOATS;
        skeinA.m_productLine = FPL_RIBBON8;
        skeinA.m_id = "CC123";
        skeinA.m_description = "Candy Cane";
        skeinA.m_colors.push_back(XSColor(0xff, 0x01, 0x02));
        skeinA.m_colors.push_back(XSColor(0xfe, 0xfd, 0xfc));
        skeinA.m_blend = FB_BANDED;


        // TODO:  programmatically create skeinA; serialize; unserialize into
        // skeinB; assert ==.
    }
}
