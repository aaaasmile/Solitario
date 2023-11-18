# WSL-2 Solitario Android

Vorrei provare a compilare il solitario, sviluppato con SDL-2, per Android.
Per questo uso WSL-2 e non vorrei installare Android Studio.
Il primo link che ho trovato è questo di SDL-2:

https://wiki.libsdl.org/SDL2/Android#install_sdl_in_a_gcc_toolchain

Da qui ho installato:
sudo apt install openjdk-11-jdk ant android-sdk-platform-tools-common

Siccome uso WSL-2 uso come referenza anche il link: https://halimsamy.com/wsl-for-developers-installing-the-android-sdk
sudo apt install gradle


Vediamo di installare i tools, dove il link si trova su https://developer.android.com/studio#downloads
nella sezione command links only.
Lo zip per Linux l'ho scaricato col browser e l'ho copiato nella directory \\wsl.localhost\Ubuntu-22.04\tmp
Poi nella WSL:

	cd ~ # Make sure you are at home!
	mkdir -p android/cmdline-tools
	unzip -q -d android/cmdline-tools /tmp/cmd-tools.zip
	mv android/cmdline-tools/cmdline-tools android/cmdline-tools/latest
	rm /tmp/cmd-tools.zip # delete the zip file (optional)

Ho settato delle variabili per lo sviluppo con android-33

	export ANDROID_HOME=$HOME/android
	export ANDROID_SDK_ROOT=${ANDROID_HOME}/platforms/android-33
	export PATH=${ANDROID_HOME}/cmdline-tools/latest/bin:${ANDROID_HOME}/platform-tools:${ANDROID_HOME}/build-tools/33.0.2:${ANDROID_HOME}/ndk-bundle:${PATH}
	
Siccome ho scaricato commandlinetools-linux-9477386_latest.zip al momento di lanciare
	
	yes | sdkmanager --licenses
	
Mi ha dato un errore di java unsupported version. Ci vuole la versione 11 e non la 8.

	sudo apt install openjdk-11-jdk
	export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64
	
Ora scarico l'sdk, che al momento (2023.06.03) è la 33.0.2

	sdkmanager "platforms;android-33" "build-tools;33.0.2"
	sdkmanager "platforms;android-22" "build-tools;22.0.1"
	sdkmanager "platforms;android-24" "build-tools;24.0.3"
Per vedere quali sono le versioni di build-tools basta vedere la release note su: https://developer.android.com/tools/releases/build-tools

Ho scaricato l'sdk 22 in quanto ho delle difficoltà ad usare il mio samsumg se uso android-33 
Per l'sdk 22 occorre anche (altrimenti non funziona aapt):

	sudo apt-get install lib32stdc++6 lib32z1

Per vedere la lista delle piattaforme si usa:

	sdkmanager --list
Questi comandi li ho aggiunti di mio

	sdkmanager "platform-tools"
	sdkmanager "ndk-bundle"
	
Il comando sdkmanager "tools" scarica gli emulatori che non ho intenzione di usare.

## Samsumg J320f
Mi piacerebbe usare il vecchio Samsumg J320f per provare qualche applicazione apk. 
Questo dispositivo, che ho acquistato su Amazon il febbraio 2017 per 130 euro, ha 1,5 GB RAM, 32 GB
di memoria interna e Android 5.1. Le api level di questo dispositivo è la 22. Avendo installato la 
33 posso creare un app per Android 5.1 se metto come minimo livello l'API level 22.
Per una lista di tutti i livelli ed a quale versione di Android corrisponde, vedi questo link:
https://source.android.com/docs/setup/start/build-numbers?hl=de
Da quello che ho capito, il massimo sistema operativo installabile su Samsumg J320f è il 7.1 Nougart,
ovvero l'API level 25. 
Flutter dovrebbe funzionare su questo dispositivo in quanto il supporto minimo è l'API level 16  

