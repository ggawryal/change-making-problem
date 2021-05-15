#!/bin/bash
wget -nc www.fftw.org/fftw-3.3.9.tar.gz
if [ $? -ne 0 ]; then
    echo '[fftw-download script]: error, wget command failed, check internet connection'
    exit 1
else
    tar -xzvf fftw-3.3.9.tar.gz && cd fftw-3.3.9 && ./configure --enable-float && make
    if [ $? -ne 0 ]; then
        echo '[fftw-download script]: error in unpacking or configuring fftw library'
        exit 2
    else
        make install
        if [ $? -ne 0 ]; then
            echo '[fftw-download script]: error in fftw installation, check your permissions and try rerunning script with sudo'
            exit 3
        else
            echo '[fftw-download script]: succesfully downloaded and installed fftw library'
        fi
    fi
fi 
