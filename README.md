# homework

### Описание: 
Клиенто-серверное приложение, которое имеет приложение с раздельным
запуском для сервера и клиента.

Взаимодействие клиента и сервера происходит при вводе вектора(он считался, отправился на сервер,
обработался, вернулся результат клиенту).

- Запуск клиента после сборки: ./client -a 1.2.3.4 -p 3432 -i 2343 <br>
- Получение справки по возможным флагам: ./client --help

  #### Необходимые параметры для программы:
1. ip(-i), т.к. нужен адрес, куда присоединяться.
2. port(-p), т.к. нужен порт, куда присоединяться.

   #### Опциональные параметры: <br>

3. id(-id), нужен, чтобы понимать, кто запускает программу.
4. role(-r), в целом можно задать дефолтное значение, как Client.
5. lib(-l), она может быть по дефолту та, где запускается программа.

- Запуск клиента после сборки: ./server -p 8080
- Получение справки по возможным флагам: ./server --help

  #### Необходимые параметры для программы:
1. port(-p), т.к. нужен порт, что прослушивать.

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
  cmake -B build/ CMakeList.txt <br>
  cmake --build build/ <br>
  cd build/<br>
  ./test<br>

- для сервера <br><br>
  cd server/tests <br>
  cmake -B build/ CMakeList.txt <br>
  cmake --build build/ <br>
  cd build/<br>
  ./test<br>

---

### Санитазеры и статический анализатор
- для запуска адресного санитайзера в CMakeList.txt по адресам ./client/ и ./server/
раскомментируйте: 
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

---

### Структура файлов 

├── client <br>
│   ├── build <br>
│   ├── cmake <br>
│   │   └── cppcheck.cmake <br>
│   ├── CMakeLists.txt <br>
│   ├── includes <br>
│   │   ├── AppSettings.h <br>
│   │   ├── DataPool.h <br>
│   │   ├── Menu.h <br>
│   │   ├── NetWork.h <br>
│   │   ├── SocketClient.h <br>
│   │   └── Tests.h <br>
│   ├── sources <br>
│   │   ├── AppSettings.cpp <br>
│   │   ├── DataPool.cpp <br>
│   │   ├── main.cpp <br>
│   │   ├── Menu.cpp <br>
│   │   ├── NetWork.cpp <br>
│   │   ├── SocketClient.cpp <br>
│   │   └── Tests.cpp <br>
│   └── tests <br>
│      ├── AppSettingsTest.cpp <br>
│      ├── CheckAppTest.cpp <br>
│      ├── CMakeLists.txt <br>
│      ├── DataPoolTets.cpp <br>
│      └── FunctionTest.cpp <br>
├── commonFunc <br>
│   ├── includes <br>
│   │   ├── PacketFunction.h <br>
│   │   ├── ResultStatus.h <br>
│   │   ├── StandardPackets.h <br>
│   │   ├── StringFunction.h <br>
│   │   ├── ValidationFunction.h <br>
│   │   └── VectorProcess.h <br>
│   └── sources <br>
│      ├── PacketFunction.cpp <br>
│      ├── ResultStatus.cpp <br>
│      ├── StringFunction.cpp <br>
│      ├── ValidationFunction.cpp <br>
│      └── VectorProcess.cpp <br>
├── README.md <br>
└── server <br>
├── cmake <br>
│   └── cppcheck.cmake <br>
├── CMakeLists.txt <br>
├── includes <br>
│   ├── ConfigApp.h <br>
│   ├── EpollServer.h <br>
│   ├── ManageServer.h <br>
│   ├── MathFunc.h <br>
│   ├── SocketServer.h <br>
│   └── ThreadPool.h <br>
├── sources <br>
│   ├── ConfigApp.cpp <br>
│   ├── EpollServer.cpp <br>
│   ├── main.cpp <br>
│   ├── ManageServer.cpp <br>
│   ├── MathFunc.cpp <br>
│   ├── SocketServer.cpp <br>
│   └── ThreadPool.cpp <br>
└── tests <br>
   ├── CMakeLists.txt <br>
   ├── configAppTest.cpp <br>
   ├── epollTest.cpp <br>
   ├── mathFuncTest.cpp <br>
   └── socketTest.cpp <br>

---

### Лицензия

Copyright (c) 2026 fisp

---

### Контакты

- GitHub: [github.com/fisp](https://github.com/fisp)