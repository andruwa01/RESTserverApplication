### Preinstalled

- postgresql (i used 17 version) (`sudo apt install postgresql-17`) (how to add repositories: https://www.postgresql.org/download/linux/ubuntu/)
- cmake 3.22.1 (`$ sudo apt install cmake`)
- Boost library (c++) (`$ sudo apt install libboost-all-dev`)
- nlohmann json library (`$ sudo apt install nlohmann-json3-dev`)
- libpq (c-library for libpqxx and postrgesql) (`$ sudo apt install libpq-dev`)
- libpqxx:
    - `$ git submodule update --init --recursive` -> it updates / add files to deps/libpqxx. This dir
    will be used in cmake when you try to build with cmake.
- pkg-config (cmake try to find it during the bulding process): `$ sudo apt install pkg-config`

### Create database in PostgreSQL

1. checking postgres.
    - check if psql tool exists (`psql --version`) -> else - install postgresql with this tool inside.
2. get password of `postgres` user / or change it if you forget:
    1. `$ sudo -u postgres psql`.
    2. `ALTER USER postgres WITH PASSWORD 'newpassword'`.
3. create new role for our database from user `postgres`
    - `CREATE ROLE "user" WITH LOGIN PASSWORD 'newpassword'`  
4. create database for user as owner.
    - `$ createdb -h localhost -U postgres -O user tasks_employees`
5. load dump .sql to db.
    - `$ psql -h localhost -U user -d tasks_employees -f tasks_employees.sql`


### Run the REST API server app

1. Prepare PostgreSQL, make it listening new connections.
2. Build and run the server app.
	1. `$ mkdir Build` - create folder with cmake build files.
	2. `$ cd build` - move to build dir. 
	3. `$ cmake ..` - generate cmake files.
	4. `$ cmake --build .` - compile project (could be warnings - DON'T WORRY).
	5. `$ ./build/serverApp` - run the application executable.
