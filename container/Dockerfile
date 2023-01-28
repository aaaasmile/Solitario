# syntax=docker/dockerfile:1
FROM ubuntu:22.04

ENV LANGUAGE='en_US.UTF-8'
ENV LC_ALL='en_US.UTF-8'
ENV LANG='en_US.UTF-8'
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

# Ubuntu
ENV DEBIAN_FRONTEND=noninteractive
ENV LANGUAGE='en_US.UTF-8'
ENV LC_ALL='en_US.UTF-8'
ENV LANG='en_US.UTF-8'
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

# Ubuntu
ENV DEBIAN_FRONTEND=noninteractive

# Time zone
RUN  ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get -q update

RUN apt-get -q -y install locales

# Locale
RUN locale-gen $LC_ALL

RUN apt-get -q -y install \
    build-essential \
    autoconf \
    gawk \
    git \
    vim \
    tar \
    gzip \
    mc \
    nano \
    wget \
	x11-apps

RUN apt-get -q -y install \
    automake \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev

#Solitario
RUN adduser invido
RUN usermod -a -G adm,dialout,cdrom,floppy,sudo,audio,dip,video,plugdev invido

USER invido
RUN cd /home/invido && mkdir ./src && mkdir ./Solitario && cd ./src && \
	wget https://github.com/aaaasmile/Solitario/archive/refs/heads/ubuntu-compile.zip &&\
    unzip ubuntu-compile.zip &&\
    cd Solitario-ubuntu-compile && autoreconf --install && \
    ./configure --bindir=/home/invido/Solitario && \
	make -j8 && \
    make install && \
    cp -a data /home/invido/Solitario && \
	rm -rf /usr/local/src/* 
	
WORKDIR /home/invido/Solitario
ENTRYPOINT ["./solitario"]