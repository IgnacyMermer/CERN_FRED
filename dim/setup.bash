#!/bin/bash

# Bash version of .setup file coming with dim distribution
# Tested on Linux 7.3 by A.Formica

POSSOSVALUES="HP-UX,AIX,OSF1,SunOS,Solaris,LynxOS,Linux,Darwin"
echo $POSSOSVALUES

if [ ! -z $OS ]; then
    echo "OS is set to $OS"
    TEMP=$OS
    echo $POSSOSVALUES | grep -q $TEMP
    if [ $? != 0 ]; then
        echo "Unknown OS... setup failed"
        echo "Possible values are: $POSSOSVALUES"
        exit 1
    fi
else
    echo "Variable OS must be defined... setup failed"
    echo "Possible values are: $POSSOSVALUES"
    exit 1
fi

if [ "${OS}" == "Solaris" ]; then
    export PATH="/usr/ccs/bin:$PATH"
    echo "Update path: $PATH"
fi

if [ ! -z $DIMDIR ]; then
    echo "DIMDIR is set to $DIMDIR"
else
    export DIMDIR=$(pwd)
    echo "DIMDIR is set to $DIMDIR"
fi

if [ ! -z $ODIR ]; then
    echo "ODIR is set to $ODIR"
else
    echo "ODIR not defined"
    case $OS in 
        HP-UX)      export ODIR=hp;;
        AIX)        export ODIR=aix;;
        OSF1)       export ODIR=osf;;
        SunOS)      export ODIR=sunos;;
        Solaris)    export ODIR=solaris;;
        LynxOS)     export ODIR=lynxos;;
        Linux)      export ODIR=linux;;
        Darwin)     export ODIR=darwin;;
        *)          echo "Unknown OS... setup failed"; exit 1;;
    esac
    echo "...setting ODIR to $ODIR"
fi

if [ "${OS}" == "Linux" ]; then
    if [ -z $LD_LIBRARY_PATH ]; then
        export LD_LIBRARY_PATH=${DIMDIR}/${ODIR}
    else
        export LD_LIBRARY_PATH=${DIMDIR}/${ODIR}:$LD_LIBRARY_PATH
    fi
fi

alias TestServer="$DIMDIR/$ODIR/testServer"
alias TestClient="$DIMDIR/$ODIR/testClient"
alias Test_server="$DIMDIR/$ODIR/test_server"
alias Test_client="$DIMDIR/$ODIR/test_client"
alias Dns="$DIMDIR/$ODIR/dns"
alias Dim_get_service="$DIMDIR/$ODIR/dim_get_service"
alias Dim_send_command="$DIMDIR/$ODIR/dim_send_command"
alias DimBridge="$DIMDIR/$ODIR/DimBridge"
alias Did="$DIMDIR/$ODIR/did"

if [ ! -d $DIMDIR/$ODIR ]; then
    mkdir -p $DIMDIR/$ODIR
    echo "Created Directory: $DIMDIR/$ODIR"
fi
