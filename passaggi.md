# Solitario
Il Solitario è un programma in C++ che usava la libreria SDL 1.2. È stato scritto 
nel lontano 2004 dove ho compilato la versione per Linux e Windows.
Dopo 18 anni ho messo la repository su github e la compilazione è rimasta indietro.
Così ho pensato di sviluppare la nuova versione con SDL2, Visual Code e WSL2 come target.

## Build Veloce
Dopo il clone della repository, nella root directory del progetto:

    autoreconf --install
    ./configure
    make
    ./solitario

## Compilazione per Ubuntu 22.04.1 LTS
Per le mie prove ho usato WSL2 su windows con la App Ubuntu 22.04.1 LTS.

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
    sudo apt-get install libsdl2-ttf-dev

così per avere il comando:

    sdl2-config --libs
ho messo il risultato direttamente nel file makefile.am

## SDL
Il programma è stato scritto originariamente per SDL1.2. In questa
nuova versione ho fatto il porting per SDL2.0.

### Libreria Libini
Il profilo viene caricato in windows usando il registry. Su Linux uso la libreria
libini che l'ho ritrovata qui:
https://sourceforge.net/projects/libini/.
L'ho integrata direttamente nel codice in quanto non fa parte della distribuzione 
di ubuntu.
La _libini_ in è compilata in modo statico e libtool non mi serve in quanto viene usato runlib.
In configure.ac metto:

    AC_PROG_RANLIB
    AM_PROG_AR
