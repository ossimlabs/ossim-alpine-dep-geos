//
// Test Suite for geos::operation::overlayng::OverlayNG class.

#include <tut/tut.hpp>
#include <utility.h>

// geos
#include <geos/operation/overlayng/OverlayNG.h>

// std
#include <memory>

using namespace geos::geom;
using namespace geos::operation::overlayng;
using geos::io::WKTReader;
using geos::io::WKTWriter;

namespace tut {
//
// Test Group
//

// Common data used by all tests
struct test_overlayng_data {

    WKTReader r;
    WKTWriter w;

    void
    testOverlay(const std::string& a, const std::string& b, const std::string& expected, int opCode, double scaleFactor)
    {
        PrecisionModel pm(scaleFactor);
        std::unique_ptr<Geometry> geom_a = r.read(a);
        std::unique_ptr<Geometry> geom_b = r.read(b);
        std::unique_ptr<Geometry> geom_expected = r.read(expected);
        std::unique_ptr<Geometry> geom_result = OverlayNG::overlay(geom_a.get(), geom_b.get(), opCode, &pm);
        // std::string wkt_result = w.write(geom_result.get());
        // std::cout << std::endl << wkt_result << std::endl;
        ensure_equals_geometry(geom_expected.get(), geom_result.get());
    }

