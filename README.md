# The Embedded Project Cookbook (EPC)

This repository contains the following resources that are referenced in the [*The Embedded Project Cookbook: A Step-By-Step Guide for Microcontroller Projects*](https://amzn.to/3YgrCWc):
-	Source code for the GM6000 example application
-	Source code for automated tests
-	Installation and configuration instructions for framework components and tools
-	Requirements tracing spreadsheets
-	Githup Projects example for use as lighweight planning and tracking tool
-	NQBP2 build tool
-	3rd party tools
-	[Sample Doxygen Output](https://johnttaylor.github.io/epc/namespaces.html)

You can freely use any of the material in this <br> Repository under the terms of the BSD-3 license.

# Documentation

The best and most thorough documentation for the source code and tools in this repository is the book [*The Embedded Project Cookbook*](https://amzn.to/3YgrCWc). However, to limit the length of the book, most of the project and tool configuration documentation was move to the [Wiki page](https://github.com/johnttaylor/epc/wiki) for this project. That is where you will find the detailed instructions on how to build the framework and software described in the book.

# Directory Structure for Repository

The directories in this repository are laid out as follows:

## GM6000 Source Code
|Directory         |Description                                             |
|------------------|--------------------------------------------------------|
|<root>/epc        |Root of the epc repository                              |
|+-- arduino/      |External colony.aruduino repo (using Outcast)           |
|+-- docs/GM6000 Example Docs/ |Contains example docments for <br>• requirements, architecture, and design documents<br>• sample spreadsheets for requirements tracing<br>• tools for generating unique identifiers|
|+-- docs/         |Contains the Doxygen output. See [Doxygen output for GM6000 Name Spaces](https://johnttaylor.github.io/epc/namespaces.html)|
|+-- pkg.info/     |Stores the Outcast 'mapping' information                |
|+-- pkgs.overlaid/|Additional Outcast information (for 'overlay' repos)    |
|+-- projects/     |Application build directory tree                        |
|+-- scripts/      |Collection of utility scripts (e.g. common RATT scripts)|
|+-- src/          |The principal source code tree                          |
|+-- tests/        |Unit test build directory tree                          |
|+-- top/          |Contains scripts related to the build/CI environments   |
|\-- xsrc/         |External code/repos (using Outcast)                     |

[![Alt text](https://github.com/johnttaylor/epc/blob/main/top/book-cover.jpg "Book Cover")](https://amzn.to/3YgrCWc)
