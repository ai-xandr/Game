```markdown
# Game (C++)

Кроссплатформенная игра на базе **SFML 3.0.2**.

## Требования

* **C++ Компилятор** (с поддержкой C++20).
* **CMake** 3.15+.
* **Conan** 2.0+.
* **Python** 3.8+.
* **Ninja** (рекомендуется).

## Быстрый старт

Клонирование репозитория:
```shell
git clone git@github.com:ai-xandr/Game.git
cd Game
```

### 1. Подготовка окружения (venv)
Рекомендуется использовать виртуальное окружение для изоляции инструментов.

#### Windows
```powershell
python -m venv .venv
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
.\.venv\Scripts\Activate.ps1
pip install conan ninja
```

#### Linux / macOS
```bash
python3 -m venv .venv
source .venv/bin/activate
pip install conan ninja
```

### 2. Установка зависимостей и настройка Git
Использование Conan для генерации пресетов сборки и настройка хуков.
```bash
# Настройка Git-хуков (единоразово)
git config core.hooksPath scripts/hooks

# Установка библиотек и генерация пресетов CMake
conan profile detect --force
conan install . --output-folder=build --build=missing -s build_type=Release -s compiler.cppstd=20
```

### 3. Сборка

#### Конфигурация
```bash
cmake --preset conan-default
```

#### Компиляция
```bash
cmake --build --preset conan-release --parallel
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