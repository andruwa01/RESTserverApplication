# Preinstalled
- cmake 3.22.1 
- Boost library (c++)   (`sudo apt install libboost-all-dev`)
- nlohmann json library (`sudo apt install nlohmann-json3-dev`)
- libpq (for libpqxx and postrgesql) (`sudo apt install libpq-dev`)
- libpqxx 
    - `git submodule update --init --recursive` -> it updates / add files to deps/libpqxx. This dir
    will be used in cmake when you try to cmake --build.