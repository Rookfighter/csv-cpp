/*
 * csvcpp.h
 *
 * Created on: 01 Feb 2018
 *     Author: Fabian Meyer
 *    License: MIT
 */

#ifndef CSVCPP_H_
#define CSVCPP_H_

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <fstream>

namespace csv
{
    class CsvValue
    {
    private:
        std::string value_;
    public:
        CsvValue()
            : value_()
        {}

        CsvValue(const CsvValue &csvVal)
            : value_(csvVal.value_)
        {}

        CsvValue(const std::string &value)
            : value_(value)
        {}

        CsvValue(const char *value)
            : value_(value)
        {}

        CsvValue(const int value)
            : CsvValue()
        {
            *this = value;
        }

        CsvValue(const unsigned int value)
            : CsvValue()
        {
            *this = value;
        }

        CsvValue(const double value)
            : CsvValue()
        {
            *this = value;
        }

        CsvValue(const float value)
            : CsvValue()
        {
            *this = value;
        }

        CsvValue(const bool value)
            : CsvValue()
        {
            *this = value;
        }

        ~CsvValue()
        {}

        template<typename T>
        T as() const
        {
            return static_cast<T>(*this);
        }

        /**********************************************************************
         * Assignment Operators
         *********************************************************************/

        CsvValue &operator=(const CsvValue &csvVal)
        {
            value_ = csvVal.value_;
            return *this;
        }

        CsvValue &operator=(const std::string &value)
        {
            value_ = value;
            return *this;
        }

        CsvValue &operator=(const char *value)
        {
            value_ = std::string(value);
            return *this;
        }

        CsvValue &operator=(const int value)
        {
            std::stringstream ss;
            ss << value;
            value_ = ss.str();
            return *this;
        }

        CsvValue &operator=(const unsigned int value)
        {
            std::stringstream ss;
            ss << value;
            value_ = ss.str();
            return *this;
        }

        CsvValue &operator=(const double value)
        {
            std::stringstream ss;
            ss << value;
            value_ = ss.str();
            return *this;
        }

        CsvValue &operator=(const float value)
        {
            std::stringstream ss;
            ss << value;
            value_ = ss.str();
            return *this;
        }

        CsvValue &operator=(const bool value)
        {
            if(value)
                value_ = "true";
            else
                value_ = "false";
            return *this;
        }

        /**********************************************************************
         * Cast Operators
         *********************************************************************/

        explicit operator const char *() const
        {
            return value_.c_str();
        }

        explicit operator std::string() const
        {
            return value_;
        }

        explicit operator int() const
        {
            char *endptr;
            // check if decimal
            int result = std::strtol(value_.c_str(), &endptr, 10);
            if(*endptr == '\0')
                return result;
            // check if octal
            result = std::strtol(value_.c_str(), &endptr, 8);
            if(*endptr == '\0')
                return result;
            // check if hex
            result = std::strtol(value_.c_str(), &endptr, 16);
            if(*endptr == '\0')
                return result;

            throw std::invalid_argument("field is not an int");
        }

        explicit operator unsigned int() const
        {
            char *endptr;
            // check if decimal
            int result = std::strtoul(value_.c_str(), &endptr, 10);
            if(*endptr == '\0')
                return result;
            // check if octal
            result = std::strtoul(value_.c_str(), &endptr, 8);
            if(*endptr == '\0')
                return result;
            // check if hex
            result = std::strtoul(value_.c_str(), &endptr, 16);
            if(*endptr == '\0')
                return result;

            throw std::invalid_argument("field is not an unsigned int");
        }

        explicit operator float() const
        {
            return std::stof(value_);
        }

        explicit operator double() const
        {
            return std::stod(value_);
        }

        explicit operator bool() const
        {
            std::string str(value_);
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);

