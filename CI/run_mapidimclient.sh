#!/bin/bash

echo "Run DIMClient Bash script started"

cd ../DIMClient
export DIM_DNS_NODE=localhost

# Indefinite mapi test
topic="FRED1/INDEF_MAPI/LOOP0/TEST"
bin/DIMClient --F --t $topic --m "test"
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC test failed"
  exit 1
else
  echo "[$topic] FRED TOPIC test succeeded"
  sleep 5
fi

# Indefinite mapi test - response on error message (publishError)
topic="FRED1/INDEF_MAPI/LOOP0/TEST"
bin/DIMClient --F --t $topic --m "error"
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC test succeeded"
  sleep 5
else
  echo "[$topic] FRED TOPIC test failed"
  exit 1
fi

# Basic mapi test
topic="FRED1/MAPI_EXAMPLE/LOOP0/TEST"
bin/DIMClient --F --t $topic --m "1"
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC test failed"
  exit 1
else
  echo "[$topic] FRED TOPIC test succeeded"
  sleep 5
fi

# Iterative mapi test
topic="FRED1/ITERATIVEBOARDS/ITERATIVEBOARD0/INTERNAL/AVERAGE"
bin/DIMClient --F --t $topic --m "20"
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC test failed"
  exit 1
else
  echo "[$topic] FRED TOPIC test succeeded"
  sleep 5
fi

# Mapigroup test
topic="FRED1/TESTGROUPS/TESTGROUP0/TEST"
bin/DIMClient --F --t $topic --m "go"
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC test failed"
  exit 1
else
  echo "[$topic] FRED TOPIC test succeeded"
  sleep 5
fi