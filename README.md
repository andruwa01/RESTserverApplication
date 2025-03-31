### Предварительные требования

- postgresql (использовалась версия 17) (`sudo apt install postgresql-17`) (как добавить репозитории: https://www.postgresql.org/download/linux/ubuntu/ если не репозиторий для `$ apt install`).
- cmake 3.22.1 (`$ sudo apt install cmake`)
- Boost библиотека (c++) (`$ sudo apt install libboost-all-dev`)
- nlohmann json библиотека (`$ sudo apt install nlohmann-json3-dev`)
- libpq (C-библиотека для libpqxx и postgresql) (`$ sudo apt install libpq-dev`)
- libpqxx:
    - `$ git submodule update --init --recursive` -> обновляет/добавляет файлы в deps/libpqxx. Эта директория
    будет использоваться в cmake при сборке проекта.
- pkg-config (cmake ищет его во время процесса сборки): `$ sudo apt install pkg-config`

### Создание базы данных в PostgreSQL

В БД В STATUS ПОЛЕ ПРИСУТСТВУЕТ in_progress, я в итоге решил его не использовать, поэтому не обращайте внимания.

1. Проверка postgres:
    - проверьте наличие утилиты psql (`psql --version`) -> если отсутствует - установите postgresql с этой утилитой.
2. Получение/изменение пароля пользователя `postgres` (чтобы зайти под ним):
    1. `$ sudo -u postgres psql`
    2. `ALTER USER postgres WITH PASSWORD 'newpassword'` - меняем пароль для `postgres`.
3. Создание новой роли `user` для нашей базы данных от пользователя `postgres` с паролем `123`:
    - `CREATE ROLE "user" WITH LOGIN PASSWORD '123'`
4. Создание базы данных от лица `postgres` с указанным пользователем `user` в качестве владельца и именем `tasks_employees`:
    - `$ createdb -h localhost -U postgres -O user tasks_employees`
5. Загрузка дампа .sql в базу данных:
    - `$ psql -h localhost -U user -d tasks_employees -f tasks_employees.sql`

### Запуск REST API сервера

1. Подготовьте PostgreSQL, настройте прослушивание новых подключений.
2. Сборка и запуск серверного приложения:
    0. Изучите предварительные требования и настройте БД.
    1. `$ mkdir build` - создание папки с файлами сборки cmake.
    2. `$ cd build` - переход в директорию сборки.
    3. `$ cmake ..` - генерация файлов make.
    4. `$ make` - компиляция проекта (возможны предупреждения - НЕ БЕСПОКОЙТЕСЬ).
    5. `$ ./build/serverApp` - запуск исполняемого файла приложения.

Сервер слушает порт 8080 для подключений на него.
В коде для подключения к БД используются данные:
`"dbname=tasks_employees user=user password=123 host=localhost port=5432"` (см. handle_request.cpp).
Если у вас будут другие данные для БД - будет ошибка.