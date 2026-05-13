# Game (C++)

Кроссплатформенная игра на базе **SFML 3.0.2**.

## Требования

* **C++ Компилятор** (с поддержкой C++20)
* **Python** 3.8+

## Быстрый старт

Клонирование репозитория:
```shell
git clone git@github.com:ai-xandr/Game.git
cd Game
```
### Полная Настройка

#### Windows
```powershell
.\scripts\setup-windows.ps1
``` 

#### Linux
```bash
./scripts/setup-linux.sh
``` 

## Запуск

#### Windows
```powershell
.\build\build\Release\Game.exe
```

#### Linux / macOS
```bash
./build/build/Release/Game
```

## Разработка

При добавлении новых, удалении или переименовании .cpp и .hpp файлов необходимо переконфигурировать и пересобрать проект: 
```
cmake --preset conan-default
cmake --build --preset conan-release
```

Если было измнено только сожержимое .cpp и .hpp файлов, а структура проекта не менялась, то достаточно только перекомпилировать проект:
```
cmake --build --preset conan-release
```