### SDL2 e J320f
Per quanto riguarda SDL2, la versione da usare per questo smartphone è la 2.0.8 come indicato 
nei pre-requisiti:
>=19 for SDL >= 2.0.8
La versione 2.0.16 vuole già un api level di 26.
In \\wsl.localhost\Ubuntu-22.04\home\igor\scratch\android\sdl-source ho messo il 
sorgente di SDL.2.0.8 preso dal tag github per vedere se riesco a fare andare un hello world con SDL.
Url di riferimento è: https://wiki.libsdl.org/SDL2/Android#install_sdl_in_a_gcc_toolchain

### Collegare J320f con WSL2
La url di riferimento è https://halimsamy.com/wsl-for-developers-connect-usb-devices
Bisogna usare usbipd che si installa con powershell e il comando:
winget install usbipd
usbipd url di riferimento è: https://github.com/dorssel/usbipd-win
Dopo ho fatto un restart di windows
Ora in powershell riesco a lanciare il comando:

	usbipd wsl list
	BUSID  VID:PID    DEVICE                                                        STATE
	2-4    04e8:6860  Samsung Galaxy J3 (2016), USB Serial Device (COM4)            Not attached
Poi apro Ubuntu in un tab parallelo a powershell e in powershell (come admin) lancio

	usbipd wsl attach --busid 2-4
Ora  in ubuntu al comando lsusb ottengo:
	
	Bus 001 Device 002: ID 04e8:6860 Samsung Electronics Co., Ltd Galaxy A5 (MTP)

Per il detach si usa

	usbipd wsl detach --busid 2-4
	
Se ci sono dei problemi, tipo 
usbip: error: Attach Request for 2-4 failed - Device busy (exported)
si usa

	usbipd unbind --busid 2-4
e poi si rifà il attach
Affinché funzioni in date explorer deve comparire il device. Può darsi che si debba usare l'eject usb
nella windows toolbar e poi fare un ricollegamento col cavo.
Dopo un restart di windows il telefono va scollegato e collegato quando si fa l'attach da powershell

### Collegare Redmi Note 11
Questo dispositivo, che ho acquistato su Amazon in novembre 2023 per 147 euro, ha 4 GB RAM, 128 GB
di memoria interna, così come un processore arm64 v8.
In Powershell
winget install usbipd
Poi faccio ripartire Powershell ed ottengo:
> usbipd wsl list
5-1    2717:ff08  Redmi Note 11

in admin powershell
usbipd wsl attach --busid 5-1 --distribution UbuntuMinitoro

Al primo tentativo ho ricevuto il seguente errore:
usbipd: error: WSL 'usbip' client not correctly installed. See https://github.com/dorssel/usbipd-win/wiki/WSL-support for the latest instructions.
L'ho corretto seguendo le istruzioni del link, che sono (uso UbuntuMinitoro dove ho Android e lo scratch di SDL2):
sudo apt-get update
sudo apt-get upgrade
sudo apt install linux-tools-virtual hwdata
sudo update-alternatives --install /usr/local/bin/usbip usbip `ls /usr/lib/linux-tools/*/usbip | tail -n1` 20
In Powershell ora il comando:
usbipd wsl attach --busid 5-1 --distribution UbuntuMinitoro
funziona senza problemi.
In WSl il comando lsusb genera:
Bus 001 Device 002: ID 2717:ff08 Xiaomi Inc. Redmi Note 3 (ADB Interface)
	
### Hello World primo tentativo
Sono partito usando la platform 33 per poi retrocedere su piattaforme passate per via d'usare
tools che erano documentati. Alla fine, vedi sezione successiva, sono riuscito a fare andare 
Hello World usando la platform 33. 
Variabili di sistema da settare prima di partire:
	
	
	export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64
	export ANDROID_HOME=$HOME/android
	export ANDROID_SDK_ROOT=${ANDROID_HOME}/platforms/android-24
	export PATH=${ANDROID_HOME}/cmdline-tools/latest/bin:${ANDROID_HOME}/platform-tools:${ANDROID_HOME}/build-tools/24.0.3:${ANDROID_HOME}/ndk-bundle:${PATH}
	export PROJ=$HOME/scratch/android/hello	
	
