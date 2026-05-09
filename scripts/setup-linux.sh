#!/bin/env bash

set -euo pipefail

python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
git config core.hooksPath scripts/hooks
conan profile detect --force
conan install . --output-folder=build --build=missing -s build_type=Release -s compiler.cppstd=20
cmake --preset conan-release
cmake --build --preset conan-release --parallel