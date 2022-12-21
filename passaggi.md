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
Il progetto usa libtool che non ho installato nel mio sistema:
sudo apt-get install libtool
Però compilo la libini in modo statico e libtool non mi serve in quanto viene usato runlib.
In configure.ac metto:
AC_PROG_RANLIB
AM_PROG_AR
Alla fine nella sottodirectory libini ho il file libini.a da linkare staticamente al
programma principale. Nota che ranlib e ar sono due programmi che servono per 
creare il file libini.a. Runlib sembra un alias di ar.

### Automake
Automake naturalmente evolve e per ogni versione bisogna stare attenti che
le macro cambiano di ruolo e di argomenti. Quindi bisogna sapere quale versione
si sta usando e come funzionano le macro.
Questo è il link che uso (versione 2.71):
https://www.gnu.org/software/autoconf/manual/autoconf-2.71/html_node/Initializing-configure.html
Questo per avere i parametri iniziali definiti poi in config.h (Nome, Versione, e-mail, tarball e URL).
Il file di partenza è configure.ac e sono partito da zero guardando il tutorial.
Ho messo le macro indispendabili come:
AC_INIT -> inizializza le info basilari
AM_INIT_AUTOMAKE -> comincia ad usarle con dei parametri di verbosità e tolleranza
AC_PROG_CXX -> definisce il compilatore c++
AC_CONFIG_FILES -> mette la lista dei makefiles che vengono creati a seconda di dove si trovano i sorgenti
AC_OUTPUT -> Esegue la creazione dei makefiles
Da qui ho aggiunto una macro per definire la versione (m4_define) e creare un configure.h
così che le definizioni del modulo siano disponibili anche nel codice (AC_CONFIG_HEADERS).

## Sviluppo in VS Code
Sono partito dalla versione linux, quindi in WSL lancio code (la liea di comando di VsCode) che mi setta l'ambiente
remoto WSL:Ubuntu-22.04. Il compilatore con automake, configure e make li uso da command line bash.
Per avere il goto definition in questo ambiente mi serve il path che setto creando un file
.vsode\c_cpp_properties.json. Tipo:
    "includePath": [
                "${workspaceFolder}/src/**",
                "${workspaceFolder}/src/libini/include",
                "/usr/include/SDL2"
            ]
Ho cambiato anche il task per avere la compilazione diretta da Visual con CTRL + SHIFT + B
Dalla bash arrivo all'editor premendo il tasto CTRL e spostando il mouse sulla linea della bash


## Pac card
Se ti meraviglia il fatto che il codice della versione 1.6 carica i files 
singoli delle carte, mentre lo zip distributivo uso il pac è dovuto ad un conflitto di versioni. 
Guardando bene lo zip 1.6 pubblicato per windows esso contiene l'exe nella versione 1.4 
che carica i pac. Perché? Per il solitario con le carte del tarocco piemontese.
Quindi anziché fare un nuovo pac con le carte del tarocco piemontese ed avere lo stesso programma che
carica diversi mazzi sempre in pac, ne ho fatto una nuova versione, la 1.6 che funziona solo con le
carte singole. Però poi solo con il nuovo mazzo, mentre tutti gli altri mazzi sono
rimasti in formato pac e quindi alla versione 1.4. 
Qui rimane il formato pac quello decisivo. 

## Error Handling
Non vorrei usare il throw del c++ perché non sono un gran fan. Nell'invido
e tressette l'ho però usato. Qui vorrei usare qualcosa di simile al clang. 
Un integer come riultato o meglio una struttura con tanto di codice e testo.