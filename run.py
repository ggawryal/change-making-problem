import subprocess
import os

def printAndRunCommand(command):
    print(command)
    os.system(command)

def try_install_fftw():
    script = 'fftw_install.sh'
    printAndRunCommand('chmod u+x '+script)
    command = './' + script
    print(command)
    r = subprocess.Popen([command])
    r.communicate()[0]
    return r.returncode

def install_fftw():
    print('downloading and installing fftw library')
    retry = True
    while retry:
        r = try_install_fftw()
        retry = False
        if r in (1,2):
            print('retry? (y/n)')
            retry = True if input() == 'y' else False
        elif r == 3:
            print('retry with sudo? (y/n)')
            if input() == 'y':
                printAndRunCommand('cd fftw-3.3.9 && sudo make install')
            return


def try_compile_solutions(use_fftw):
    flags = ' -lfftw3f' if use_fftw else ' -DDONT_USE_FFTW_LIB'
    printAndRunCommand('g++ -std=c++11 -O3 benchmark.cpp -o benchmark' + flags)
    if not os.path.isfile('benchmark'):
        return False
    printAndRunCommand('g++ -std=c++11 -O3 test_correctness.cpp -o test_correctness' + flags)
    if not os.path.isfile('test_correctness'): 
        return False
    return True


def compile_solutions():
    print('compiling solutions')
    retry, use_fftw = True, True
    while retry:
        compiled = try_compile_solutions(use_fftw)
        if not compiled:
            if use_fftw == False:
                print('something went really wrong, compilation failed even without fftw library')
                exit(0)
            print('fftw library is probably not installed, type:\n(d) to download and install it\n(n) to compile without it (recommended only if installing fails)\n(q) to quit')
            r = input()
            if r in ('d', '(d)'):
                install_fftw()
            elif r in ('n','(n)'):
                retry, use_fftw = True, False
            else:
                exit(0)
        else:
            print('compilation finished succesfully ' + ('with fftw library' if use_fftw else 'with slower fft written by author'))
            retry = False


compile_solutions()
printAndRunCommand('./test_correctness')