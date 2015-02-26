#!/bin/bash

rm -fr PFA
mkdir -p PFA
rsync -r ../../../ ./PFA/

CONT_NAME="fusion_build"
CONT_ID="tmp_fusion_build"

BUILD_PATH="/PFA/build/masterServer"
DEST_PATH="`pwd`/../ressources/"

docker build -t ${CONT_NAME} .

echo "Build finished time to retrieve payload."
docker run --name="${CONT_ID}" ${CONT_NAME}
docker cp ${CONT_ID}:${BUILD_PATH} ${DEST_PATH}

docker stop $CONT_ID
docker rm $CONT_ID

