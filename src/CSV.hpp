/*
 * CSV.hpp
 *
 * Created on: 01 Feb 2018
 *     Author: Fabian Meyer
 *    License: MIT
 */

#ifndef CSV_HPP_
#define CSV_HPP_

#include <string>
#include <vector>

namespace csv
{
    class CsvValue
    {
    private:
        std::string value_;
    public:
        CsvValue();
        CsvValue(const int value);
        CsvValue(const unsigned int value);
        CsvValue(const double value);
        CsvValue(const float value);
        CsvValue(const bool value);
        CsvValue(const std::string &value);
        CsvValue(const char *value);
        CsvValue(const CsvValue &csvVal);
        ~CsvValue();

        const std::string &asString() const;
        int asInt() const;
        unsigned int asUInt() const;
        double asDouble() const;
        float asFloat() const;
        bool asBool() const;

        CsvValue &operator=(const CsvValue &csvVal);
        CsvValue &operator=(const int value);
        CsvValue &operator=(const unsigned int value);
        CsvValue &operator=(const double value);
        CsvValue &operator=(const float value);
        CsvValue &operator=(const bool value);
        CsvValue &operator=(const std::string &value);
        CsvValue &operator=(const char *value);
    };

    typedef std::vector<CsvValue> CsvRow;

    class CsvFile : public std::vector<CsvRow>
    {
    private:
        char valueSep_;
        char comment_;
        char esc_;
        size_t rowLen_;

        void decodeRowNo(const std::string &line, const size_t lineNo, CsvRow &row);

    public:
        static char DefaultSep;
        static char DefaultComment;
        static char DefaultEsc;
        static size_t DefaultRowLen;

        CsvFile();
        CsvFile(const std::string &fileName);
        CsvFile(std::istream &is);
        ~CsvFile();

        void setSep(const char sep);
        void setComment(const char comment);
        void setRowLen(const size_t len);

        void decodeRow(const std::string &line, CsvRow &row);
        void decode(std::istream &is);
        void decode(const std::string &content);
        void encode(std::ostream &os);
        std::string encode();

        void load(const std::string &fileName);
        void save(const std::string &fileName);
    };
}


#endif
