FROM debian:11 as freeswitch

ARG SIGNALWIRE_LOGIN
ARG SIGNALWIRE_PASSWORD

RUN apt-get update && apt-get install -y apt-transport-https ca-certificates gnupg2 wget lsb-release
RUN wget --http-user=${SIGNALWIRE_LOGIN} --http-password=${SIGNALWIRE_PASSWORD} -O /usr/share/keyrings/signalwire-freeswitch-repo.gpg https://freeswitch.signalwire.com/repo/deb/debian-release/signalwire-freeswitch-repo.gpg
RUN echo "machine freeswitch.signalwire.com login ${SIGNALWIRE_LOGIN}  password ${SIGNALWIRE_PASSWORD}" > /etc/apt/auth.conf
RUN echo "deb [signed-by=/usr/share/keyrings/signalwire-freeswitch-repo.gpg] https://freeswitch.signalwire.com/repo/deb/debian-release/ `lsb_release -sc` main" > /etc/apt/sources.list.d/freeswitch.list
RUN echo "deb-src [signed-by=/usr/share/keyrings/signalwire-freeswitch-repo.gpg] https://freeswitch.signalwire.com/repo/deb/debian-release/ `lsb_release -sc` main" >> /etc/apt/sources.list.d/freeswitch.list

RUN apt-get update && apt-get install -y freeswitch libfreeswitch-dev libfreeswitch1-dbg freeswitch-mod-event-socket freeswitch-mod-event-test freeswitch-conf-testing freeswitch-mod-console freeswitch-mod-test freeswitch-mod-commands freeswitch-mod-dialplan-xml freeswitch-mod-dptools freeswitch-mod-lua
RUN apt-get install -y wget unzip build-essential pkg-config libsqlite3-dev

ENV FS_INCLUDES /usr/include
ENV FS_MODULES /usr/lib64/freeswitch/mod

EXPOSE 8021
EXPOSE 5060
EXPOSE 16384-32768

FROM freeswitch as wren
ARG WREN_COMMIT=4ffe2ed38b238ff410e70654cbe38883f7533
RUN apt-get install -y python3 git-core
# build amalgamation of wren on /usr/src/wren/amalgamation
RUN cd /usr/src \
    && git clone https://github.com/wren-lang/wren \
    && cd wren \
    && git checkout -f ${WREN_COMMIT} \
    && python3 util/generate_amalgamation.py > wren.c \
    && mkdir amalgamation \
    && cp wren.c src/include/wren.h amalgamation/

VOLUME ["/usr/src/mod_wren"]
WORKDIR /usr/src/mod_wren

CMD ["/bin/bash"]
