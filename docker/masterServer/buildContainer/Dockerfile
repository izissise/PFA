# Fusion Master Server Builder
#
#
# Build the master server
#
#
# Version 1

FROM      ubuntu

MAINTAINER      izissise <morisset.hugues@gmail.com>

#Install dependencies

RUN apt-get update && apt-get install --no-install-recommends -y \
  cmake build-essential ssh g++ gcc protobuf-c-compiler protobuf-compiler

RUN apt-get install --no-install-recommends -y \
  libenet-dev libprotobuf-dev libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libsqlite0-dev libsqlite3-dev

#Build here
RUN mkdir PFA
ADD PFA /PFA/
ADD buildMasterServer.sh /
RUN ./buildMasterServer.sh

CMD ["bash"]





