# Game (C++)

Кроссплатформенная игра на базе **SFML 3.0.2**.

## Требования

* **C++ Компилятор**
* **Система сборки**: CMake 3.11+.
* **Пакетный менеджер**: Conan 2.0+.
* **Интерпретатор**: Python 3.8+ (для скриптов автоматизации).
* **Форматировщик кода**: Clang-Format.

## Быстрый старт

### 1. Подготовка окружения
Скрипт автоматически настраивает Git-хуки и проверяет наличие необходимых инструментов.

**Linux / macOS:**
```bash
python3 scripts/setup.py
```

**Windows:**
```powershell
python scripts/setup.py
```

### 2. Установка зависимостей
Использование Conan для загрузки и сборки SFML.

**Универсальная команда:**
```bash
conan install . --output-folder=build --build=missing -s build_type=Release
```

### 3. Сборка

**Linux / macOS:**
```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel $(nproc)
```

**Windows (PowerShell):**
```powershell
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel $env:NUMBER_OF_PROCESSORS
```

## Запуск

### Linux / macOS
```bash
./build/bin/Game
```

### Windows
```powershell
.\build\bin\Game.exe
```
Если выдает ошибку, то попробовать этот вариант:

```powershell
.\build\bin\Release\Game.exe
```

---

## Разработка

### Качество кода
В проекте настроен `pre-commit` хук. Перед каждым коммитом автоматически запускается `clang-format`. Если код не соответствует стилю, он будет отформатирован автоматически перед фиксацией.