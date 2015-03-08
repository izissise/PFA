#!/bin/bash

cd /home/${RUNUSER}/
su -m "${RUNUSER}" -c ${SERVICE_COMMAND}
