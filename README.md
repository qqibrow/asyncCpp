asyncCpp
============
A not-that-fun journey to implement [async](https://github.com/caolan/async) in C++ with lots of template magic tricks

## Installation
### Requirements
git and cmake 2.8+

```bash
git clone https://github.com/qqibrow/asyncCpp.git && cd asyncCpp
./setup_gtest # download gtest
mkdir mybuild && cd mybuild
cmake .. && make && make test # or ./runUnitTests in the last step
```
Tested in Mac only

# License
MIT
