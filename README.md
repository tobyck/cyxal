# Cyxal

A version of Vyxal re-written in C.

## Usage

Well, you can't use it at all yet so... reading the next section would be greatly appreciated.

## Contributing

Note: this project is set up for UNIX-based systems and Windows support has not yet been added. Help from a Windows user would be very helpful.

### Environment

If you're a collaborator of the GitHub repository just clone the repo, and you should be ready to go, otherwise make a fork of the repository and sumbit pull requests.

To build the code, run `make build` (see the `Makefile` for more info), or to build and run the executable afterwards, use `make run` (in CLion these make targets can be configured in the panel in the top right next the run/build buttons for a much easier process).

Before you go to make more changes, make sure to pull the latest code from the master branch, and I would recommend managing this through your IDE's Git interface. As for specific IDE's it's up to you, but at the time of writing this I'm using JetBrains CLion which comes free with the GitHub education pack.

Please write meaningful commit messages and follow the style guide below.

Thank you!

### Style Guide

- Variable/argument names are `lower_snake_case`
- Structs, enums, and unions are `PascalCase`
- Macros are also `lower_snake_case` unless they are constants, in which case they should be `UPPER_SNAKE_CASE`
- The `*` for pointers sits next to the identifier with a space after the type
- Comment where you think it's necessary, but please provide a comment before function definitions describing what it does.

### Folder Structure

As a general rule, each source file (`.c`) should have a corresponding header file (`.h`) which contains definitions for types and macros, and function *declarations*, not definitions.  When you need to import a file into another, it's the header file that you include.

Below are descriptions of what each folder is for.

- `src`: Contains the source code (seperated into further folders) from which an executable is compiled.
  - `parser`: Contains the tokeniser to split code into a flat list of simple tokens e.g. "number" or "open structure", and the parser which groups those tokens to be executed by the interpreter.
  - `builtins`: Contains types and methods regarding the `CyValue` struct which is used to store any type available in Cyxal, and files containing the elements, modifiers, and structures.
- `build`: Files from `src` are compiled into an executable which lives here. This folder can be emptied with `make clean`.
- `gmp`: Files for the [GMP Multiple Precision
  Arithmetic Library](https://gmplib.org) which is used to allow arbitrary precision rationals like in Vyxal.