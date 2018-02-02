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

    static char escChar(const char c, const char esc)
    {
        if(c == esc)
            return esc;

        switch(c) {
            case 'v':
                return '\v';
            case 't':
                return '\t';
            case 'r':
                return '\r';
            case 'n':
                return '\n';
            case 'f':
                return '\f';
            case '\\':
                return '\\';
            default:
                return '\0';
        }
    }

    enum DecodeState {
        BeginVal = 0,
        EscVal,
        StdVal,
        EndVal
    };

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

    char CsvFile::DefaultSep = ',';
    char CsvFile::DefaultComment = '#';
    char CsvFile::DefaultEsc = '"';
    size_t CsvFile::DefaultRowLen = 16;

    CsvFile::CsvFile()
        : valueSep_(DefaultSep), comment_(DefaultComment), esc_(DefaultEsc), rowLen_(DefaultRowLen)
    {
    }

    CsvFile::CsvFile(const std::string &fileName)
        : CsvFile()
    {
        load(fileName);
    }

    CsvFile::CsvFile(std::istream &is)
        : CsvFile()
    {
        decode(is);
    }

    CsvFile::~CsvFile()
    {
    }

    void CsvFile::setSep(const char sep)
    {
        valueSep_ = sep;
    }

    void CsvFile::setComment(const char comment)
    {
        comment_ = comment;
    }

    void CsvFile::setRowLen(const size_t len)
    {
        rowLen_ = len;
    }

    void CsvFile::decodeRowNo(const std::string &line, const size_t lineNo, CsvRow &row)
    {
        // init row vector with proposed capacity
        row.clear();
        row.reserve(rowLen_);

        DecodeState state = BeginVal;
        // string stream used to build values read from the file
        std::stringstream ss;
        for(size_t i = 0; i <= line.size(); ++i)
        {
            switch(state) {
                // handle begin of a new value
                case BeginVal:
                    // check if value starts with escape character
                    if(line[i] == esc_)
                        state = EscVal;
                    else
                    {
                        ss << line[i];
                        state = StdVal;
                    }
                    break;
                // handle escaped values (wrapped in escape char)
                case EscVal:
                    // found ending escape char
                    if(line[i] == esc_)
                    {
                        if(i+1 < line.size() && line[i+1] != valueSep_)
                        {
                            std::stringstream es;
                            es << "l" << lineNo
                               << ": csv parsing failed, no separator or newline after escaped value";
                            throw std::logic_error(es.str());
                        }
                        state = EndVal;
                    }
                    else if(line[i] == '\\')
                    {
                        // check if there is a following character
                        if(i+1 == line.size())
                        {
                            std::stringstream es;
                            es << "l" << lineNo
                               << ": csv parsing failed, escape not finished";
                            throw std::logic_error(es.str());
                        }
                        // increment i and consume char after escape
                        ++i;
                        char c = escChar(line[i], esc_);
                        // check if c is a valid escape char
                        if(c == '\0')
                        {
                            std::stringstream es;
                            es << "l" << lineNo
                               << ": csv parsing failed, invalid escape sequence \\"
                               << line[i];
                            throw std::logic_error(es.str());
                        }

                        ss << c;
                    }
                    else
                        ss << line[i];
                    break;
                case StdVal:
                    ss << line[i];
                    if(i+1 == line.size() || line[i+1] == valueSep_)
                        state = EndVal;
                    break;
                case EndVal:
                    row.push_back(CsvValue(ss.str()));
                    ss.str(std::string()); // = std::stringstream();
                    state = BeginVal;
                    break;
            }
        }
    }

    void CsvFile::decodeRow(const std::string &line, CsvRow &row)
    {
        decodeRowNo(line, 0, row);
    }

    void CsvFile::decode(std::istream &is)
    {
        // count lines of file to preinit vector
        size_t lineCount = std::count(std::istreambuf_iterator<char>(is),
             std::istreambuf_iterator<char>(), '\n') + 1;

        // reset istream
        is.clear();
        is.seekg(0, std::ios::beg);

        // clear vector and reserve enough space
        clear();
        reserve(lineCount);

        // iterate through all lines
        size_t lineNo = 0;
        std::string line;

        while(!is.eof() && !is.fail()) {
            std::getline(is, line, '\n');
            ++lineNo;

            // skip if line is empty
            if(line.size() == 0)
                continue;
            // skip if line is a comment
            if(line[0] == comment_)
                continue;

            CsvRow row;
            decodeRowNo(line, lineNo, row);
            push_back(row);
        }
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
