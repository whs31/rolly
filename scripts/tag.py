import os
import sys
import subprocess
import shutil
import argparse
import re

try:
    from termcolor import colored
except ModuleNotFoundError:
    def colored(text, color=None):
        return text


def getroot(foldername: str) -> str:
    __root = os.path.abspath(os.path.dirname(__file__))
    if os.path.basename(__root) == foldername:
        __root = os.path.abspath(os.path.join(__root, ".."))
    print(colored(f'- project root directory: {__root}', "magenta"))
    return __root


def find_executable(name: str):
    try:
        _a = subprocess.run(f'{name} --version', shell=True, stdout=subprocess.DEVNULL)
        if _a.returncode != 0:
            raise FileNotFoundError
        print(colored(f'- {name} found ({_a.stdout if _a.stdout else _a.stderr if _a.stderr else "unknown version"})',
                      "green"))
    except FileNotFoundError:
        print(colored(f'- {name} not found', "red"))
        raise FileNotFoundError


def strip_version(version: str) -> str:
    """
    Strips version from "X.Y.Z[.*]" to "X.Y.Z"
    :param version: specified version
    :return: stripped version
    """
    last_char = version[-1]
    if last_char.isdigit():
        return version
    else:
        return strip_version(version[:-1])


def patch_cmake(version: str, directory: str) -> None:
    """
    Finds CMakeLists.txt, searches for project version and replaces it with specified one
    :param version: specified version
    :param directory: project root directory or directory where CMakeLists.txt is located
    :return: None
    :raise: FileNotFoundError if CMakeLists.txt not found
    """
    print(colored(f'- patching CMakeLists.txt ({version})', "green"))
    try:
        with open(os.path.join(directory, "CMakeLists.txt"), "r") as f:
            lines = f.readlines()
            # use regex: VERSION (\d+\.)?(\d+\.)?(\*|\d+)
            for i, line in enumerate(lines):
                if re.search(r"VERSION (\d+\.)?(\d+\.)?(\*|\d+)", line) and not line.startswith("cmake_"):
                    lines[i] = re.sub(r"VERSION (\d+\.)?(\d+\.)?(\*|\d+)", f"VERSION {version}", line)
            with open(os.path.join(directory, "CMakeLists.txt"), "w") as f:
                f.writelines(lines)
    except FileNotFoundError:
        raise FileNotFoundError(f"CMakeLists.txt not found in {directory}")


def patch_conanfile(version: str, directory: str) -> bool:
    """
    Finds conanfile.py, searches for project version and replaces it with specified one.
    If conanfile.py not found, returns False.
    :param version: specified version
    :param directory: project root directory or directory where conanfile.py is located
    :return: True if conanfile.py found and patched, False otherwise
    """
    print(colored(f'- patching conanfile.py ({version})', "green"))
    try:
        with open(os.path.join(directory, "conanfile.py"), "r") as f:
            lines = f.readlines()
            for i, line in enumerate(lines):
                if re.search(r"version\s*=\s*\"(.*)\"", line):
                    lines[i] = re.sub(r"version\s*=\s*\"(.*)\"", f"version = \"{version}\"", line)
            with open(os.path.join(directory, "conanfile.py"), "w") as f:
                f.writelines(lines)
        return True
    except FileNotFoundError:
        print(colored(f"- conanfile.py not found in {directory}", "yellow"))
        return False


def patch_doxyfile(version: str, directory: str) -> bool:
    """
    Finds Doxyfile, searches for project version and replaces it with specified one
    :param version: specified version
    :param directory: project root directory or directory where Doxyfile is located
    :return: None
    :raise: FileNotFoundError if Doxyfile not found
    """
    print(colored(f'- patching Doxyfile ({version})', "green"))
    try:
        with open(os.path.join(directory, "Doxyfile"), "r") as f:
            lines = f.readlines()
            # use regex: VERSION (\d+\.)?(\d+\.)?(\*|\d+)
            for i, line in enumerate(lines):
                if re.search(r"PROJECT_NUMBER\s*=\s*(\S*)", line):
                    lines[i] = re.sub(r"PROJECT_NUMBER\s*=\s*(\S*)", f"PROJECT_NUMBER         = {version}", line)
            with open(os.path.join(directory, "Doxyfile"), "w") as f:
                f.writelines(lines)
        return True
    except FileNotFoundError:
        print(colored(f"- Doxyfile not found in {directory}", "yellow"))
        return False


