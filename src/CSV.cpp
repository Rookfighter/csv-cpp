/*
 * CSV.cpp
 *
 * Created on: 01 Feb 2018
 *     Author: Fabian Meyer
 *    License: MIT
 */

#include "CSV.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <iostream>

#define TO_UPPER(str) std::transform(str.begin(), str.end(),str.begin(), ::toupper)

namespace csv
{
    /************************************
     *          Helper Functions
     ************************************/

    static bool strToBool(const std::string &value)
    {
        std::string tmp(value);
        TO_UPPER(tmp);

        if(tmp == "TRUE")
            return true;
        else if(tmp == "FALSE")
            return false;
        else
            throw std::domain_error("field is not a bool");
    }

    static int strToInt(const std::string &value)
    {
        char *endptr;
        int result = strtol(value.c_str(), &endptr, 10);
        if(*endptr != '\0')
            throw std::domain_error("field is not an int");

        return result;
    }

    static double strToDouble(const std::string &value)
    {
        char *endptr;
        double result = strtod(value.c_str(), &endptr);
        if(*endptr != '\0')
            throw std::domain_error("field is not a double");

        return result;
    }

    /************************************
     *          CsvValue
     ************************************/

    CsvValue::CsvValue()
    {
    }

    CsvValue::CsvValue(const std::string &value)
        : value_(value)
    {

    }

    CsvValue::CsvValue(const CsvValue &csvVal)
        : value_(csvVal.value_)
    {
    }

    CsvValue::~CsvValue()
    {
    }

    const std::string &CsvValue::asString() const
    {
        return value_;
    }

    int CsvValue::asInt() const
    {
        return strToInt(value_);
    }

    unsigned int CsvValue::asUInt() const
    {
        return static_cast<unsigned int>(strToInt(value_));
    }

    double CsvValue::asDouble() const
    {
        return strToDouble(value_);
    }

    float CsvValue::asFloat() const
    {
        return static_cast<float>(strToDouble(value_));
    }

    bool CsvValue::asBool() const
    {
        return strToBool(value_);
    }

    CsvValue &CsvValue::operator=(const CsvValue &csvVal)
    {
        value_ = csvVal.value_;
        return *this;
    }

    CsvValue &CsvValue::operator=(const int value)
    {
        std::stringstream ss;
        ss << value;
        value_ = ss.str();
        return *this;
    }

    CsvValue &CsvValue::operator=(const unsigned int value)
    {
        std::stringstream ss;
        ss << value;
        value_ = ss.str();
        return *this;
    }

    CsvValue &CsvValue::operator=(const double value)
    {
        std::stringstream ss;
        ss << value;
        value_ = ss.str();
        return *this;
    }

    CsvValue &CsvValue::operator=(const float value)
    {
        std::stringstream ss;
        ss << value;
        value_ = ss.str();
        return *this;
    }

    CsvValue &CsvValue::operator=(const bool value)
    {
        if(value)
            value_ = "true";
        else
            value_ = "false";
        return *this;
    }

    CsvValue &CsvValue::operator=(const std::string &value)
    {
        value_ = value;
        return *this;
    }

    /************************************
     *          CsvFile
     ************************************/

    CsvFile::CsvFile(const char valueSep, const char comment)
        : valueSep_(valueSep), comment_(comment)
    {
    }

    CsvFile::CsvFile(const std::string &fileName, const char valueSep,
                     const char comment)
        : valueSep_(valueSep), comment_(comment)
    {
        load(fileName);
    }

    CsvFile::CsvFile(std::istream &is, const char valueSep, const char comment)
        : valueSep_(valueSep), comment_(comment)
    {
        decode(is);
    }

    CsvFile::~CsvFile()
    {
    }

    void CsvFile::setValueSep(const char sep)
    {
        valueSep_ = sep;
    }

    void CsvFile::setCommentChar(const char comment)
    {
        comment_ = comment;
    }

    void CsvFile::decode(std::istream &is)
    {

    }

    void CsvFile::decode(const std::string &content)
    {
        std::istringstream ss(content);
        decode(ss);
    }

    void CsvFile::load(const std::string &fileName)
    {
        std::ifstream is(fileName.c_str());
        decode(is);
    }

    void CsvFile::encode(std::ostream &os)
    {

    }

    std::string CsvFile::encode()
    {
        std::ostringstream ss;
        encode(ss);
        return ss.str();
    }

    void CsvFile::save(const std::string &fileName)
    {
        std::ofstream os(fileName.c_str());
        encode(os);
    }
}
