# CSV-cpp
![](https://travis-ci.org/Rookfighter/csv-cpp.svg?branch=master)
![](https://ci.appveyor.com/api/projects/status/o55jm3mjodtat75g?svg=true)

CSV-cpp is a simple and easy to use, header-only comma separated values (CSV) en- and decoder for C++.

## Install

Install the headers using the CMake build system:

```sh
cd <path-to-repo>
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make install
```

or simply copy the header file into your project and include it directly.

## Usage

CSV-cpp allows loading data from any ```std::istream```. Simply call
the ```decode()``` method or use the overloaded constructor.

```cpp
#include <csvcpp.h>

int main()
{
	// create istream object "is" ...

	// use function
	csv::CsvFile myCsv;
	myCsv.decode(is);

	// or use constructor
	csv::CsvFile myCsv(is);
}
```

For convenience there is also a ```load()``` method that expects a file name
and allows to parse file contents directly.

Rows and columns parsed from the stream can be accessed using the index
operator ```[]```. The extracted value can be converted to various native
types.

```cpp
bool myBool = myCsv[0][0].as<bool>();
std::string myStr = myCsv[0][1].as<std::string>();
const char *myCStr = myCsv[0][2].as<const char *>();
int myInt = myCsv[0][3].as<int>();
unsigned int myUInt = myCsv[0][4].as<unsigned int>();
double myDouble = myCsv[0][5].as<double>();
float myFloat = myCsv[0][6].as<float>();
```

Natively supported types are:

* ```const char *```
* ```std::string```
* ```int```
* ```unsigned int```
* ```bool```
* ```float```
* ```double```

Custom type conversions can be added by implementing an explicit cast and assignment operator
for ```CsvValue```.

```cpp
#include <csvcpp.h>

struct MyDataType
{
	// ...
};

explicit operator MyDataType(const csv::CsvValue &csvVal)
{
	// convert std::string to your data type
	// return MyDataType::fromStr(csvVal.value_);
}

csv::CsvValue &operator=(csv::CsvValue &csvVal, const MyDataType &value)
{
	// conert your data type to a string
	// csvVal.value_ = value.toStr();
	// return csvVal;
}
```

Values can be assigned to csv values just by using the assignment operator.
The content of the csv file can then be written to any ```std::ostream``` object.

```cpp
#include <csvcpp.h>

int main()
{
	// create ostream object "os" ...

	csv::CsvFile myCsv;

	myCsv.push_back({1, "Hello world", true, 1.2});
	myCsv.push_back({"Foobar", 2.1f, -2});

	myCsv.encode(os);
}
```

For convenience there is also a ```save()``` function that expects a file name
and stores the values of the ```CsvFile``` object in that file.

Custom value separator and comment character can be set with their setter
functions or with one of the overloaded constructors.

```cpp
#include <csvcpp.h>

int main()
{
	char sep = '\t';
	char comment = ';';

	// use the constructor
	csv::CsvFile myCsv1(sep, comment);

	// use the setter
	csv::CsvFile myCsv2;
	myCsv2.setValueSeparator(sep);
	myCsv2.setCommentChar(comment);
}
```

If your CSV rows are very long you can set a proposed row length
(number of csv values per row) to make parsing more efficient.

```cpp
#include <csvcpp.h>

int main()
{
	size_t rowLen = 2056;

	// use the setter
	csv::CsvFile myCsv;
	myCsv.proposeRowLength(rowLen);
}
```