A quanto pare serve java 1.8 e non 1.11 per android 24. In ogni modo sono riuscito a fare andare hello.apk
usando l'alternativa (vedi punto 13 sotto), ma usa un tool ApkBuilderMain che è deprecated.
Sarebbe interessante vedere se con java 1.8 (vedi sotto per lo switch) 
Oppure la versione con d8 e appbuilder messa in StackOverflow alla fine.
	
Voglio provare un Hello World a linea di comando senza usare andorid studio.
Per questo  seguo il link https://stackoverflow.com/questions/29178552/hello-world-using-the-android-sdk-alone-no-ide
Il post su StackOverflow è datato e considero anche 
https://authmane512.medium.com/how-to-build-an-apk-from-command-line-without-ide-7260e1e22676
che anch'esso è datato in quanto dx non c'è più ma è comparso d8.

Il codice lo edito con code all'interno di WSL.

creo una nuova directory hello qui sotto che setto nella variabile PROJ
export PROJ=$HOME/scratch/android/hello

Parto dal punto 9 in quanto i punti precedenti li ho già eseguiti.
mkdir res
mkdir obj
mkdir bin

Il punto 10 uso il mio SDK  che ho installato prima (che a questo punto non fa nulla in quanto res è vuota):
aapt package -f \
  -I ${ANDROID_SDK_ROOT}/android.jar \
  -J src -m \
  -M AndroidManifest.xml -S res -v
  
