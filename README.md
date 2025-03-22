# Preinstalled
- postgresql (`sudo apt install postgresql`)
- cmake 3.22.1 (`sudo apt install cmake`)
- Boost library (c++) (`sudo apt install libboost-all-dev`)
- nlohmann json library (`sudo apt install nlohmann-json3-dev`)
- libpq (c-library for libpqxx and postrgesql) (`sudo apt install libpq-dev`)
- libpqxx:
    - `git submodule update --init --recursive` -> it updates / add files to deps/libpqxx. This dir
    will be used in cmake when you try to build with cmake.
- pkg-config (cmake try to find it during the bulding process): `sudo apt install pkg-config`