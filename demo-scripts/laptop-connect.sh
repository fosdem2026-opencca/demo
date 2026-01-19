#!/bin/bash

set -x

kill -9 $(lsof -t -iTCP:5901 -sTCP:LISTEN) 2>/dev/null || true
ssh -f -N -o ExitOnForwardFailure=yes -L 5901:192.168.33.2:5901 user@192.33.93.163
vncviewer localhost:5901