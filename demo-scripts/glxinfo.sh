#!/bin/bash

set -x
glxinfo -B | egrep 'OpenGL vendor|OpenGL renderer|OpenGL version'