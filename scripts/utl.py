import os
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