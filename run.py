import subprocess
import os

directory = os.path.dirname(os.path.realpath(__file__))



def printAndRunCommand(command):
    print(command)
    r = subprocess.call(command.split())
    return r

def try_install_fftw():
    script = os.path.join(directory,'fftw_install.sh')
    printAndRunCommand('chmod u+x '+script)
    return printAndRunCommand(script)

def install_fftw():
    print('downloading and installing fftw library')
    retry = True
    while retry:
        r = try_install_fftw()
        if r == 0:
            retry = False
        else:
            print('retry? (y/n)')
            retry = True if input() == 'y' else False


def try_compile_solutions(use_fftw):
    flags = ' -L'+os.path.join(directory,'fftw-3.3.9','lib') + ' -I'+os.path.join(directory,'fftw-3.3.9','include')+ ' -lfftw3f -lm' if use_fftw else ' -DDONT_USE_FFTW_LIB'
    returnCode = printAndRunCommand('g++ -std=c++11 -O3 '+os.path.join(directory,'benchmark.cpp') + ' -o '+os.path.join(directory,'benchmark') + flags)
    if returnCode != 0:
        return False
    returnCode = printAndRunCommand('g++ -std=c++11 -Og '+os.path.join(directory,'test_correctness.cpp') +' -o '+os.path.join(directory,'test_correctness') + flags)
    if returnCode != 0:
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
exit_code = printAndRunCommand(os.path.join(directory,'test_correctness') )
if exit_code != 0:
    print("some testcases failed!")
    exit(0)
printAndRunCommand(os.path.join(directory,'benchmark')+ ' 10 '+ os.path.join(directory,'data',''))

import pandas as pd
import matplotlib.pyplot as plt

def plotFromFile(name):
    df = pd.read_csv(os.path.join(directory,'data',name+'.csv'))
    xs = [float(x) for x in df.columns if x != 'name']
    labelToStyle = {
        'classic': ('blue',':'),
        'solution 1': ('orange',':'),
        'solution 2': ('olive','--'),
        'solution 3': ('purple','-.'),
        'solution 4': ('red','-'),
        'small u': ('brown', '-'),
        'small u single target': ('pink','--')
    }
    for i, row in df.iterrows():
        ys = [float(x) for x in row[1:]]
        plt.plot(xs,ys,label=row['name'],color=labelToStyle[row['name']][0], linestyle=labelToStyle[row['name']][1])
    plt.legend()
    plt.savefig(os.path.join(directory,'data','plot_'+str(name)+'.png'),dpi=200)
    plt.clf()

plotFromFile('t_time')
plotFromFile('smallu_classic')
plotFromFile('smallusingletarget_sol')
plotFromFile('n_time')
