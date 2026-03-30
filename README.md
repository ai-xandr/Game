# Игра на C++
Для сборки проекта необходимы:
* **Компилятор**: GCC или Clang.
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
.\build\bin\Release\Game.exe
```
## Разработка
При создании новых .cpp/.hpp файлов обязательно добавить их в `CMakeLists.txt` в раздел `SOURCE`, а затем пересобрать проект
`cmake --build build --config Release`, после можно запускать
(см. раздел запуск)