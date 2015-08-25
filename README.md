cpp_template
============
[![Build Status](https://travis-ci.org/qqibrow/cpp_template.svg?branch=master)](https://travis-ci.org/qqibrow/cpp_template)

c++ project template, incldue:

  * project skeleton using cmake
  * gtest

## Installation
### Requirements
git and cmake 2.8+

```bash
git clone https://github.com/qqibrow/cpp_template.git && cd cpp_template
./setup_gtest # download gtest
mkdir mybuild && cd mybuild
cmake .. && make && make test # or ./runUnitTests in the last step
```
Tested in Mac, Ubuntu and RHEL6

# License
MIT
