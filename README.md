# FREDServer

## Version History
* v3.1 - Updates PATTERN_PLAYER for ALF compatibility [v0.18.0](https://github.com/AliceO2Group/ALF/releases/tag/v0.18.0)
* v3.0 - New features,Changes:
  - Added support for full size (80bit) return value of SWT protocol messages in standard topics (HIGH_WORD parameter in topic config, EQUATION cannot be used in this mode)
  - Added support to limit how many threads should be created by FRED queues (BANK_COUNT parameter in config)
  - New Logging option - PrintData. Logging with Data option is enabled after passing `-d/--data` parameter during start of FREDServer
  - **Modified implementation of MAPI code, Custom code requires own repository with FREDServer as submodule named `core`, for usage, see [Mapi Example](https://gitlab.cern.ch/alialfred/fred_mapiexample) , [Standard topic Example](https://gitlab.cern.ch/alialfred/fred_example)**
  - Extended examples for every supported ALF topic, Added examples for Indefinite MAPI

* v2.5 - Latest version which supports User code in main.cpp
* v2.4 - Breaks backwards compatibility with ALF version < 0.14.0 (FLP Suite v0.18.0). Changes:
  - FRED configuration files adjusted for changes in ALF services
  - Added support for new CRU (User Space) Register_READ/WRITE services
  - Tested compaitble ALF's parallel link access
* v2.3.1 - Adds DIM_HOST_NODE environment variable support
* v2.3 - Changes:
  - Adds support for new ALF naming convention (backwards compatibility maintained)
  - Adds support for CRORC
* v2.2 - New features:
  - LLA support:
    - LLA_MAPPING block in .section or .cru files
    - LLA parameters = repeat, delay (defaults: 1, 0)
  - Database interface (intended for standard dddfero schemas)
  - CANALF support
* v2.1 - Adds new features:
  - SCA: "n,wait" line instructs ALF to pause between SCA instructions, where n is an integer in milliseconds
  - CRU_MAPPING block added to .section files. See: 'examples/CRU'. Adds:
    - PATTERN_PLAYER service (for usage, see: [ALF Readme on GitHub](https://github.com/AliceO2Group/ALF#pattern_player))
    - REGISTER_READ and REGISTER_WRITE services for those CRUs in config
  - Indefinte MAPI (for usage, see [docs](https://espace.cern.ch/alicecontrols/ALICE%20DCS%20Wiki/ALFRED%20Wiki.aspx))
  - OUT_VARS config is now list of OUT_VARs to publish in _ANS, with order respected
    - 'EQUATION' keyword can be used in OUT_VARS for publishing result of an EQUATION
  - CANALF support
  - Various bugfixes, minor improvements, and updated examples
* v2.0.1 - Updated examples, and other minor improvements
* v2.0 - Moves compatibility to O2 ALF (from 'alisw-el7' repository/flp-suite Ansible install) - N.B. NOT backwards compatible
  - Existing 'section' files must be modified to reflect new ALF DIM server name (from ALFID to ALF_HOSTNAME)
  - Existing SWT MAPI implementations must be modified to reflect the new protocol (see: [ALF Readme on GitHub](https://github.com/AliceO2Group/ALF/blob/master/README.md))
* v1.9 - Extends MAPI capabilities, including iterative requests to ALF (Iterativemapi), and custom grouping of topics (Mapigroup)
  - New booleans: 
    - 'noReturn' (do not publish to WinCC)
    - 'noRpcRequest' (do not send request to ALF)
    - 'returnError' (publish via _ERR instead of _ANS)
  - New functions: 
    - 'newRequest(string)' (call this MAPI's 'processInputMessage()')
    - 'publishAnswer(string)' & 'publishError(string)' (publish string to _ANS & _ERR for WinCC)
    - 'newMapiGroupRequest(vector<topicname, input>)' (call these topics with their inputs)
  - See READMEs and graphics in examples/Mapi for more details
* v1.8 - Adds optional '--log \<path\>' parameter to the command line for direct logging to file
* v1.7.1 - Adds Topic names to relevant output lines (more informative)
* v1.7 - Adds Error channel: _ERR service joins _REQ and _ANS
  - _ERR will publish errors from ALF as a string, _ANS will not be updated
* v1.6 - Adds '--help', '--parser', and '--verbose' modes, minor bugfixes
  - '--parser' mode will process and validate your configs, then exit without running
* v1.5 - Adds 'Mask' block for masking devices from Group commands
* v1.4 - Changes the way section files are structured to be much cleaner
  - Removes 'ALFS' and 'TOPICS': info now processed from 'MAPPING' and board config, respectively
* v1.3.1 - Bugfix for carriage-return characters in configs
* v1.3 - Adds direct access to CRU registers (r/w), adds helpful parsing errors

## Prerequisites

cmake3 devtoolset-7

## Installation
Clone core repository
```
git clone https://gitlab.cern.ch/alialfred/FREDServer.git core

cd core

source scl_source enable devtoolset-7
```
If using MAPI:
```
cmake3 . -DMAPI=1
```
If not:
```
cmake3 .
```
Finally build application
```
make all
```

## Required Actions

Look into the `Mapi Example` and `Standard topic example` repositories.
This repositories contains some examples of th required file structure for FRED to work with corresponding README for each example. Links to this examples are located at the end of this README.

1. Create 'config/fred.conf' with your FREDServer name, and the network address of your DIM DNS server (nominally hosted on the same machine that runs FREDServer)
2. Create 'sections', 'boards', 'boards/yourdevice', and 'boards/yourdevice/commands' directories
3. Create config files corresponding to your hardware design, including 'sections/yourgenericdevice.section', 'boards/yourdevice/yourdevice.conf'
4. Create sequence files that correspond to the instruction sequence to be executed by your hardware in e.g. 'boards/yourdevice/commands/readsequence.sqc', etc.



## Run FRED

You have to be in the directory containing your configs, then:
```
/path/to/bin/FREDServer [--help] [--log <path>] [--parser] [--verbose] [--data]
```
## Examples

Examples are available at [Mapi Example](https://gitlab.cern.ch/alialfred/fred_mapiexample) , [Standard topic Example](https://gitlab.cern.ch/alialfred/fred_example). You can use [ALF Simulator](https://gitlab.cern.ch/alice-dcs-kosiceteam/alf-simulator) for testing FRED without real hardware / ALF.
