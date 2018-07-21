/*
 * tes_csvfile.cpp
 *
 * Created on: 01 Feb 2018
 *     Author: Fabian Meyer
 *    License: MIT
 */

#include <catch.hpp>
#include "csvcpp.h"

TEST_CASE("parse csv file", "CsvFile")
{
    csv::CsvFile csvf;
    csvf.decode("foo,true,1.0\nbar,false,200,\"Hello, World!\"");

    REQUIRE(csvf.size() == 2);

    REQUIRE(csvf[0].size() == 3);
    REQUIRE(csvf[0][0].as<std::string>() == "foo");
    REQUIRE(csvf[0][1].as<bool>() == true);
    REQUIRE(csvf[0][2].as<double>() == 1.0);

    REQUIRE(csvf[1].size() == 4);
    REQUIRE(csvf[1][0].as<std::string>() == "bar");
    REQUIRE(csvf[1][1].as<bool>() == false);
    REQUIRE(csvf[1][2].as<int>() == 200);
    REQUIRE(csvf[1][3].as<std::string>() == "Hello, World!");
}

TEST_CASE("parse row", "CsvFile")
{
    std::string str = "foo,true,1.0,10";
    csv::CsvFile csvf;
    csv::CsvRow row;

    csvf.decodeRow(str, row);

    REQUIRE(row.size() == 4);

    REQUIRE(row[0].as<std::string>() == "foo");
    REQUIRE(row[1].as<bool>() == true);
    REQUIRE(row[2].as<double>() == 1.0);
    REQUIRE(row[3].as<int>() == 10);
}

TEST_CASE("parse row escaped", "CsvFile")
{
    std::string str = "\"foo\",\"true\",1.0,\"10\",\"Hello, World\\t!\"";
    csv::CsvFile csvf;
    csv::CsvRow row;

    csvf.decodeRow(str, row);

    REQUIRE(row.size() == 5);

    REQUIRE(row[0].as<std::string>() == "foo");
    REQUIRE(row[1].as<bool>() == true);
    REQUIRE(row[2].as<double>() == 1.0);
    REQUIRE(row[3].as<int>() == 10);
    REQUIRE(row[4].as<std::string>() == "Hello, World\t!");
}

TEST_CASE("parse row single char values", "CsvFile")
{
    std::string str = "1,2,\"3\"";
    csv::CsvFile csvf;
    csv::CsvRow row;

    csvf.decodeRow(str, row);

    REQUIRE(row.size() == 3);

    REQUIRE(row[0].as<int>() == 1);
    REQUIRE(row[1].as<int>() == 2);
    REQUIRE(row[2].as<int>() == 3);
}

TEST_CASE("add values", "CsvFile")
{
    csv::CsvFile csvf;

    csvf.push_back({"foo", true, 1.0});
    csvf.push_back({"bar", false, 200, 2.1f});

    REQUIRE(csvf.size() == 2);

    REQUIRE(csvf[0].size() == 3);
    REQUIRE(csvf[0][0].as<std::string>() == "foo");
    REQUIRE(csvf[0][1].as<bool>() == true);
    REQUIRE(csvf[0][2].as<double>() == 1.0);

    REQUIRE(csvf[1].size() == 4);
    //REQUIRE(csvf[1][0].asString() == "bar");
    REQUIRE(csvf[1][1].as<bool>() == false);
    REQUIRE(csvf[1][2].as<int>() == 200);
    REQUIRE(csvf[1][3].as<float>() == 2.1f);
}

TEST_CASE("encode without escape", "CsvFile")
{
    csv::CsvFile csvf;
    std::string s1 = "foo,true,1\nbar,false,200,2.1\n";

    csvf.push_back({"foo", true, 1.0});
    csvf.push_back({"bar", false, 200, 2.1f});

    REQUIRE(csvf.encode() == s1);
}

TEST_CASE("encode with escape", "CsvFile")
{
    csv::CsvFile csvf;
    std::string s1 = "\"f,oo\",\"tr\\tu\\\"e\",1\n\"b\\nar\",false,200,2.1\n";

    csvf.push_back({"f,oo", "tr\tu\"e", 1.0});
    csvf.push_back({"b\nar", false, 200, 2.1f});

    REQUIRE(csvf.encode() == s1);
}
