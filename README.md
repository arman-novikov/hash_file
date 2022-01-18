## Utility to hash files by chunks

### Usage

**compiled utility usage**

run in terminal supplying 2 and 1 optional arguments.

Parameters are parsed one by one.

1. file to hash
2. hash result file
3. each block size (in bytes). Optional. By default it is `1Mb`.

**Example (Linux)**

`./hash_file my_file res.txt 1024`

_Legend_

* Hashes `my_file`;
* Writes result into `res.txt`;
* Each block is 1Kb.

**docker usage**

`docker build -t hash_file .`

_Legend_

* builds image
* builds app
* runs app hashing `test/test_file` _(whatever supplied with this name)_
* prints generated hash _(block size is 1Mb)_ like:

```
res:
44EE2C7760C927C7DF0567043AC0EED3


```

*todo: make image install specified boost version*


### Content

* `hasher` interface and a specific specialization (md5) of hashing
* `test` test data (currently there are no autotests)
* `worker` class to deal with hashing of chunks stored in Tasks
* `CMakeList.txt` cmake receipt to build app 
* `Dockerfile` scheme to build image to hash the `test/test_file`
* `main.cpp`

### Dependencies

* C++17
* Boost 1.71
* CMake >3.5