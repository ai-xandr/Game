# аналог set -e (в bash)
$ErrorActionPreference = "Stop"

python -m venv .venv
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
& .\.venv\Scripts\Activate.ps1

python -m pip install --upgrade pip
pip install -r requirements.txt

git config core.hooksPath scripts/hooks

conan profile detect --force
conan install . --output-folder=build --build=missing -s build_type=Release -s compiler.cppstd=20

cmake --preset conan-release
cmake --build --preset conan-release --parallel

# вывод при успешной настройке
Write-Host "`n[SUCCESS] Setup and build complete!" -ForegroundColor Green