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
        CsvValue(const std::string &value);
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
    };

    typedef std::vector<CsvValue> CsvRow;

    class CsvFile : public std::vector<CsvRow>
    {
    private:
        char valueSep_;
        char comment_;
        char esc_;
        size_t rowLen_;
    public:
        CsvFile(const char valueSep = ',', const char comment = '#');
        CsvFile(const std::string &fileName, const char valueSep = ',',
                const char comment = '#');
        CsvFile(std::istream &is, const char valueSep = ',', const char comment = '#');
        ~CsvFile();

        void setValueSep(const char sep);
        void setCommentChar(const char comment);
        void setProposedRowLen(const size_t len);

        void decode(std::istream &is);
        void decode(const std::string &content);
        void encode(std::ostream &os);
        std::string encode();

        void load(const std::string &fileName);
        void save(const std::string &fileName);
    };
}


#endif
