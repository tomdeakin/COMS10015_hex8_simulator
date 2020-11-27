# COMS10015: Hex 8 ISA simulator

This is a simple simulator to run Hex 8 machine code programs.

## Building

The code should build just by running:

    make 

## Running

The binary takes a single argument: a `*.hex` file with Hex 8 machine code.

The simulator runs the program, and prints out the value of `areg` and the memory when a `BR -2` instruction occurs.
This `BR -2` allows the simulator to stop, however an implementation in ModuleSim will continue running indefinitely.

