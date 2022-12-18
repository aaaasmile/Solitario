Per prima cosa vedere qui per cambiare gli autoconf
https://www.gnu.org/software/automake/manual/automake.html#Modernize-AM_005fINIT_005fAUTOMAKE-invocation
Cambiato il file config.in in config.ac
Cambiato diverse macro nel file config.ac
Poi lanciato il comando:
 autoreconf --install
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
L'ho trovata qui:
https://sourceforge.net/projects/libini/

