#!/bin/bash

ls -l /usr/lib64/libaio*
ls -l /usr/lib/libaio*

export LD_LIBRARY_PATH=/usr/lib64:/usr/lib:$LD_LIBRARY_PATH

echo "Parser test Bash script started"

cp -r ./examples/config/ .
cp -r ./examples/SCA/* .
cp -r ./examples/SWT/* .
sed -i 's/dimdnsexample.cern.ch/localhost/g' ./config/fred.conf

./bin/FREDServer --p

if [ $? -ne 0 ]
then
  echo "Parser failed to parse correct configuration files"
  exit 1
else
  echo "Correct configuration files parsed successfully"
fi

cp -r ./CI/wrong_examples/* .
./bin/FREDServer -- p

if [ $? -ne 0 ]
then
  echo "Configuration error detected by the parser"
  exit 0
else
  echo "Configuration error not detected by the parser"
  exit 1
fi
