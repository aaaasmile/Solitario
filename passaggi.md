# Solitario
Il Solitario è un programma in C++ che usa la libreria SDL 1.2. È stato scritto 
nel lontano 2004 dove ho compilato la versione per Linux e Windows.
Dopo 17 anni ho messo la repository su github e la compilazione è rimasta indietro.

## Compilazione per Ubuntu 22.04.1 LTS
Per le mie prove ho usato WSL su windows con la App Ubuntu 22.04.1 LTS.
    uname- r
    5.15.79.1-microsoft-standard-WSL2

Qui va aggiornato tutto il processo di autoconf, la libreria libini e SDL va portato a 2.0 
come per l'Invido e il Tressette.
Per prima cosa vedere qui per cambiare gli autoconf
https://www.gnu.org/software/automake/manual/automake.html#Modernize-AM_005fINIT_005fAUTOMAKE-invocation
Cambiato il file config.in in config.ac
Cambiato diverse macro nel file config.ac
Poi lanciato il comando:
    autoreconf --install
Se autoconf non è installato si usa:
    sudo apt-get install automake

Ho installato sdl con
    sudo apt install libsdl2-dev
Per le altre librerie sdl:
    sudo apt-get install libsdl2-image-dev
    sudo apt-get install libsdl2-mixer-dev

così per avere il comando:
    sdl-config --libs
ho messo il risultato direttamente nel file makefile.am

## SDL
Sembra che stia usando SDL2.0 in quanto uso SDL_UnlockSurface che è disponibile solo
dalla versione 2.0

### Libreria Libini
Il profilo viene caricato in windows usando il registry. Su Linux uso la libreria
libini che l'ho ritrovata qui:
https://sourceforge.net/projects/libini/
L'ho integrata direttamente nel codice in quanto non fa parte della distribuzione 
di ubuntu.

