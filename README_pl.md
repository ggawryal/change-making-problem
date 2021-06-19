# Problem wydawania reszty
Zbiór algorytmów do [problemu wydawania reszty](https://en.wikipedia.org/wiki/Change-making_problem), napisanych w C++ z wykorzystaniem biblioteki [FFTW](http://www.fftw.org).

## Opisy algorytmów
-  T. M. Chan and Q. He: **On the Change-Making Problem** https://tmc.web.engr.illinois.edu/coin_sosa.pdf
-  T. M. Chan and Q. He: **More on Change-Making and Related Problems** https://drops.dagstuhl.de/opus/volltexte/2020/12895/

## Uruchomienie
Uruchom skrypt ```run.py``` w Python3. Przy pierwszym uruchomieniu skrypt zapyta, czy ma pobrać bibliotekę FFTW. Po otrzymaniu pozytywnej odpowiedzi skrypt pobierze, skonfiguruje i zainstaluje bibliotekę w lokalnym katalogu.
Następnie, skrypt skompiluje i uruchomi plik ```test_correctness.cpp```, który sprawdza poprawność algorytmów, oraz plik ```benchmark.cpp``` mierzący czasy wykonania każdego z rozwiązań na różnych generatorach testów.
Po zakończeniu, skrypt ```run.py``` naszkicuje wykresy dla każdego rozwiązania i zapisze je w katalogu ```data/```.

Alternatywnie, można nie korzystać z biblioteki FFTW - w momencie, gdy skrypt zapyta, czy chcemy ją zainstalować, wystarczy wcisnąć (n). W tym wypadku rozwiązania będą korzystały z własnej implementacji szybkiego przekształcenia Fouriera, które jest
kilka razy wolniejsze od tego z biblioteki. 


W katalogu ```data/``` znajdują się przykładowe wykresy oraz pliki z informacjami o czasach wykonania każdego rozwiązania, wygenerowane na komputerze z procesorem Intel(R) Core(TM) i5-7300HQ CPU  @ 2.50GHz, z wykorzystaniem biblioteki FFTW,
uśrednione na 10 uruchomieniach. Domyślna liczba uruchomień wynosi 1 i można ją zwiększyć, przekazując flagę ``--runs=`` na przykład ```python3 run.py --runs=10```.

## Wymagania
- g++ ze wsparciem c++11,
- python3 z bibliotekami matplotlib i pandas (można uruchomić ```pip install -r requirements.txt```)

Dodatkowo, skrypt ```run.py``` potrafi zainstalować bibliotekę FFTW tylko na systemach \*nix, dla reszty systemów (np. Windows) należy ją [ręcznie zainstalować](http://www.fftw.org/fftw3_doc/Installation-on-non_002dUnix-systems.html#Installation-on-non_002dUnix-systems) 
(przy konfigurowaniu tej biblioteki należy przekazać flagę --enable-float, por. plik ```fftw-install.sh```) lub nie korzystać z niej wcale.