            if(str == "TRUE")
                return true;
            else if(str == "FALSE")
                return false;
            else
                throw std::invalid_argument("field is not a bool");
        }
    };

    typedef std::vector<CsvValue> CsvRow;

    class CsvFile : public std::vector<CsvRow>
    {
    private:
        char valueSep_;
        char comment_;
        char esc_;
        size_t rowLen_;

        enum DecodeState
        {
            BeginVal = 0,
            EscVal,
            StdVal,
            EndVal
        };

        /** Converts an escape sequence to its actual character.
         *  @param escape sequence
         *  @return character of the escape sequence if is valid escape
         *          sequence, else '\0' */
        char esc2char(const char c) const
        {
            if(c == esc_)
                return esc_;

            switch(c)
            {
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

        /** Converts a charater to its escpae sequence.
         *  @param c character to be escaped
         *  @return escape sequence if char is to be escaped, else '\0' */
        char char2esc(const char c) const
        {
            if(c == esc_)
                return esc_;

            switch(c)
            {
            case '\v':
                return 'v';
            case '\t':
                return 't';
            case '\r':
                return 'r';
            case '\n':
                return 'n';
            case '\f':
                return 'f';
            case '\\':
                return '\\';
            default:
                return '\0';
            }
        }

        /** Parses a single row in CSV format.
         *  The given row number is used for creating meaningful error messages.
         *  @param line the line string to be parsed
         *  @lineNo the line number within the file
         *  @row output row */
        void decodeRowNo(const std::string &line, const size_t lineNo, CsvRow &row)
        {
            // init row vector with proposed capacity
            row.clear();
            row.reserve(rowLen_);

            DecodeState state = BeginVal;
            // string stream used to build values read from the file
            std::stringstream ss;
            size_t i = 0;
            while(i <= line.size())
            {
                switch(state)
                {
                // handle begin of a new value
                case BeginVal:
                    // check if value starts with escape character
                    if(line[i] == esc_)
                        state = EscVal;
                    else
                    {
                        state = StdVal;
                        continue;
                    }
                    break;
                // handle escaped values (wrapped in escape char)
                case EscVal:
                    // found ending escape char
                    if(line[i] == esc_)
                    {
                        if(i + 1 < line.size() && line[i + 1] != valueSep_)
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
                        if(i + 1 == line.size())
                        {
                            std::stringstream es;
                            es << "l" << lineNo
                               << ": csv parsing failed, escape not finished";
                            throw std::logic_error(es.str());
                        }
                        // increment i and consume char after escape
                        ++i;
                        char c = esc2char(line[i]);
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
                    if(i + 1 == line.size() || line[i + 1] == valueSep_)
                        state = EndVal;
                    break;
                case EndVal:
                    row.push_back(CsvValue(ss.str()));
                    ss.str(std::string()); // = std::stringstream();
                    state = BeginVal;
                    break;
                }
                ++i;
            }
        }

    public:
        CsvFile()
            : valueSep_(','), comment_('#'), esc_('"'), rowLen_(128)
        {}

        CsvFile(const char valueSep, const char comment = '#', const char esc = '"')
            : valueSep_(valueSep), comment_(comment), esc_(esc), rowLen_(128)
        {}

        CsvFile(const std::string &fileName)
            : CsvFile()
        {
            load(fileName);
        }

        CsvFile(std::istream &is)
            : CsvFile()
        {
            decode(is);
        }

        ~CsvFile()
        {}

        void setSeparator(const char sep)
        {
            valueSep_ = sep;
        }

        void setCommentChar(const char comment)
        {
            comment_ = comment;
        }

        void proposeRowLength(const size_t len)
        {
            rowLen_ = len;
        }

        void decodeRow(const std::string &line, CsvRow &row)
        {
            decodeRowNo(line, 0, row);
        }

        void decode(std::istream &is)
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

            while(!is.eof() && !is.fail())
            {
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

        void decode(const std::string &content)
        {
            std::istringstream ss(content);
            decode(ss);
        }

        void load(const std::string &filename)
        {
            std::ifstream is(filename.c_str());
            decode(is);
        }

        void encode(std::ostream &os)
        {
            for(const CsvRow &row : *this)
            {
                for(const CsvValue &val : row)
                {
                    bool isEsc = false;
                    std::stringstream ssVal;
                    for(char c : val.as<std::string>())
                    {
                        char esc = char2esc(c);
                        if(esc != '\0')
                        {
                            isEsc = true;
                            ssVal << '\\' << esc;
                        }
                        else
                        {
                            if(c == valueSep_)
                                isEsc = true;
                            ssVal << c;
                        }
                    }

                    if(isEsc)
                        os << esc_;

                    os << ssVal.str();

                    if(isEsc)
                        os << esc_;

                    os << valueSep_;
                }

                long pos = os.tellp();
                os.seekp(pos - 1);

                os << '\n';
            }
        }

        std::string encode()
        {
            std::ostringstream ss;
            encode(ss);
            return ss.str();
        }

        void save(const std::string &fileName)
        {
            std::ofstream os(fileName.c_str());
            encode(os);
        }
    };
}


#endif
