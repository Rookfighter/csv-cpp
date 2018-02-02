# CSV Cpp ![](https://travis-ci.org/Rookfighter/csv-cpp.svg?branch=master)

CSV Cpp is a simple and easy to use comma separated values (CSV) en- and decoder for C++.

## Install

You can either build a static library using the CMake building system by running

```sh
cd <path-to-repo>
mkdir build
cd build
cmake ..
make
```

or simply copy the source files into your project and compile them directly.

## Usage

CSV Cpp allows loading data from any ```std::istream```. Simply call
the ```decode()``` method or use the overloaded constructor.

```cpp
#include <CSV.hpp>

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
bool myBool = myCsv[0][0].asBool();
std::string myStr = myCsv[0][1].asString();
int myInt = myCsv[0][2].asInt();
unsigned int myUInt = myCsv[0][3].asUInt();
double myDouble = myCsv[0][4].asDouble();
float myFloat = myCsv[0][5].asFloat();
```

To create a CSV file with CSV Cpp add rows and values. Supported types are:

* bool
* std::string
* int
* unsigend int
* double
* float

The content of the CSV file can then be written to any std::ostream object
using the ```encode()``` method.

```cpp
#include <CSV.hpp>

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
