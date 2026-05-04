#!/usr/bin/env python3
import os
import sys
import subprocess
import platform
import shutil
import re

VENV_DIR = ".venv"

REQUIRED_PY_PACKAGES = [
    "conan==2.*",
    "pre-commit"
]


def run(cmd, check=True):
    print("> " + " ".join(cmd))
    subprocess.run(cmd, check=check)


def exists(cmd):
    return shutil.which(cmd) is not None


def get_venv_python():
    system = platform.system()
    py = os.path.join(
        VENV_DIR,
        "Scripts" if system == "Windows" else "bin",
        "python.exe" if system == "Windows" else "python"
    )
    return py if os.path.exists(py) else None


def ensure_venv():
    if not os.path.exists(VENV_DIR):
        run([sys.executable, "-m", "venv", VENV_DIR])

    py = get_venv_python()
    if not py:
        raise RuntimeError("venv python not found")

    return py


def ensure_pip_packages(py):
    run([py, "-m", "pip", "install", "--upgrade", "pip"])

    for pkg in REQUIRED_PY_PACKAGES:
        name = pkg.split("=")[0]
        res = subprocess.run(
            [py, "-m", "pip", "show", name],
            stdout=subprocess.DEVNULL
        )
        if res.returncode != 0:
            run([py, "-m", "pip", "install", pkg])


def detect_gcc_version():
    try:
        # более надёжно
        out = subprocess.check_output(
            ["gcc", "-dumpfullversion"],
            text=True
        ).strip()

        print(f"[DEBUG] gcc version raw: {out}")

        major = int(out.split(".")[0])
        return major

    except Exception as e:
        print(f"[WARN] failed to detect gcc version: {e}")
        return None


def normalize_gcc_version(version):
    if version is None:
        print("[WARN] gcc version unknown → fallback 12")
        return "12"

    if version >= 15:
        print(f"[INFO] gcc {version} → normalized to 15 (Conan max)")
        return "15"

    return str(version)


def generate_conan_profile(build_type):
    os.makedirs("build", exist_ok=True)

    profile_path = os.path.join("build", "conan_profile")

    if os.path.exists(profile_path):
        os.remove(profile_path)

    gcc_version = normalize_gcc_version(detect_gcc_version())

    profile = f"""
[settings]
os=Linux
arch=x86_64
compiler=gcc
compiler.version={gcc_version}
compiler.libcxx=libstdc++11
compiler.cppstd=gnu17
build_type={build_type}

[conf]
tools.system.package_manager:mode=check
"""

    with open(profile_path, "w") as f:
        f.write(profile.strip())

    return profile_path


def ensure_conan(py):
    conan = os.path.join(
        VENV_DIR,
        "bin" if platform.system() != "Windows" else "Scripts",
        "conan" + (".exe" if platform.system() == "Windows" else "")
    )

    if os.path.exists(conan):
        return conan

    fallback = shutil.which("conan")
    if fallback:
        return fallback

    raise RuntimeError("conan not found")


def ensure_git_hooks():
    if os.path.isdir(".git") and os.path.isdir("scripts/hooks"):
        run(["git", "config", "core.hooksPath", "scripts/hooks"])


def ensure_tools():
    if not exists("cmake"):
        raise RuntimeError("cmake not found")

def debug_profile(path):
    print("\n[DEBUG] Conan profile content:")
    with open(path) as f:
        print(f.read())
    print()

def conan_install(conan, profile_path):
    if not (os.path.exists("conanfile.txt") or os.path.exists("conanfile.py")):
        return

    run([
        conan, "install", ".",
        "--output-folder=build",
        "--build=missing",
        "--profile:host", profile_path,
        "--profile:build", profile_path
    ])


def cmake_configure(build_type):
    system = platform.system()
    generator = "Unix Makefiles" if system != "Windows" else "Visual Studio 17 2022"

    tc_file = None
    generators_dir = None
    for root, dirs, files in os.walk("build"):
        if "conan_toolchain.cmake" in files:
            tc_file = os.path.abspath(os.path.join(root, "conan_toolchain.cmake"))
            generators_dir = os.path.abspath(root)
            break

    if not tc_file:
        raise RuntimeError("conan_toolchain.cmake не найден. Проверь вывод conan install.")

    cmd = [
        "cmake", "-B", "build",
        "-G", generator,
        f"-DCMAKE_TOOLCHAIN_FILE={tc_file}",
        f"-DCMAKE_PREFIX_PATH={generators_dir}"
    ]

    if system != "Windows":
        cmd.append(f"-DCMAKE_BUILD_TYPE={build_type}")

    run(cmd)


def cmake_build(build_type):
    cmd = ["cmake", "--build", "build"]

    if platform.system() == "Windows":
        cmd += ["--config", build_type]
    else:
        cmd += ["--parallel", str(os.cpu_count() or 1)]

    run(cmd)


def main():
    build_type = os.environ.get("BUILD_TYPE", "Release")

    ensure_tools()

    py = ensure_venv()
    ensure_pip_packages(py)

    conan = ensure_conan(py)

    ensure_git_hooks()
    
    profile = generate_conan_profile(build_type)
    debug_profile(profile)

    conan_install(conan, profile)
    cmake_configure(build_type)
    cmake_build(build_type)

    print(f"\nDone ({build_type}) → build/bin")


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print("ERROR:", e)
        sys.exit(1)