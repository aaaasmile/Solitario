# Solitario
Il Solitario è un programma in C++ che usava la libreria SDL 1.2. È stato scritto 
nel lontano 2004 dove ho compilato la versione per Linux e Windows.
Dopo 18 anni ho messo la repository su github e la compilazione è rimasta indietro.
Così ho pensato di sviluppare la nuova versione con SDL2, Visual Code e WSL2 come target.

## Build Veloce (WSL2 e ubuntu)
Dopo il clone della repository, nella root directory del progetto:

    autoreconf --install
    ./configure
    make
    ./solitario

## Build veloce in MySys2

    mkdir Build
    cd Build
    cmake ..
    ninja

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
Il file libini.a, però, non viene installato ma semplicemente linkato. Il processo è indicato qui:
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
Ho messo le macro indispensabili come:

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
Sono partito dalla versione linux, quindi in WSL2 lancio

    code . 
(la linea di comando di VsCode) che mi setta l'ambiente
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
Nella nuova versione 2.0 le carte possono essere caricate in entrambi i formati (singolo e pac).
Visto che il formato singolo è superfluo uso solo il pac che ora include anche il tarocco piemontese.
Come ho creato il formato pac? Ho usato il progetto PacSprite https://github.com/aaaasmile/PacSprite/blob/main/passaggi.md.
Poi ho messo il risultato in data/mazzi.

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
Per fare il debug del solitario ho invece dovuto aggiungere un launch.json e 
nel task.jon ho messo il "Debug Build", che altro non è che il make standard visto
che aggiunge di default il parametro -g. Basta mettere un Breakpoint in main.cpp e 
premere F5 e gdb parte subito.
Sviluppato dentro questo progetto:

    igor@MiniToro:~/projects/Solitario$

## Defines
Nei sorgenti utilizzo le definizioni tipo DATA_PREFIX per puntare
alle directory dove si trovano i file ini e i files di risorsa per il programma.
A seconda del target, windows o unbuntu, queste directory sono differenti. 
È compito della configurazione definire in modo corretto, a seconda del target,
le variabili sopra elencate. Questo tipo di definizioni vanno messe anche nel 
file .vscode/c_cpp_properties.json per avere l'editor senza errori.
Per quanto riguarda la HOME directory, questa dipende dall'utente ed è una
variabile che setto in runtime (la ~ in compilazione non ha dato i risultati aspettati).

## Docker (host testato Windows)
Nella directory _container_ ho messo il file Dockerfile che uso per creare un'immagine 
da lanciare in windows. In Linux è meglio usare Apptainer (vedi sotto).
Per creare l'immagine Docker uso (powershell):

    docker build  -t solitario-ubuntu .
Il programma lo lancio poi con:

    docker run -it --rm -e DISPLAY=host.docker.internal:0 solitario-ubuntu
Nota che, al contrario di WSL2, occorre lanciare un X-server tipo Xming in Windows11.  
Per vedere il contenuto dell'immagine creata si può lanciare una bash con:

    docker run -it --rm --entrypoint bash -e DISPLAY=host.docker.internal:0 solitario-ubuntu
Per vedere se funziona il server X posso lanciare xclock che fa parte dell'immagine:

    docker run -it --rm --entrypoint xclock -e DISPLAY=host.docker.internal:0 solitario-ubuntu
Sul mio computer, però, ricevo quest'errore in docker:

    Error of failed request:  GLXUnsupportedPrivateRequest
    Major opcode of failed request:  143 (GLX)
    Minor opcode of failed request:  17 (X_GLXVendorPrivateWithReply)
    Serial number of failed request:  85
    Current serial number in output stream:  85

## Apptainer Installazione
Un altro tipo di Container è Apptainer che crea immagini più piccole di Docker.
Installazione di Apptainer in Ubuntu 22.04 (nativo e WSL2)

    wget https://github.com/apptainer/apptainer/releases/download/v1.1.5/apptainer_1.1.5_amd64.deb
    sudo apt-get install libfuse2 uidmap squashfuse fuse2fs fuse-overlayfs libsquashfuse0 libfuse3-3 fuse3
    sudo dpkg -i apptainer_1.1.5_amd64.deb
    sudo apptainer config fakeroot --add $USER
    sudo apptainer config fakeroot --enable $USER

## Apptainer (host testato con successo Linux e anche su WSL2)
Nella directory _container_ ho messo il file solitario-ubuntu-22.04.def che mi è stato
gentilmente messo a disposizione per creare un file sif. Si crea con:

    apptainer build --fakeroot solitario-ubuntu-22.04.sif solitario-ubuntu-22.04.def
Si esegue con

    apptainer run --bind /run solitario-ubuntu-22.04.sif
