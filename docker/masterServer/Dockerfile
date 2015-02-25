# Fusion Master Server
#
#
# Lauch the master server
#
#
# Version 1

FROM      debian:wheezy

MAINTAINER      izissise <morisset.hugues@gmail.com>

EXPOSE 25255/udp

RUN apt-get update && apt-get install -y locales adduser

RUN dpkg-reconfigure locales && \
    locale-gen C.UTF-8 && \
    /usr/sbin/update-locale LANG=C.UTF-8

ENV LC_ALL C.UTF-8

#Install dependencies

RUN apt-get install --no-install-recommends -y \
  libenet1a libprotobuf7 libboost-system1.49.0 libboost-program-options1.49.0 libsqlite3-0
  
 
#Create new user
ENV RUNUSER server
RUN adduser --disabled-password --gecos '' "${RUNUSER}"


#Add binary
ADD ressources/masterServer /home/server/

RUN chown ${RUNUSER}:${RUNUSER} /home/server/masterServer

#Set entry point
ENV SERVICE_COMMAND "/home/server/masterServer"

ADD ressources/entry.sh /

CMD ["./entry.sh"]


