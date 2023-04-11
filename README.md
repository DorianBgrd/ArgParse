ArgParse
========

This library is imitates the python ArgParse module and provide simple management 
for command line tools and commands.
The Library is header only and does not depend on any third party library, except
standard libraries like string, vector or map.

Main principles
===============

The library provides a Parser class, that registers flags, and parses command line
arguments and can be queried for results.

Exemple :
---------
```cpp
Parser parser;

// Register a program name.
parser.SetProgramName("Test program");

// Add a new flag.
parser.AddFlag("-f",         // Short flag name. 
               "--flag",     // Long flag name.
               0,            // number of time argument can appear.
               "Description for the added flag."); // Description.

parser.Parse(argc, argv);

parser.FlagFound("-f");  // Returns whether flag was found.

parser.FlagResult("--flag"); // Returns first flag value.

parser.FlagResults("-f"); // Returns all flag values.
```

