# Setup con NSIS
L'installer di NSIS viene creato usando ruby e i due script rb che sono in questa directory.
La versione del programma viene letta direttamente nei sorgenti in MenuMgr.cpp.
Ho copiato tutti i files necessari per produrre il setup nella directory build.
È quella usata da Cmake e ci sono altri files che non mi servono nel target che vanno esclusi (file setuo_creator.rb).
Nota che la directory published .\published non può essere un parent di build.

## target_deploy_info.yaml
- Mettere in target_deploy_info.yaml la directory dove vengono messe tutte le versioni
- Occorre nsis installer. Il path in target_deploy_info.yaml

## Prerequisiti
- gem install D:\Projects\github\filescandir\filescandir-0.0.1.gem
- gem install --remote erubis

## Creare il setup
Per usare ruby in powershel setto il path del programma con:
    $env:path="D:\ruby\ruby_3_2_0\bin;"+ $env:path
    ruby -v
    => ruby 3.2.0 (2022-12-25 revision a528908271) [x64-mingw-ucrt]
Per lanciare lo script uso:

    cd D:\Projects\github\carte\solitario\deploy
    ruby build_the_setup.rb
Questo comando crea il setup.exe in un colpo solo. La sotto directory di published viene aggiornata 
automaticamente.

