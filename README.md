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
## Настройка

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

---

## Разработка

### Качество кода
В проекте настроен `pre-commit` хук (через `core.hooksPath`). Перед каждым коммитом автоматически запускается `clang-format`. Если код не соответствует стилю, он будет отформатирован автоматически.