    void
    testOverlayNoOpt(const std::string& a, const std::string& b, const std::string& expected, int opCode, double scaleFactor)
    {
        PrecisionModel pm(scaleFactor);
        std::unique_ptr<Geometry> geom_a = r.read(a);
        std::unique_ptr<Geometry> geom_b = r.read(b);
        std::unique_ptr<Geometry> geom_expected = r.read(expected);
        OverlayNG ov(geom_a.get(), geom_b.get(), &pm, opCode);
        ov.setOptimized(true);
        std::unique_ptr<Geometry> geom_result = ov.getResult();
        ensure_equals_geometry(geom_expected.get(), geom_result.get());
    }

};

typedef test_group<test_overlayng_data> group;
typedef group::object object;

group test_overlayng_group("geos::operation::overlayng::OverlayNG");

//
// Test Cases
//

//  Square overlapping square
template<>
template<>
void object::test<1> ()
{
    std::string a = "POLYGON((1000 1000, 2000 1000, 2000 2000, 1000 2000, 1000 1000))";
    std::string b = "POLYGON((1500 1500, 2500 1500, 2500 2500, 1500 2500, 1500 1500))";
    std::string exp = "POLYGON((1500 2000,2000 2000,2000 1500,1500 1500,1500 2000))";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testEmptyGCBothIntersection
template<>
template<>
void object::test<2> ()
{
    std::string a = "GEOMETRYCOLLECTION EMPTY";
    std::string b = "GEOMETRYCOLLECTION EMPTY";
    std::string exp = "GEOMETRYCOLLECTION EMPTY";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testEmptyAPolygonIntersection
template<>
template<>
void object::test<3> ()
{
    std::string a = "POLYGON EMPTY";
    std::string b = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string exp = "POLYGON EMPTY";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testEmptyBIntersection
template<>
template<>
void object::test<4> ()
{
    std::string a = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string b = "POLYGON EMPTY";
    std::string exp = "POLYGON EMPTY";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testEmptyABIntersection
template<>
template<>
void object::test<5> ()
{
    std::string a = "POLYGON EMPTY";
    std::string b = "POLYGON EMPTY";
    std::string exp = "POLYGON EMPTY";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testEmptyADifference
template<>
template<>
void object::test<6> ()
{
    std::string a = "POLYGON EMPTY";
    std::string b = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string exp = "POLYGON EMPTY";
    testOverlay(a, b, exp, OverlayNG::DIFFERENCE, 1);
}

//  testEmptyAUnion
template<>
template<>
void object::test<7> ()
{
    std::string a = "POLYGON EMPTY";
    std::string b = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string exp = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

//  testEmptyASymDifference
template<>
template<>
void object::test<8> ()
{
    std::string a = "POLYGON EMPTY";
    std::string b = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string exp = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    testOverlay(a, b, exp, OverlayNG::SYMDIFFERENCE, 1);
}

//  testEmptyLinePolygonIntersection
template<>
template<>
void object::test<9> ()
{
    std::string a = "LINESTRING EMPTY";
    std::string b = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string exp = "LINESTRING EMPTY";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testEmptyLinePolygonDifference
template<>
template<>
void object::test<10> ()
{
    std::string a = "LINESTRING EMPTY";
    std::string b = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string exp = "LINESTRING EMPTY";
    testOverlay(a, b, exp, OverlayNG::DIFFERENCE, 1);
}

//  testEmptyPointPolygonIntersection
template<>
template<>
void object::test<11> ()
{
    std::string a = "POINT EMPTY";
    std::string b = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string exp = "POINT EMPTY";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testDisjointIntersection
template<>
template<>
void object::test<12> ()
{
    std::string a = "POLYGON ((60 90, 90 90, 90 60, 60 60, 60 90))";
    std::string b = "POLYGON ((200 300, 300 300, 300 200, 200 200, 200 300))";
    std::string exp = "POLYGON EMPTY";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testPolygoPolygonWithLineTouchIntersection
template<>
template<>
void object::test<13> ()
{
    std::string a = "POLYGON ((360 200, 220 200, 220 180, 300 180, 300 160, 300 140, 360 200))";
    std::string b = "MULTIPOLYGON (((280 180, 280 160, 300 160, 300 180, 280 180)), ((220 230, 240 230, 240 180, 220 180, 220 230)))";
    std::string exp = "POLYGON ((220 200, 240 200, 240 180, 220 180, 220 200))";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testBoxTriIntersection
template<>
template<>
void object::test<14> ()
{
    std::string a = "POLYGON ((0 6, 4 6, 4 2, 0 2, 0 6))";
    std::string b = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string exp = "POLYGON ((3 2, 1 2, 2 5, 3 2))";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testBoxTriUnion
template<>
template<>
void object::test<15> ()
{
    std::string a = "POLYGON ((0 6, 4 6, 4 2, 0 2, 0 6))";
    std::string b = "POLYGON ((1 0, 2 5, 3 0, 1 0))";
    std::string exp = "POLYGON ((0 6, 4 6, 4 2, 3 2, 3 0, 1 0, 1 2, 0 2, 0 6))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

//  test2spikesIntersection
template<>
template<>
void object::test<16> ()
{
    std::string a = "POLYGON ((0 100, 40 100, 40 0, 0 0, 0 100))";
    std::string b = "POLYGON ((70 80, 10 80, 60 50, 11 20, 69 11, 70 80))";
    std::string exp = "MULTIPOLYGON (((40 80, 40 62, 10 80, 40 80)), ((40 38, 40 16, 11 20, 40 38)))";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

// //  test2spikesUnion
template<>
template<>
void object::test<17> ()
{
    std::string a = "POLYGON ((0 100, 40 100, 40 0, 0 0, 0 100))";
    std::string b = "POLYGON ((70 80, 10 80, 60 50, 11 20, 69 11, 70 80))";
    std::string exp = "POLYGON ((0 100, 40 100, 40 80, 70 80, 69 11, 40 16, 40 0, 0 0, 0 100), (40 62, 40 38, 60 50, 40 62))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

//  testTriBoxIntersection
template<>
template<>
void object::test<18> ()
{
    std::string a = "POLYGON ((68 35, 35 42, 40 9, 68 35))";
    std::string b = "POLYGON ((20 60, 50 60, 50 30, 20 30, 20 60))";
    std::string exp = "POLYGON ((37 30, 35 42, 50 39, 50 30, 37 30))";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testNestedShellsIntersection
template<>
template<>
void object::test<19> ()
{
    std::string a = "POLYGON ((100 200, 200 200, 200 100, 100 100, 100 200))";
    std::string b = "POLYGON ((120 180, 180 180, 180 120, 120 120, 120 180))";
    std::string exp = "POLYGON ((120 180, 180 180, 180 120, 120 120, 120 180))";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

//  testNestedShellsUnion
template<>
template<>
void object::test<20> ()
{
    std::string a = "POLYGON ((100 200, 200 200, 200 100, 100 100, 100 200))";
    std::string b = "POLYGON ((120 180, 180 180, 180 120, 120 120, 120 180))";
    std::string exp = "POLYGON ((100 200, 200 200, 200 100, 100 100, 100 200))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

//  testATouchingNestedPolyUnion
template<>
template<>
void object::test<21> ()
{
    std::string a = "MULTIPOLYGON (((0 200, 200 200, 200 0, 0 0, 0 200), (50 50, 190 50, 50 200, 50 50)), ((60 100, 100 60, 50 50, 60 100)))";
    std::string b = "POLYGON ((135 176, 180 176, 180 130, 135 130, 135 176))";
    std::string exp = "MULTIPOLYGON (((0 0, 0 200, 50 200, 200 200, 200 0, 0 0), (50 50, 190 50, 50 200, 50 50)), ((50 50, 60 100, 100 60, 50 50)))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}


// testTouchingPolyDifference
template<>
template<>
void object::test<22> ()
{
    std::string a = "POLYGON ((200 200, 200 0, 0 0, 0 200, 200 200), (100 100, 50 100, 50 200, 100 100))";
    std::string b = "POLYGON ((150 100, 100 100, 150 200, 150 100))";
    std::string exp = "MULTIPOLYGON (((0 0, 0 200, 50 200, 50 100, 100 100, 150 100, 150 200, 200 200, 200 0, 0 0)), ((50 200, 150 200, 100 100, 50 200)))";
    testOverlay(a, b, exp, OverlayNG::DIFFERENCE, 1);
}

// testTouchingHoleUnion
template<>
template<>
void object::test<23> ()
{
    std::string a = "POLYGON ((100 300, 300 300, 300 100, 100 100, 100 300), (200 200, 150 200, 200 300, 200 200))";
    std::string b = "POLYGON ((130 160, 260 160, 260 120, 130 120, 130 160))";
    std::string exp = "POLYGON ((100 100, 100 300, 200 300, 300 300, 300 100, 100 100), (150 200, 200 200, 200 300, 150 200))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

// testTouchingMultiHoleUnion
template<>
template<>
void object::test<24> ()
{
    std::string a = "POLYGON ((100 300, 300 300, 300 100, 100 100, 100 300), (200 200, 150 200, 200 300, 200 200), (250 230, 216 236, 250 300, 250 230), (235 198, 300 200, 237 175, 235 198))";
    std::string b = "POLYGON ((130 160, 260 160, 260 120, 130 120, 130 160))";
    std::string exp = "POLYGON ((100 300, 200 300, 250 300, 300 300, 300 200, 300 100, 100 100, 100 300), (200 300, 150 200, 200 200, 200 300), (250 300, 216 236, 250 230, 250 300), (300 200, 235 198, 237 175, 300 200))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

// testBoxLineIntersection
template<>
template<>
void object::test<25> ()
{
    std::string a = "POLYGON ((100 200, 200 200, 200 100, 100 100, 100 200))";
    std::string b = "LINESTRING (50 150, 150 150)";
    std::string exp = "LINESTRING (100 150, 150 150)";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

// testBoxLineUnion
template<>
template<>
void object::test<26> ()
{
    std::string a = "POLYGON ((100 200, 200 200, 200 100, 100 100, 100 200))";
    std::string b = "LINESTRING (50 150, 150 150)";
    std::string exp = "GEOMETRYCOLLECTION (POLYGON ((100 200, 200 200, 200 100, 100 100, 100 150, 100 200)), LINESTRING (50 150, 100 150))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

// testAdjacentBoxesIntersection
template<>
template<>
void object::test<27> ()
{
    std::string a = "POLYGON ((100 200, 200 200, 200 100, 100 100, 100 200))";
    std::string b = "POLYGON ((300 200, 300 100, 200 100, 200 200, 300 200))";
    std::string exp = "LINESTRING (200 100, 200 200)";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

// testAdjacentBoxesUnion
template<>
template<>
void object::test<28> ()
{
    std::string a = "POLYGON ((100 200, 200 200, 200 100, 100 100, 100 200))";
    std::string b = "POLYGON ((300 200, 300 100, 200 100, 200 200, 300 200))";
    std::string exp = "POLYGON ((100 100, 100 200, 200 200, 300 200, 300 100, 200 100, 100 100))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

// testCollapseBoxGoreIntersection
template<>
template<>
void object::test<29> ()
{
    std::string a = "MULTIPOLYGON (((1 1, 5 1, 5 0, 1 0, 1 1)), ((1 1, 5 2, 5 4, 1 4, 1 1)))";
    std::string b = "POLYGON ((1 0, 1 2, 2 2, 2 0, 1 0))";
    std::string exp = "POLYGON ((2 0, 1 0, 1 1, 1 2, 2 2, 2 1, 2 0))";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

// testCollapseBoxGoreUnion
template<>
template<>
void object::test<30> ()
{
    std::string a = "MULTIPOLYGON (((1 1, 5 1, 5 0, 1 0, 1 1)), ((1 1, 5 2, 5 4, 1 4, 1 1)))";
    std::string b = "POLYGON ((1 0, 1 2, 2 2, 2 0, 1 0))";
    std::string exp = "POLYGON ((2 0, 1 0, 1 1, 1 2, 1 4, 5 4, 5 2, 2 1, 5 1, 5 0, 2 0))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

// testSnapBoxGoreIntersection
template<>
template<>
void object::test<31> ()
{
    std::string a = "MULTIPOLYGON (((1 1, 5 1, 5 0, 1 0, 1 1)), ((1 1, 5 2, 5 4, 1 4, 1 1)))";
    std::string b = "POLYGON ((4 3, 5 3, 5 0, 4 0, 4 3))";
    std::string exp = "MULTIPOLYGON (((4 3, 5 3, 5 2, 4 2, 4 3)), ((4 0, 4 1, 5 1, 5 0, 4 0)))";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

// testSnapBoxGoreUnion
template<>
template<>
void object::test<32> ()
{
    std::string a = "MULTIPOLYGON (((1 1, 5 1, 5 0, 1 0, 1 1)), ((1 1, 5 2, 5 4, 1 4, 1 1)))";
    std::string b = "POLYGON ((4 3, 5 3, 5 0, 4 0, 4 3))";
    std::string exp = "POLYGON ((1 1, 1 4, 5 4, 5 3, 5 2, 5 1, 5 0, 4 0, 1 0, 1 1), (1 1, 4 1, 4 2, 1 1))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

// testCollapseTriBoxIntersection
template<>
template<>
void object::test<33> ()
{
    std::string a = "POLYGON ((1 2, 1 1, 9 1, 1 2))";
    std::string b = "POLYGON ((9 2, 9 1, 8 1, 8 2, 9 2))";
    std::string exp = "POINT (8 1)";
    testOverlay(a, b, exp, OverlayNG::INTERSECTION, 1);
}

// testCollapseTriBoxUnion
template<>
template<>
void object::test<34> ()
{
    std::string a = "POLYGON ((1 2, 1 1, 9 1, 1 2))";
    std::string b = "POLYGON ((9 2, 9 1, 8 1, 8 2, 9 2))";
    std::string exp = "MULTIPOLYGON (((1 1, 1 2, 8 1, 1 1)), ((8 1, 8 2, 9 2, 9 1, 8 1)))";
    testOverlay(a, b, exp, OverlayNG::UNION, 1);
}

// testDisjointIntersectionNoOpt
template<>
template<>
void object::test<35> ()
{
    std::string a = "POLYGON ((60 90, 90 90, 90 60, 60 60, 60 90))";
    std::string b = "POLYGON ((200 300, 300 300, 300 200, 200 200, 200 300))";
    std::string exp = "POLYGON EMPTY";
    testOverlayNoOpt(a, b, exp, OverlayNG::INTERSECTION, 1);
}

// testMultiPolygonNarrowGap
template<>
template<>
void object::test<36> ()
{
    std::string a = "MULTIPOLYGON (((1 9, 5.7 9, 5.7 1, 1 1, 1 9)), ((9 9, 9 1, 6 1, 6 9, 9 9)))";
    std::string b = "POLYGON EMPTY";
    std::string exp = "POLYGON ((1 9, 6 9, 9 9, 9 1, 6 1, 1 1, 1 9))";
    testOverlayNoOpt(a, b, exp, OverlayNG::UNION, 1);
}

} // namespace tut