def patch_include_version(version: str, filepath: str, prefix: str) -> None:
    try:
        print(colored(f'- patching {os.path.basename(filepath)} ({version})', "green"))
        major = version.split(".")[0]
        minor = version.split(".")[1]
        patch = version.split(".")[2]
        with open(filepath, "r") as f:
            lines = f.readlines()
            for i, line in enumerate(lines):
                if re.search(fr'#\s*define\s+{prefix}_VERSION_MAJOR\s+(\d+)', line):
                    lines[i] = re.sub(fr'#\s*define\s+{prefix}_VERSION_MAJOR\s+(\d+)',
                                      f'#define {prefix}_VERSION_MAJOR {major}', line)
                if re.search(fr'#\s*define\s+{prefix}_VERSION_MINOR\s+(\d+)', line):
                    lines[i] = re.sub(fr'#\s*define\s+{prefix}_VERSION_MINOR\s+(\d+)',
                                      f'#define {prefix}_VERSION_MINOR {minor}', line)
                if re.search(fr'#\s*define\s+{prefix}_VERSION_PATCH\s+(\d+)', line):
                    lines[i] = re.sub(fr'#\s*define\s+{prefix}_VERSION_PATCH\s+(\d+)',
                                      f'#define {prefix}_VERSION_PATCH {patch}', line)
            with open(filepath, "w") as f:
                f.writelines(lines)
    except FileNotFoundError:
        raise FileNotFoundError(f"{filepath} not found")


def tag_git(tag: str) -> None:
    """
    Tags git repository with specified tag
    :param tag: specified tag
    :return: None
    """
    find_executable("git")
    print(colored(f'- tagging git repository with {tag}', "green"))
    subprocess.run(["git", "tag", tag]
                   , stdout=subprocess.DEVNULL
                   , stderr=subprocess.DEVNULL
                   , check=True
                   , cwd=getroot("scripts"))

    print(colored(f'- pushing git repository with {tag}', "green"))
    subprocess.run(["git", "push", "origin", tag]
                   , stdout=subprocess.DEVNULL
                   , stderr=subprocess.DEVNULL
                   , check=True
                   , cwd=getroot("scripts"))


def extract_semver(string: str) -> str:
    return re.search(r"(\d+\.)?(\d+\.)?(\*|\d+)", string).group(0)


def print_semver(root: str, filename: str, regex: str) -> str:
    try:
        with open(os.path.join(root, filename), "r") as f:
            lines = f.readlines()
            for i, line in enumerate(lines):
                if re.search(regex, line) and not line.startswith("cmake_"):
                    print(colored(f"- {filename:<30}: {extract_semver(line)}", "green"))
                    break
    except FileNotFoundError:
        print(colored(f"- {filename:<30}: not found", "yellow"))


def print_include_version(filepath: str, prefix: str) -> str:
    def extract_define(line: str, prefix: str, suffix: str) -> str:
        return re.search(fr'#\s*define\s+{prefix}_VERSION_{suffix}\s+(\d+)', line).group(1)

    try:
        with open(filepath, "r") as f:
            lines = f.readlines()
            for i, line in enumerate(lines):
                if re.search(fr'#\s*define\s+{prefix}_VERSION_MAJOR\s+(\d+)', line):
                    major = extract_define(line, prefix, "MAJOR")
                if re.search(fr'#\s*define\s+{prefix}_VERSION_MINOR\s+(\d+)', line):
                    minor = extract_define(line, prefix, "MINOR")
                if re.search(fr'#\s*define\s+{prefix}_VERSION_PATCH\s+(\d+)', line):
                    patch = extract_define(line, prefix, "PATCH")
            print(colored(f"- {os.path.basename(filepath):<30}: {major}.{minor}.{patch}", "green"))
    except FileNotFoundError:
        print(colored(f"- {os.path.basename(filepath):<30}: not found", "yellow"))


def show_versions(root):
    print_semver(root, "CMakeLists.txt", r"VERSION (\d+\.)?(\d+\.)?(\*|\d+)")
    print_semver(root, "conanfile.py", r"version\s*=\s*\"(.*)\"")
    print_semver(root, "Doxyfile", r"PROJECT_NUMBER\s*=\s*(\S*)")
    print_include_version(os.path.join(root, "include", "rolly", "global", "version_definitions.h"), "ROLLY")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-v", "--version", type=str, help="New project version")
    parser.add_argument("-g", "--git", action="store_true", help="Tag git repository")
    parser.add_argument("-s", "--show", action="store_true", help="Show current versions")
    args = parser.parse_args()
    root = getroot("scripts")
    if args.show:
        show_versions(root)
        sys.exit(0)
    patch_cmake(strip_version(args.version), root)
    patch_conanfile(args.version, root)
    patch_doxyfile(args.version, root)
    patch_include_version(
        args.version,
        os.path.join(root, "include", "rolly", "global", "version_definitions.h"),
        "ROLLY"
    )
    if args.git:
        tag_git(args.version)


if __name__ == "__main__":
    try:
        main()
    except FileNotFoundError as e:
        print(colored(f'⚠️ failed to tag project due to: {e}', "red"))
        sys.exit(1)