Alla fine nella sottodirectory libini ho il file _libini.a_ da linkare staticamente al
programma principale. Nota che ranlib e ar sono due programmi che servono per 
creare il file libini.a. _Runlib_ sembra un alias di _ar_.
Nota che la libreria nel progetto ha un proprio target e make file.
Il file libini.a, peró, non viene installato ma semplicemente linkato. Il processo è indicato qui:
_(concept of a "convenience library", which is an intermediate target that is never itself installed, 
but contains object code that can be linked into an executable or a larger library_.

=> Preso da https://stackoverflow.com/questions/58359548/how-to-write-automake-files-to-recursive-build-subdirectories 
un link che mi ha fatto creare il file finale e spiegato un po' il funzionamento di automake).

## Automake
Automake naturalmente evolve e per ogni versione bisogna stare attenti che
le macro cambiano di ruolo e di argomenti. Quindi bisogna sapere quale versione
si sta usando e come funzionano le macro.
Questo è il link che uso (versione 2.71):
https://www.gnu.org/software/autoconf/manual/autoconf-2.71/html_node/Initializing-configure.html.
Questo per avere i parametri iniziali definiti poi in Config.h (Nome, Versione, e-mail, tarball e URL).
Il file di partenza è configure.ac e sono partito da zero guardando il tutorial.
Ho messo le macro indispendabili come:

    AC_INIT -> inizializza le info basilari
    AM_INIT_AUTOMAKE -> comincia ad usarle con dei parametri di verbosità e tolleranza
    AC_PROG_CXX -> definisce il compilatore c++
    AC_CONFIG_FILES -> mette la lista dei makefiles che vengono creati a seconda di dove si trovano i sorgenti
    AC_OUTPUT -> Esegue la creazione dei makefiles.

Da qui ho aggiunto una macro per definire la versione (m4_define) e creare un Configure.h
così che le definizioni del modulo siano disponibili anche nel codice (AC_CONFIG_HEADERS)
e linkare staticamente del codice come libini.

### Automake Makefile
Ho organizzato il mio codice in diverse directory (libini e src). 
Il Makefile nella root mi serve per definire il target. La libini ha un suo makefile.
Quello nella subdir ./libini contiene la libreria ini per creare il file libini.a
Entrambi Makefile hanno il loro posto nella macro AC_CONFIG_FILES  del file configure.ac.
Nota che per compilare i files tipo src/main.cpp ho dovuto mettere nella macro 
AM_INIT_AUTOMAKE l'opzione subdir-objects.
Avevo provato a mettere il makefile del target nella sottodirectory src, ma non
volevo il risultato in quella directory. Molto più facile è avere il target
dove risiede il Makefile.

## Sviluppo in VS Code
Sono partito dalla versione linux, quindi in WSL lancio

    code . 
(la liea di comando di VsCode) che mi setta l'ambiente
remoto WSL:Ubuntu-22.04. Il compilatore con _automake_, _configure_ e _make_ li uso da command line bash.
Per avere il goto definition in questo ambiente mi serve il path che setto creando un file

    .vsode\c_cpp_properties.json. Tipo:
        "includePath": [
                    "${workspaceFolder}/src/**",
                    "${workspaceFolder}/src/libini/include",
                    "/usr/include/SDL2"
                ]
Ho cambiato anche il task per avere la compilazione diretta da Visual con CTRL + SHIFT + B
Dalla bash arrivo all'editor premendo il tasto CTRL e spostando il mouse sulla linea della bash

## Pac Card
Se ti meraviglia il fatto che il codice della versione 1.6 carica i files 
singoli delle carte mentre lo zip distributivo usa il pac è perché esiste un conflitto di versioni. 
Guardando bene lo zip 1.6 pubblicato per windows esso contiene l'exe nella versione 1.4 
che carica i pac. Perché? Per far funzionare il solitario con le carte del tarocco piemontese.
Quindi, anziché fare un nuovo pac con le carte del tarocco piemontese ed avere lo stesso programma che
carica diversi mazzi sempre in pac, ho fatto una nuova versione, la 1.6, che funziona solo con le
carte singole. Però solo con il mazzo del Tarocco Piemontese, mentre tutti gli altri mazzi sono
rimasti in formato pac e quindi hanno bisogno dell'eseguibile alla versione 1.4. 
Qui rimane il formato pac quello decisivo. 
Nella nuova versione 1.7 le carte possono essere caricate in entrambi i formati (singolo e pac).

## Error Handling
Non vorrei usare il throw del c++ perché non mi convince. Nell'Invido
e Tressette, però, l'ho usato in mancanza di meglio. Qui vorrei usare qualcosa di simile al clang. 
Il risultato è la struttura messa in _ErrorInfo.h_, vale a dire una struttura (puntatore) che viene 
ritornata da una funzione. Se il valore non è nullo, allora è avvenuto un errore e il chiamante
è obbligato a gestire l'errore. Il Throw risparmia il codice per ritornare l'errore alla
funzione chiamante, ma perde l'handling particolare della gerarchia delle funzioni. Alla
fine vuol dire che il il gestore dell'errore non sa da dove proviene l'errore, non sempre sa che cosa fare
ed è possibile che qualche puntatore rimanga in bilico (memory leak).

## Sviluppo e Test in WSL2
Per quanto ne ho capito, nel dicembre 2022 WSL2 sembra già avere un X server´integrato.
Basta provare con Xclock, che una volta installato parte senza altri server.
Per vedere se funziona un programma SDL2 nella WSL2 basta provare a clonare la
repository https://github.com/aminosbh/falling-brick-game
e seguire le istruzioni per compilare ed eseguire il programma. Nel mio caso ha funzionato.
Per fare il debug del solitario ho invece dovuto aggiungere un lauch.json e 
nel task.jon ho messo il "Debug Build", che altro non è che il make standard visto
che aggiunge di default il parametro -g. Basta mettere un Breakpoint in main.cpp e 
premere F5 e gdb parte subito.
Sviluppato dentro questo progetto:

    igor@MiniToro:~/projects/Solitario$

### TODO
- Il drag parte da x,y = 0,0
- Il menu ha ancora bisogno di correzzioni sdl 1.2 -> sdl 2.0 [DONE]
- Il rendering del gioco non usa le immagini in formato Pac ma quelle singole [DONE]


