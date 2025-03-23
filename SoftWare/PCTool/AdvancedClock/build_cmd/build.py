import os
import sys
import shutil

def copy_folder(source_folder, destination_folder):
    shutil.copytree(source_folder, destination_folder)

def copy_file(source_folder, destination_folder):
    shutil.copy(source_folder, destination_folder)
    

class CustomError(Exception):
    def __init__(self, message):
        self.message = message
        super().__init__(self.message)

def main():

    version = sys.argv[1]
    mode = sys.argv[2]
    mingw_compile_path = sys.argv[3]
    qt_path = sys.argv[4]

    os.system(f'mkdir  ..\\build_{version}\\out')
    sys_path = os.getenv('Path')
    path_add=f"{qt_path}_{version}\\bin;"+sys_path
    os.environ['Path']=path_add

    try:
        rlt = os.system(f'{mingw_compile_path}_{version}\\bin\\qmake.exe ..\\AdvancedClock.pro -o ..\\build_{version}')
        if (0 != rlt):
            raise CustomError(f"error: generate makefile_{version}_{mode} failed")
            
        os.chdir(f'..\\build_{version}')
        rlt = os.system(f'mingw32-make.exe -f Makefile.{mode} -j16' )
        if (0 != rlt):
            raise CustomError(f"error: compile_{version}_{mode} failed")
            
        source_file = f'{mode}\\AdvancedClock.exe'
        destination_file = 'out\\AdvancedClock.exe'
        copy_file(source_file, destination_file)

        os.chdir(f'..\\build_{version}\\out')
        os.system(f'{mingw_compile_path}_{version}\\bin\\windeployqt.exe AdvancedClock.exe')

    except CustomError as e:
        print(f'{e}')


if __name__ == "__main__":
   main()
