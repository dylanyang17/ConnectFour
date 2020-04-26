import os


def del_extern(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
        for i in range(len(lines)):
            lines[i] = lines[i].replace('extern "C" __declspec(dllexport) ', '')
    with open(filename, 'w') as f:
        for i in range(len(lines)):
            f.write(lines[i])


if __name__ == '__main__':
    os.system('copy ..\\Strategy\\Strategy\\*.cpp .\\ /Y')
    os.system('copy ..\\Strategy\\Strategy\\*.h .\\ /Y')
    s = 'extern "C" __declspec(dllexport) '
    del_extern('Strategy.h')
    del_extern('Strategy.cpp')
