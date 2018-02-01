/*
 * TestCSV.cpp
 *
 * Created on: 01 Feb 2018
 *     Author: Fabian Meyer
 *    License: MIT
 */

#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include "CSV.hpp"

TEST_CASE("parse csv file", "CsvFile")
{
    std::istringstream ss("foo,true,1.0\nbar,false,200,\"Hello, World!\"");
    csv::CsvFile csvf(ss);

    REQUIRE(csvf.size() == 2);

    REQUIRE(csvf[0].size() == 3);
    REQUIRE(csvf[0][0].asString() == "foo");
    REQUIRE(csvf[0][1].asBool() == true);
    REQUIRE(csvf[0][2].asDouble() == 1.0);

    REQUIRE(csvf[1].size() == 4);
    REQUIRE(csvf[1][0].asString() == "bar");
    REQUIRE(csvf[1][1].asBool() == false);
    REQUIRE(csvf[1][2].asInt() == 200);
    REQUIRE(csvf[1][3].asString() == "Hello, World!");
}
