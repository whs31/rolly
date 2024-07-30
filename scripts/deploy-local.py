import subprocess
import os
import shutil

try:
    from termcolor import colored
except ImportError:
    def colored(text, color=None, on_color=None, attrs=None):
        return text
    

def getroot(foldername: str) -> str:
    __root = os.path.abspath(os.path.dirname(__file__))
    if os.path.basename(__root) == foldername:
        __root = os.path.abspath(os.path.join(__root, ".."))
    print(colored(f'▶ project root directory: {__root}', "magenta"))
    return __root


def main():
    root = getroot("scripts")
    if os.path.exists(os.path.join(root, "build")):
        print(colored("▶ removing old build folder", "magenta"))
        shutil.rmtree(os.path.join(root, "build"))
    print(colored("▶ building project", "magenta"))
    subprocess.run(["conan", "create", ".", "-b", "missing"], cwd=root)
    print(colored("▶ building project done!", "green"))


if __name__ == "__main__":
    main()