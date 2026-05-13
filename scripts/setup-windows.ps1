$ErrorActionPreference = "Stop"

if (-not (Test-Path ".venv")) { python -m venv .venv }
& .\.venv\Scripts\Activate.ps1
pip install -q -r requirements.txt
git config core.hooksPath scripts/hooks

if (Test-Path "build") { Remove-Item -Recurse -Force build }
if (Test-Path "CMakeUserPresets.json") { Remove-Item -Force CMakeUserPresets.json }

conan profile detect --force
conan install . -of . -b missing -s build_type=Release -s compiler.cppstd=20

cmake --preset conan-default
cmake --build --preset conan-release

Write-Host "`n[SUCCESS] Build complete!" -ForegroundColor Green