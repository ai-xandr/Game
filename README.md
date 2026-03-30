# Игра на C++
Для сборки проекта необходимы:
* **Компилятор**: GCC (версия 11+) или Clang.
* **Система сборки**: CMake 3.11+.
## Сборка и запуск
### Клонирование репозитория
```bash
git clone git@github.com:ai-xandr/Game.git
cd Game
```
### Конфигурация и компиляция
```bash
cmake -B build 
cmake --build build --config Release
```
### Запуск
### Linux:
```bash
./build/bin/Game
```
### Windows:
```shell
build/bin/Release/Game.exe
```