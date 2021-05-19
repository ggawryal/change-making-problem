#!/bin/bash
wget -nc www.fftw.org/fftw-3.3.9.tar.gz
if [ $? -ne 0 ]; then
    echo [fftw-download script]: error, wget command failed, check internet connection
    exit 1
else
	tar -xzvf fftw-3.3.9.tar.gz && cd fftw-3.3.9 && ./configure --enable-float --prefix="$(pwd)" && make && make install
    if [ $? -ne 0 ]; then
        echo [fftw-download script]: error in unpacking or installing fftw library
        exit 2
    else
        echo [fftw-download script]: succesfully downloaded and installed fftw library
    fi
fi 
