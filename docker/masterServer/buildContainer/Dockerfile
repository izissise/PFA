# Fusion Master Server Builder
#
#
# Build the master server
#
#
# Version 1

FROM      debian:wheezy

MAINTAINER      izissise <morisset.hugues@gmail.com>

RUN apt-get update && apt-get install -y locales

RUN dpkg-reconfigure locales && \
    locale-gen C.UTF-8 && \
    /usr/sbin/update-locale LANG=C.UTF-8

ENV LC_ALL C.UTF-8

#Install dependencies

RUN apt-get install --no-install-recommends -y \
  cmake build-essential ssh git g++ gcc protobuf-c-compiler protobuf-compiler

RUN apt-get install --no-install-recommends -y \
  libenet-dev libprotobuf-dev libboost-system-dev libboost-program-options-dev libsqlite0-dev libsqlite3-dev

#Build here
RUN mkdir PFA
ADD PFA /PFA/
ADD buildMasterServer.sh /
RUN ./buildMasterServer.sh

CMD ["bash"]





