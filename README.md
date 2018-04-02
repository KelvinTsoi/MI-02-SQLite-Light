# Welcome to use SQLiteLight into your project

------

**SQLiteLight** was a formal project in Embededd Devices. Case it's Efficiency and Low Consumption. It would be extremely fit for light-weighted Software Designing, which means you guys do not need to install or configure Complex Database, just add the SQLite source code into your project, Things Done!

> * - [x] Fundamental Operation(Increase, Decrease, Modify, Delete)
> * - [x] Automatic backup module
> * - [x] MD5 Checksum Verifier
> * - [x] Thread safety
> * - [x] Fully backup solution
> * - [x] White-Box testing module
> * - [ ] Complex Construction(Have not been tested, but it could Meet ordinary needs Steadily)

The Best thing in the repository is the light-weighted. Since the database I have used can be added into the project in C source file as well as it's only take up nearly 5 MB of device memory.

```C++
#include <sqlite3.h>
```

###Table Data Structure(Your can modify the data structure base on what your actual needs)

|Argument|Seqence Number|Data Type|Expire Time|Data|Content|
|:-:|:-:|:-:|:-:|:-:|:-:|
|DATA_S|unsigned int|unsigned long long|unsigned char|unsigned int|char *|

Later, I will upload the performances of this DataBase Program on different platforms.

For more information, you can do some reading in http://www.sqlite.org
