Per prima cosa vedere qui per cambiare gli autoconf
https://www.gnu.org/software/automake/manual/automake.html#Modernize-AM_005fINIT_005fAUTOMAKE-invocation
Cambiato il file config.in in config.ac
Cambiato diverse macro nel file config.ac
Poi lanciato il comando:
 autoreconf --install
Ho installato sdl con
sudo apt install libsdl1.2-dev

così per avere il comando:
sdl-config --libs