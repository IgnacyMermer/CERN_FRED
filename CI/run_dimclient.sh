#!/bin/bash

echo "Run DIMClient Bash script started"

cd ../DIMClient
export DIM_DNS_NODE=localhost
# SWT test
topic="FRED1/POWER_BOARDS/POWERUNIT1/INIT"
bin/DIMClient --F --t $topic
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC test failed"
  exit 1
else
  echo "[$topic] FRED TOPIC test succeeded"
  sleep 5
fi

#SWT Equation test
topic="FRED1/POWER_BOARDS/POWERUNIT4/VOLTAGE/READ"
bin/DIMClient --F --t $topic --m 33
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC test failed"
  exit 1
else
  echo "[$topic] FRED TOPIC test succeeded"
  sleep 5
fi

#SCA test 
topic="FRED1/RU_SCA/ITSRU0/SCATEMP/READ"
bin/DIMClient --F --t $topic
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC test failed"
  exit 1
else
  echo "[$topic] FRED TOPIC test succeeded"
  sleep 5
fi

#GROUP topic test (inner_barrel.section)
topic="FRED1/INNER_BARREL/STAVES/READ_ALL"
bin/DIMClient --F --t $topic
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC test failed"
  exit 1
else
  echo "[$topic] FRED TOPIC test succeeded"
  sleep 5
fi

#Test for wrong input - READ should have empty string as input
topic="FRED1/RU_SCA/ITSRU0/SCATEMP/READ"
bin/DIMClient --F --t $topic --m 123456
if [ $? -ne 0 ]
then
  echo "[$topic] FRED TOPIC error test succeeded"
  exit 0
else
  echo "[$topic] FRED TOPIC error test failed"
  exit 1
fi