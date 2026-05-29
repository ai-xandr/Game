# David the Vodolaz

Кроссплатформенная **C++** игра на базе **SFML 3.0.2**. Поддерживаемые **ОС**: Windows, Linux

David the Vodolaz — погружение в таинственные глубины: управляйте водолазом, исследуйте руины, сражайтесь с морскими обитателями и собирайте очки в динамичной аркадной игре с пиксельной эстетикой, красивой музыкой и простым интуитивным управлением.

## 🗣️ Как скачать?
* Перейдите в раздел **Releases**
* Скачайте **архив** под вашу ОС
* Распакуйте его, перейдите в распакованную папку и запустите **Game**
* Наслаждайтесь!

## Требования для разработки

* **C++ Компилятор** (с поддержкой C++20)
* **Python** 3.8+

## Быстрый старт

### Клонирование репозитория:
```shell
git clone git@github.com:ai-xandr/Game.git
cd Game
```
### Полная Настройка

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

## Разработка

При добавлении новых, удалении или переименовании .cpp и .hpp файлов необходимо переконфигурировать и пересобрать проект: 
Windows:
```
cmake --preset conan-default
cmake --build --preset conan-release
```

Если было изменено только содержимое .cpp и .hpp файлов, а структура проекта не менялась, то достаточно только перекомпилировать проект:
```
cmake --build --preset conan-release
```