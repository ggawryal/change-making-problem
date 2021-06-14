import subprocess
import os
import sys

directory = os.path.dirname(os.path.realpath(__file__))
number_of_runs_on_same_testgenerator = 1
for arg in sys.argv[1:]:
    if arg.startswith("--runs="):
        try:
            number_of_runs_on_same_testgenerator = int(arg[7:])
        except:
            print("wrong format of 'runs' argument, it should be for example --runs=10")
            exit(4)


def printAndRunCommand(command):
    print(command)
    r = subprocess.call(command.split(),shell=(True if os.name == 'nt' else False)) #shell=True on Windows, False on *nix
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
printAndRunCommand(os.path.join(directory,'benchmark')+ ' '+str(number_of_runs_on_same_testgenerator)+' '+ os.path.join(directory,'data',''))


import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import rcParams

rcParams['font.family'] = 'serif'
rcParams['font.serif'] = ['Times New Roman']
rcParams['mathtext.fontset'] = 'cm'

def plotFromFile(name, log_scale = False):
    df = pd.read_csv(os.path.join(directory,'data',name+'.csv'))
    x_name = df.columns[0]
    xs = [float(x) for x in df.columns[1:]]
    labelToStyle = {
        'classic': ('$O(nt)$, klasyczne','blue',':'),
        'solution 1': ('$O(t\ \log^3 t)$','orange',':'),
        'solution 2': ('$O(t\ \log t\ \log w)$','olive','--'),
        'solution 3': ('$O(t\ \log^2 t)$, lemat o podziale','purple','-.'),
        'solution 4': ('$O(t\ \log t\ \log \log t)$','red','-'),
        'small u': ('$O(u^2 \log u)$','brown', '-'),
        'small u single target': ('$\widetilde O(u)$','pink','--')
    }
    for i, row in df.iterrows():
        if not str(row.iloc[0]).startswith("avg_"):
            continue
        
        curve_name = str(row.iloc[0])[4:]
        style = labelToStyle[curve_name]
        
        avg_time = [float(x) for x in row[1:]]
        stddev = [float(x) for x in df.loc[df[x_name] == 'stddev_'+curve_name].iloc[0][1:]]
        plt.errorbar(xs,avg_time,stddev,label=style[0],color=style[1], linestyle=style[2],elinewidth=0.8,mew=1,capsize=1)


    if log_scale:
        plt.xscale('log')
    else:
        plt.ticklabel_format(axis="x",style='plain')
    plt.xlabel(x_name)
    plt.ylabel('czas [s]')
    plt.legend()
    plt.savefig(os.path.join(directory,'data','plot_'+str(name)+'.pdf'))
    plt.clf()

for i in (1,2,3,4):
    plotFromFile('t_time'+str(i))
    plotFromFile('n_time'+str(i))
    plotFromFile('smallusingletarget_sol'+str(i))
    plotFromFile('smallu_classic'+str(i))

plotFromFile('sol2_vs_4_big_answer',log_scale=True)
