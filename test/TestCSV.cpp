/*
 * TestCSV.cpp
 *
 * Created on: 01 Feb 2018
 *     Author: Fabian Meyer
 *    License: MIT
 */

#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <iostream>
#include "CSV.hpp"

TEST_CASE("parse csv file", "CsvFile")
{
    csv::CsvFile csvf;
    csvf.decode("foo,true,1.0\nbar,false,200,\"Hello, World!\"");

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

TEST_CASE("parse row", "CsvFile")
{
    std::string str = "foo,true,1.0,10";
    csv::CsvFile csvf;
    csv::CsvRow row;

    csvf.decodeRow(str, row);

    REQUIRE(row.size() == 4);

    REQUIRE(row[0].asString() == "foo");
    REQUIRE(row[1].asBool() == true);
    REQUIRE(row[2].asDouble() == 1.0);
    REQUIRE(row[3].asInt() == 10);
}

TEST_CASE("parse row escaped", "CsvFile")
{
    std::string str = "\"foo\",\"true\",1.0,\"10\",\"Hello, World\\t!\"";
    csv::CsvFile csvf;
    csv::CsvRow row;

    csvf.decodeRow(str, row);

    REQUIRE(row.size() == 5);

    REQUIRE(row[0].asString() == "foo");
    REQUIRE(row[1].asBool() == true);
    REQUIRE(row[2].asDouble() == 1.0);
    REQUIRE(row[3].asInt() == 10);
    REQUIRE(row[4].asString() == "Hello, World\t!");
}

TEST_CASE("add values", "CsvFile")
{
    csv::CsvFile csvf;

    csvf.push_back({"foo", true, 1.0});
    csvf.push_back({"bar", false, 200, 2.1f});

    REQUIRE(csvf.size() == 2);

    REQUIRE(csvf[0].size() == 3);
    REQUIRE(csvf[0][0].asString() == "foo");
    REQUIRE(csvf[0][1].asBool() == true);
    REQUIRE(csvf[0][2].asDouble() == 1.0);

    REQUIRE(csvf[1].size() == 4);
    //REQUIRE(csvf[1][0].asString() == "bar");
    REQUIRE(csvf[1][1].asBool() == false);
    REQUIRE(csvf[1][2].asInt() == 200);
    REQUIRE(csvf[1][3].asFloat() == 2.1f);
}
