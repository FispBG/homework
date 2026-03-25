# homework

### Описание: 
Клиенто-серверное приложение, которое имеет приложение с раздельным
запуском для сервера и клиента. 
- Запуск клиента после сборки: ./client -a 1.2.3.4 -p 3432 -i 2343 <br><br>

  #### Необходимые параметры для программы:
1. ip(-a), т.к. нужен адрес, куда присоединяться.
2. port(-p), т.к. нужен порт, куда присоединяться.
3. id(-i), нужен, чтобы понимать, кто запускает программу.<br><br>

   #### Опциональные параметры: <br>

4. role(-r), в целом можно задать дефолтное значение, как Client.
5. lib(-l), она может быть по дефолту та, где запускается программа.

- Запуск клиента после сборки: ./server

---

### Зависимости

Компилятор
- GCC >= 13.3.0
- Стандарт: C++20 (C++17 тоже подойдет)

#### Библиотеки
- GTest >= v1.14.x

#### Окружение
- ОС: Linux
- CMake >= 3.28

#### Сборка
- cmake

---

### Запуск тестов

- для клиента <br><br>
cd client/tests <br>
cmake CMakeList.txt <br>
make <br>
./test<br><br>

- для сервера <br><br>
  cd server/tests <br>
  cmake CMakeList.txt <br>
  make <br>
  ./test

---

### Санитазеры и статический анализатор
- для запуска адресного санитайзера в CMakeList.txt по адресам ./client/ и ./server/
раскомментируйте: 
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

---

### Структура файлов 

project/<br>
├── README.md<br>
│<br>
├── commonFunc/<br>
│   ├── includes/<br>
│   │   ├── functions.h<br>
│   │   └── StandardPackets.h<br>
│   └── sources/<br>
│       └── functions.cpp<br>
│<br>
├── client/<br>
│   ├── CMakeLists.txt<br>
│   ├── cmake/<br>
│   │   └── cppcheck.cmake<br>
│   ├── includes/<br>
│   │   ├── AppSettings.h<br>
│   │   ├── DataPool.h<br>
│   │   ├── Menu.h<br>
│   │   ├── NetWork.h<br>
│   │   ├── SocketClient.h<br>
│   │   └── Tests.h<br>
│   ├── sources/<br>
│   │   ├── main.cpp<br>
│   │   ├── AppSettings.cpp<br>
│   │   ├── Menu.cpp<br>
│   │   ├── NetWork.cpp<br>
│   │   ├── SocketClient.cpp<br>
│   │   └── Tests.cpp<br>
│   └── tests/<br>
│       ├── CMakeLists.txt<br>
│       └── test_input.cpp<br>
│
└── server/<br>
├── CMakeLists.txt<br>
├── includes/<br><br>
│   ├── EpollServer.h<br>
│   ├── ManageServer.h<br>
│   ├── MathFunc.h<br>
│   └── SocketServer.h<br>
├── sources/<br>
│   ├── main.cpp<br>
│   ├── EpollServer.cpp<br>
│   ├── ManageServer.cpp<br>
│   ├── MathFunc.cpp<br>
│   └── SocketServer.cpp<br>
└── tests/<br>
├── CMakeLists.txt<br>
└── test_input.cpp<br>

---

### Лицензия

Copyright (c) 2026 fisp

---

### Контакты

- GitHub: [github.com/fisp](https://github.com/fisp)