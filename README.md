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

# Create database in PostgreSQL

1. check if psql tool exists (`psql --version`) -> else - install postgresql with this tool inside.
2. get password of `postgres` user / or change it if you forget:
    1. `$ sudo -u postgres psql`.
    2. `$ psql`.
    2. `ALTER USER postgres WITH PASSWORD 'newpassword'`.
3. create new role for our database.
    1. 