Per avere una shell nel container si usa:
     apptainer shell lolcow_latest.sif
     apptainer shell solitario-ubuntu-22.04.sif

## Apptainer (host Windows con WSL2)
L'installazione di Apptainer di sopra ha funzionato senza problemi.
Il display invece è stato un po' difficoltoso. Nella file def ho aggiunto le x11-apps per
provare se un'applicazione tipo _xclock_ funziona.

Occorre lanciare Xming, l'XServer integrato in WSL2 non riesco a farlo andare.
Xming va lanciato con l'opzione "No Access Control" selezionato.
Per poi provare se funziona il display, ho usato 
    export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2; exit;}'):0
    apptainer shell solitario-ubuntu-22.04.sif
    xclock

Anche qui se lancio il solitario ottengo lo stesso errore di Docker

    igor@MiniToro:~/apptainer/my-try$ apptainer run --bind /run solitario-ubuntu-22.04.sif
    X Error of failed request:  GLXUnsupportedPrivateRequest
        Major opcode of failed request:  143 (GLX)
        Minor opcode of failed request:  17 (X_GLXVendorPrivateWithReply)
        Serial number of failed request:  85
        Current serial number in output stream:  85

## Help
Il file Pdf dell' help l'ho generato usando Context. I file sorgenti tex si trovano
nella sottodirectory help-source. Questo il dettaglio di Context:

    mtx-context     | ConTeXt Process Management 1.04
    mtx-context     |
    mtx-context     | main context file: D:/Projects/ConTex/tex/texmf-context/tex/context/base/mkiv/context.mkiv
    mtx-context     | current version: 2022.11.14 22:58
    mtx-context     | main context file: D:/Projects/ConTex/tex/texmf-context/tex/context/base/mkxl/context.mkxl
    mtx-context     | current version: 2022.11.14 22:58
Per la visualizzazione dell'help su ubuntu uso zathura.
ConText lo faccio partire con SciTe da: 

    D:\Projects\ConTex\prepare_context.ps1
In Scite apro il file solitario.tex e genero il file pdf con CTRL + F7.

## Target Windows

## CMake
Dopo avere sviluppato l'intero progetto su WSL2 con Ubuntu è arrivato il momento di avere anche il target
Windows, nel mio caso MySys2. Per questo voglio provare CMake e non cercare di fare andare Automake
per entrambi target.
Si parte dal file CMakeLists.txt nella root del progetto. In Visual Code ho installato l'extension di Microsoft 
per CMake. 
Per lanciarlo uso MySys con la sequenza:
```sh
# Create a build folder
mkdir build
cd build

# Build
cmake ..
ninja
```

Invece di usare il solito make ho voluto provare un suo successore, vale a dire ninja. Si installa con (ricorda che uso solo ucrt):
```sh
pacman -S mingw-w64-ucrt-x86_64-ninja
ninja 
```
Uso un solo file CMakeLists.txt che contiene tutti i sorgenti compresa la libreria libini. Al momento
supporto solo il target MySys che definisce WIN32. L'unica funzione che da dei problemi è mkdir.
Nota che se cambio la lista dei files o un linker, non ho bisogno di rilanciare cmake.
Quello che mi manca è l'integrazione in VsCode con diversi target per la produzione, il debugger e i TRACE.
Infatti quando installo l'extension di cmake, non usa ninja, ma mingw make che non è corretto.
Il comando da usare, quando si cambia il file CMakeList.txt nella root, è quello di andare nella
dir build e lanciare cmake

    cd build
    cmake ..

## Visual Code in Windows
Il file c_cpp_properties.json è nella repository, quindi viene usato sia in WSL2 che in windows.
Ci sono due configurazioni: win32 e linux. Sono due nomi riservati che vengono selezionati automaticamente.
Esse servono per avere un editor con intellisense ed errori appropriati in quanto i folder delle
librerie nei due ambienti sono piazzate in paths differenti. Se non viene selezionato in modo 
corretto, nella statusbar in basso a destra lo si può selezionare manualmente.
Nella Statusbar in fondo è anche possibile settare anche tutti i vari target che ninja riesce a
compilare nella directory build. Nella mia configurazione iniziale però ho solo  [all] e 
dovrei aggiungere anche debug. In ogni modo il debugger con F5 parte subito in modo corretto
senza nessun task aggiuntivo come ho su Ubuntu. In Launch si tratta di avere il programma solitario.exe
con il suo path e gdb. Non mi funziona CTRL + SHIFT + B in quanto esegue make per ubuntu, ma, invece,
oltre alla powershell con ninja in build, mi funziona la icona Build nella statusbar

