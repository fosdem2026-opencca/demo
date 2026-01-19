#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ROOT_DIR=$SCRIPT_DIR/../../
SNAPSHOT_DIR=$ROOT_DIR/snapshot
BIN=$SNAPSHOT_DIR/lkvm
LINUX_DIR=$ROOT_DIR/linux


# Unload panthor on host
rmmod panthor || true

# Power on GPU so that MMIO is accessible
rmmod $LINUX_DIR/arch/arm64/kvm/demo/gpu_poweron.ko || true
insmod $LINUX_DIR/arch/arm64/kvm/demo/gpu_poweron.ko

# Load irq router
rmmod $LINUX_DIR/arch/arm64/kvm/demo/irq.ko || true
insmod $LINUX_DIR/arch/arm64/kvm/demo/irq.ko

