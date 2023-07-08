#!/bin/bash

echo "Run ALFDummies Bash script started"

export DIM_DNS_NODE=localhost
../ALFDummies/FLP042/ALFDummy/bin/ALFDummy --a FLP042 --l 10 --s 10 --v &
../ALFDummies/FLP001/ALFDummy/bin/ALFDummy --a FLP001 --l 10 --s 10 --v &
