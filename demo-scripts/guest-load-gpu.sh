#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

set -x
cd $SCRIPT_DIR/../../linux/drivers/gpu/drm/panthor
rmmod ./panthor.ko || true
modprobe ./panthor.ko