### Debugger della versione Msys2
Basta semplicemente premere la toolbar di sinistra sull'icona del debug e selezionare il progetto
"Debug MySys Win". Nota che la configurazione si trova nel file launch.json. La configurazione "(gdb) Launch"
è usata per WSL2.  

### Per partire con Visual Code su Windows (Msys2)
Per compilare il solitario in windows, che ha come target windows e le dll di msys2, bisogna installare
il sistema msys2. Come riferimento posso vedere le note di https://github.com/aaaasmile/HelloSDL/blob/main/passaggi.md
Ho fatto un download del programma https://www.msys2.org/ e poi nella shell ho messo i seguenti comandi:

    pacman -Syu   (=> qui ho fatto un restart della shell)
    pacman -S mingw-w64-ucrt-x86_64-gcc
    pacman -S mingw-w64-ucrt-x86_64-gtk2
    pacman -S mingw-w64-ucrt-x86_64-make
    pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
    pacman -S git mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
    pacman -S mingw-w64-ucrt-x86_64-gtest mingw-w64-ucrt-x86_64-giflib
    pacman -S mingw-w64-ucrt-x86_64-libpng mingw-w64-ucrt-x86_64-libjpeg-turbo
    pacman -S mingw-w64-ucrt-x86_64-SDL2
    pacman -S mingw-w64-ucrt-x86_64-SDL2_net
    pacman -S mingw-w64-ucrt-x86_64-SDL2_mixer
    pacman -S mingw-w64-ucrt-x86_64-SDL2_image
    pacman -S mingw-w64-ucrt-x86_64-SDL2_ttf
    pacman -S mingw-w64-ucrt-x86_64-SDL2_gfx
Nota che è un'installazione che consente di compilare anche il sistem ConTex.
Ora posso lanciare Visual Code con il path per compilare il solitario usando lo script .\start_code.ps1
che lancia Visual Code con il path di msys2 corretto. Nota che non metto msys2 nel path di windows per
la semplice ragione che uso versioni differenti del compilatore g++ e la sua toolchain a seconda del 
linguaggio di programmazione e target (esempio in golang uso C:\TDM-GCC-64)

### TODO
- windows setup
    - suono del main loop del game è wings-of-the-wind.ogg
    - Il file exe dovrebbe avere un altra icona
    - Il deploy così com'è non va bene. Le dll e l'exe devono provenire dalla directory build.
        Qui occorre un filtro per non inglobare anche i files di CMake.  [DONE]
    - Il link della guida nell'installazione è sbagliato [DONE]
- Score (http://www.solitairecity.com/Help/Klondike_Scoring.shtml)
    - Menu "Punteggi" [DONE]
    - Salva i dieci migliori punteggi con la data e il nome dell'utente nel file ini e il numero delle carte (56 o 40) [DONE]
    - Spiega il punteggio nell'help [DONE]
    Good Score
        - Playing a card from stock to tableau (+45) [DONE]
        - Turning over a face down tableau card (+25) [DONE]
        - Transferring a card to the foundations (+60) [DONE]
    Bad Score
        - Moving a foundation card back to the tableau (-75) [DONE]
        - Each re-deal after the first pass through the stock (-175) [DONE]
        - Every second of elapsed time (-1) [DONE]
    - Quando finisce si mostra il punteggio prima di cominciare una nuova partita [DONE]
- Domanda (msgbox) quando vuole uscire o cominciare una nuova partita [DONE]
- Merge con Italian Special Solitaire [DONE]
- Rimuovere in git i file automake generati [DONE]
- In windows manca il settaggio del path dove si trova l'exe per avere il path completo
  del caricamento delle resources. [DONE]
- Deployment su Windows [DONE]
- Dialogo delle opzioni da completare (mazzo di carte) [DONE]
- Goto invido.it [DONE]
- Le opzioni di SDL2 dovrebbero risultare dal processo di configure. Uso CMake. [DONE]
- Sfondo delle opzioni non è centrato [DONE]
- Nei credits vorrei la musica water [DONE]
- Help [DONE]
- Le opzioni cambiate vanno scritte nell'options.ini dell'utente [DONE]
- Draw del background non contiene i bordi neri. Il drag della prima foundation è sporco [DONE]
- Il drag parte da x,y = 0,0 [DONE]
- Il menu ha ancora bisogno di correzioni sdl 1.2 -> sdl 2.0 [DONE]
- Il rendering del gioco non usa le immagini in formato Pac ma quelle singole [DONE]

### TODO Nice To Have
    - Il codice taget di supporto del trace service si può implementare una sola volta mentre su ogni target
        si implementa solo la configurazione
    - Score potrebbe essere un componente gfx
    - Effetto sonoro quando si manda una carta nella pila degli assi e magari alla fine del drag

