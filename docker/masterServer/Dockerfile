# Fusion Master Server
#
#
# Lauch the master server
#
#
# Version 1

FROM      ubuntu

MAINTAINER      izissise <morisset.hugues@gmail.com>

EXPOSE 25255/udp

#Install dependencies

RUN apt-get update && apt-get install --no-install-recommends -y \
  libenet2a libprotoc8 libsqlite3-0
  
 
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


