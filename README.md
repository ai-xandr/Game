# Game (C++)

Кроссплатформенная игра на базе **SFML 3.0.2**.

## Требования

* **C++ Компилятор** (с поддержкой C++20)
* **Python** 3.8+

## Быстрый старт

### Клонирование репозитория:
```shell
git clone git@github.com:ai-xandr/Game.git
cd Game
```
### Настройка

#### Windows
```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
.\scripts\setup-windows.ps1
``` 

#### Linux
```bash
./scripts/setup-linux.sh
``` 

### Запуск

#### Windows
```powershell
.\build\Release\Game.exe
```

#### Linux / macOS
```bash
./build/build/Release/Game
```