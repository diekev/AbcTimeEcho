AbcTimeEcho
===========

Simple utility to print time information from an Alembic archive.

Usage
-----

    abc_time_echo FILE

Example Output
--------------

    Number of TimeSamplings in the file : 2

    Minimum time in seconds : 0.04
    Maximum time in seconds : 0.52

    Minimum frame : 1
    Maximum frame : 13

    Frame rate : 25 FPS

Build Instruction
-----------------

Requires Alembic and OpenEXR to be installed.

    cd /path/to/sources
    mkdir build && cd build
    cmake ..
    make