Punto 11
javac -d obj -bootclasspath ${ANDROID_SDK_ROOT}/android.jar  -classpath src -source 1.7 -target 1.7 src/dom/domain/*.java
ora in 
  
Punto 12 (non riesco a capirlo quindi lo salto)
Con l'sdk 24
dx --dex --output=$PROJ/bin/classes.dex $PROJ/obj

Con l'sdk 33 (con una sequenza non ancora esatta per Samsung J320f)
	Nota che nella mia platform 33 dx ora si chiama d8
	Questo comando non funziona:
	d8 --output=$PROJ/bin/classes.dex $PROJ/obj
	Questo va meglio
	d8 --output $PROJ/bin $PROJ/obj/dom/domain/SayingHello.class
	ma genera l'errore 
	Type `android.app.Activity` was not found, it is required for default or static interface methods desugaring
	Quindi provo:
	d8 --output $PROJ/bin $PROJ/obj/dom/domain/SayingHello.class ${ANDROID_SDK_ROOT}/android.jar
	d8 --output $PROJ/bin --lib $ANDROID_PLATFORM/android.jar $(find . -name "*.class")
	che mi genera il mio classes.dex

Punto 13
aapt package -f \
  -f -m -F $PROJ/bin/hello.unaligned.apk \
  -I ${ANDROID_SDK_ROOT}/android.jar \
  -M AndroidManifest.xml -S res -v


aapt add $PROJ/bin/hello.unaligned.apk $PROJ/bin/classes.dex

A questo punto il comando:
aapt list $PROJ/bin/hello.unaligned.apk
Mi mostra il seguente risultato:
AndroidManifest.xml
/home/igor/scratch/android/hello/bin/classes.dex
Da cui si vede che il mio hello.unaligned.apk è composto di due files: il dex e il manifest.

* Alternativa che funziona!!! - inizio - *
NOTA: per usare questa alternativa ho dovuto usare java 8 che ho fatto lo switch con:
	sudo update-alternatives --config javac
	sudo update-alternatives --config java

aapt package -f -F $PROJ/bin/app.apkPart -I $ANDROID_SDK_ROOT/android.jar -M AndroidManifest.xml -S res -v

CLASSPATH=$ANDROID_TOOLS/lib/* java  \
com.android.sdklib.build.ApkBuilderMain $PROJ/bin/app.apkUnalign \
-d -f $PROJ/bin/classes.dex -v -z $PROJ/bin/app.apkPart

zipalign -f -v 4 $PROJ/bin/app.apkUnalign  $PROJ/bin/hello.apk
adb install -r $PROJ/bin/hello.apk
adb shell am start -n dom.domain/.SayingHello

* Fine alternativa*

Punto 14 lo salto e vado al punto 15
(va fatta la signature prima, vedi sotto)
zipalign -f 4 $PROJ/bin/hello.unaligned.apk $PROJ/bin/hello.apk


Con l'sdk 24  (che non riesco a farlo andare sul Samsung J320f)
Ora si procede con la signature. Solo una volta si fa:
	keytool -genkeypair -validity 365 -keystore mykey.keystore -keyalg RSA -keysize 2048
(per i dettagli vedi il file secrets.md e va fatta solo una volta)  
apksigner sign --ks mykey.keystore $PROJ/bin/hello.apk 
Per vedere se la signature funziona:
apksigner verify $PROJ/bin/hello.apk
Nota che l'help mostra (https://developer.android.com/tools/apksigner):
>Caution: If you sign your APK using apksigner and make further changes to the APK, the APK's signature is invalidated. If you use zipalign to align your APK, use it before signing the APK.
Il che significa che la signature è l'ultimo step prima dell'installazione.

Punto 16
Per una lista completa delle opzioni di adb vedi https://developer.android.com/tools/adb
qui l'opzione -r significa reinstall
adb install -r $PROJ/bin/hello.apk

Non funziona ancora:
performing Push Install
/home/igor/scratch/android/hello/bin/hello.apk: 1 file pushed, 0 skipped. 474.3 MB/s (1573238 bytes in 0.003s)
        pkg: /data/local/tmp/hello.apk
Failure [INSTALL_FAILED_DEXOPT]
Per vedere il log dettagliato si usa:
adb logcat
salvato sul file device.log per un'analisi dell'errore:
E/Parcel  (  754): Class not found when unmarshalling: xqm
E/Parcel  (  754): java.lang.ClassNotFoundException: xqm

Punto 17
Se l'installazione funziona allora si lancia l'app con:

	adb shell am start -n dom.domain/.SayingHello

Punto 18
Disinstallare l'app:

	adb uninstall dom.domain

### Hello World con java-11 e platform 33
Come prerequisito ora uso java 11 e non java 8 come la sezione precedente

	export JAVA_HOME=/usr/lib/jvm/java-1.11.0-openjdk-amd64
	export ANDROID_HOME=$HOME/android
	export ANDROID_SDK_ROOT=${ANDROID_HOME}/platforms/android-33
	export PATH=${ANDROID_HOME}/cmdline-tools/latest/bin:${ANDROID_HOME}/platform-tools:${ANDROID_HOME}/build-tools/33.0.2:${ANDROID_HOME}/ndk-bundle:${PATH}
	export PROJ=$HOME/scratch/android/hello	
	export ANDROID_TOOLS=${ANDROID_HOME}/tools
	
processo: java -> class -> dex ->apkpart -> unaligned.apk -> apk -> signed apk -> install
	
Clean up:

	find ./obj  -name "*.class" -exec rm {} +
	find ./bin  -name "*.*" -exec rm {} +

Compilazione resources

	aapt package -f \
	-I ${ANDROID_SDK_ROOT}/android.jar \
	-J src -m \
	-M AndroidManifest.xml -S res -v

compilazione class e dex
  
	javac -d obj -bootclasspath ${ANDROID_SDK_ROOT}/android.jar  -classpath src -source 1.7 -target 1.7 src/dom/domain/*.java

	d8 --lib ${ANDROID_SDK_ROOT}/android.jar --output $PROJ/bin $(find ./obj -name "*.class")

packaging

	aapt package -f \
	-f -m -F $PROJ/bin/hello.apkPart \
	-I ${ANDROID_SDK_ROOT}/android.jar \
	-M AndroidManifest.xml -S res -v
  
Make the full APK using the `ApkBuilder` tool (tool che però è marcato come THIS TOOL IS DEPRECATED.)

	CLASSPATH=$ANDROID_TOOLS/lib/* java \
	com.android.sdklib.build.ApkBuilderMain $PROJ/bin/hello.unaligned.apk \
	-u -f $PROJ/bin/classes.dex -z $PROJ/bin/hello.apkPart

Allineamento

	zipalign -f -v 4 $PROJ/bin/hello.unaligned.apk  $PROJ/bin/hello.apk
	
Signer 

	apksigner sign --ks mykey.keystore $PROJ/bin/hello.apk 

Installazione

	adb install -r $PROJ/bin/hello.apk
	
Start
	
	adb shell am start -n dom.domain/.SayingHello
	
Disinstallare l'app:

	adb uninstall dom.domain
	
### adb devices
Prerequisito è una powershell in admin con
	
	usbipd wsl list
	usbipd wsl attach --busid 2-4
E in ubuntu WSL bisogna avere sdb. Se uso SDL2 allora uso le seguenti variabili di sistema:

	export ANDROID_SDK_ROOT=$HOME/android/
	export PATH=${ANDROID_SDK_ROOT}/cmdline-tools/latest/bin:${ANDROID_SDK_ROOT}/platform-tools:${PATH}
	

Ora  stop e restart del server, ma questa volta usando sudo.
Nota che sul device mi compare un messaggio per autorizzare il collegamento

	adb kill-server
	sudo /home/igor/android/platform-tools/adb start-server
	adb devices
List of devices attached
420050c6cc2eb300        device 

Se non funziona e mostra una lista vuota, significa che non c'è nessun dispositivo collegato.
Se invece c'è, allora prova a vedere il file 51-android.rules dove ci piazzo il mio dispositivo
Ora, come citato nel blog halimsamy.com, creo questo file:

	sudo nano /etc/udev/rules.d/51-android.rules
e lì ci metto questa riga:
SUBSYSTEM=="usb", ATTR{idVendor}=="04e8", ATTR{idProduct}=="6860", MODE="0666", GROUP="plugdev", SYMLINK+="android%n"
Dove vendor e productid li ho presi da lsusb.
 
Vendor e Prod Id in lsusb
Vendor 04e8
ProdId 6860 


## Gradle
Android Studio usa Gradle per compilare i progetti col target android.
Nel mio Ubuntu WSL ho installato Gradle con apt, la versione è:

	gradle --version
	Gradle 4.4.1
La 4.4.1 non funziona con java 11 e allora ho usato il wrapper alla versione 8.1.1
Il comando è il seguente:
	
	gradle wrapper --gradle-version=8.1.1
Quando ne richiedo la versione, esso scarica dalla sua repository la versione 8.1.1
e la rende disponibile per tutto il progetto. Per usare il wrapper si usa:

	./gradlew --version
	
Per avere gli script in formato kotlin (dopo aver cancellato quelli che avevo creato in formato groovy)

	./gradlew  init --dsl kotlin
 
## SDL2 hello World per android
Vorrei provare a compilare un progetto HelloWorld per SDL2. È un programma che mostra
un rettangolo 

### Uso dopo aver settato tutto
Faccio partire UbuntuMinitoro su WSL.

	cd scratch/android/sdl-android-prj-hello/
	code .
Nel terminal di Code e il telefono collegato all'hub provo a vedere se adb funziona

	export ANDROID_SDK_ROOT=$HOME/android/
	export PATH=${ANDROID_SDK_ROOT}/cmdline-tools/latest/bin:${ANDROID_SDK_ROOT}/platform-tools:${PATH}
	sudo /home/igor/android/platform-tools/adb start-server
	adb devices
	
Compilare il codice

	./gradlew compileDebugSources
	
Compilare e Installare il programma sul device (anche dopo aver cambiato il codice):

	./gradlew installDebug
	
Il programma con l'icona SDLGame si trova sotto il menu Apps alla fine
Per farlo partire dal computer si usa:
	
	adb shell am start -n org.libsdl.app/.SDLActivity
	
Per vedere altri tasks si usa:

	./gradlew tasks
	
### Problemi
Il comando

	./gradlew installDebug
mi manda windows in blue screen of the death. Per ricollegarmi al device ho dovuto
far ripartire windows due volte. Il telefono solo una volta.

Altro problema con RedMi 11

	List of devices attached
	f5c24a47        unauthorized
Questo si risolve nel momento in cui si collega il cavo al telefono. Qui compare il popup che chiede
l'autorizzazione. Dopo la conferma sul telefono, quello che avviene è:

	List of devices attached
	f5c24a47        device
Interessante il fatto che adb devices lanciato in windows:

	D:\Xiaomi\platform-tools_r34.0.5-windows\platform-tools>adb devices
non mostra nulla se non si lancia 

	usbipd wsl attach --busid 5-1 --distribution UbuntuMinitoro

Altro problema su RedMi  
INSTALL_FAILED_USER_RESTRICTED: Install canceled by user
Questo dipende dai developer settings che non sono settati su:  
Turn On "USB Debugging"
Turn On "Install via USB"

Il problema è che li posso attivare solo se ho inserito una simcard. Ho inserito la simcard di hot,
che non è nenache attivata, ed ha funzionato l'attivazione delle opzioni.
Ho poi tolto la simcard e fatto un reboot e l'opzione è rimasta attiva.
	
### Settaggio iniziale del progetto SDL2 hello World
Per partire scarico i sorgenti di SDL2 stabili (SDL-release-2.26.5.tar.gz) in una dir temporanea 
e copio la sottodirectory android-project, che uso come template,  nel mio scratch di android. 
Qui copio il contenuto della directory di SDL (non tutto vedi sotto)
nella nuova sotto directory SDL in \Ubuntu-22.04\home\igor\scratch\android\sdl-android-prj-hello\app\jni
In Ubuntu-22.04\home\igor\scratch\android\sdl-android-prj-hello\app\jni\src creo il file main.cpp
dove il contenuto lo prendo dal gist https://gist.github.com/fschr/92958222e35a823e738bb181fe045274
Ora si tratta di compilare il progetto con gradle, che è poi gradlew. Uso Visual Code per il terminal
e l'editor del progetto. Il file gradlew ha bisogno di essere eseguibile:

	chmod +x ./gradlew
Quando lancio ./gradlew tasks mi installa gradle 7.3
Ora si tratta di settare le variabili di ambiente per compilare il progetto.
Il settaggio fatto in precedenza per la compilazione senza gradle non va. Sembra che occorra solo
una variabile che mi definisce il ROOT delle installazioni di android

	export ANDROID_SDK_ROOT=$HOME/android/
	export PATH=${ANDROID_SDK_ROOT}/cmdline-tools/latest/bin:${ANDROID_SDK_ROOT}/platform-tools:${PATH}
	sudo /home/igor/android/platform-tools/adb start-server
	adb devices
Il path cmdline-tools mi serve solo se voglio lanciare sdkmanager nel terminal,
mentre platform-tools mi server per adb e connettere un device.

Ora si pianta qui:
> Preparing "Install NDK (Side by side) 21.4.7075529   

e non fa più nulla. Dovrei installare NDK con sdk-manager a linea di comando. Come?
Con sdkmanager, che funziona dopo aver aggiornato il path con i cmdline-tools, vedo di installare ndk 21.4.7075529 
Esso si vede e si installa con:  

	sdkmanager --list | grep ndk
	sdkmanager "ndk;21.4.7075529"
Anche se non sono sicuro che non faceva nulla. L'sdk 31 l'ha scaricato senza problemi, 
probabile che il download di ndk non fosse finito in quanto con gradlew non c'è indicazione del progresso 
come, invece, c'è in sdkmanager.

Ora il comando

	./gradlew tasks
funziona. Ora vorrei capire quale ABI target devo usare per il mio J320f
Secondo https://de.wikipedia.org/wiki/Samsung_Galaxy_J3_(2016)#Hardware il processore è 
un 1,5 GHz ARM Cortex-A7 CPU.
Vale a dire l'ultima generazione dei processori ARM a 32 bit. Quindi il target v7 32 bit dovrebbe essere corretto.

Ora posso partire per la compilazione con:

	 ./gradlew compileDebugSources
per un cleanup

	./gradlew clean
Nota che se manca qualcosa tipo build-tools o il framework  31 esso viene installato prima di partire con
la compilazione. Non uso il framework 33 in quanto dovrei aggiornare anche gradlew.

Nota che ho copiato i sorgenti di SDL dallo zip SDL-release-2.26.5.tar.gz che è l'ultima release stabile di sdl2
dentro a app/jni/SDL. 
Importante: non ho copiato tutto, ma solo i files di root (Android.mk ci deve essere) poi include
e src. Le directories di SDL-release-2.26.5 "android project" è meglio non copiarle 
che mi pianta tutto il file explorer. 
Penso che partano dei daemon con una referenza circolare che ho risolto solo con un restart del computer. 
In Andorid.mk uso poi:
	
	SDL_PATH := ../SDL
	LOCAL_SHARED_LIBRARIES := SDL2
Nota che SDL viene compilato come shared library (estensione so). 
Il nome SDL2 si trova poi nella sotto directory SDL/Android.mk
Nota che nominare la directory SDL2 invece di SDL in jni non serve. Visual Code per avere un editor senza errori
usa un nuovo settings c_cpp_properties.json dove si ha un include path "${workspaceFolder}/**".
Qui basta fare un reload window e in main.cpp si usa poi:

	#include <SDL.h>
senza usare riferimenti a SDL2 come nell'installazione di ubuntu. Per Android copio i sorgenti di SDL.
Il processo di build crea i binaries nella sotto directory di build.
Nella directory /build/intermediates/ndkBuild/debug/obj/local/armeabi-v7a si vedono le due librerie 
create con ndk: libSDL2.so libmain.so una volta che il build ha successo.
In /build/intermediates/javac/debug/classes/org/libsdl/app ho, invece, tutti i files .class che servono per
l'activity che, una volta lanciata, ingloba il loop di Sdl con il mio main.cpp.

Quando si aggiunge la libreria SDL2_image essa va aggiunta nella funzione getLibraries() del file SDLActivity.java.

Mi sembra che con gradlew e Visual Code riesco a programmare senza problemi senza dover installare Android Studio. 
Tutti i miei esperimenti li ho fatti a linea di comando e in Visual Code
usando WSL2 sotto windows 11. Con un progetto template che contenga gradle lo sviluppo risulta molto veloce.

### SDL2 hello World su RedMi 11
Su questo smartphone il processore è il Snapdragon 680. Ha 4GB Ram e 12GB di memoria interna.
Su https://en.wikipedia.org/wiki/List_of_Qualcomm_Snapdragon_systems_on_chips
viene descritto la sua caratteristica:
4 + 4 cores (2.4 GHz Kryo 265 Gold – Cortex-A73 + 1.9 GHz Kryo 265 Silver – Cortex-A53).
The ARM Cortex-A73 is a central processing unit implementing the ARMv8-A 64-bit instruction set designed by ARM Holdings' Sophia design centre
Quindi è un arm v8 a 64 bit. l'opzione dovrebbe essere arm64-v8a.
Per compilare uso:
./gradlew compileDebugSources
Per installare
./gradlew installDebug
Per lanciare l'app dal terminal di Visual Code:
adb shell am start -n org.libsdl.app/.SDLActivity
