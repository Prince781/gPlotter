gPlotter
========

![Mockup](mockups/mockup.png "Current design for gPlotter")

A neat little program for displaying the output of functions in various ways, among them, as a representation along a Cartesian plane in two-dimensional space. In addition to its graphing capabilties, gPlotter will also serve as an analysis tool capable of evaluating more complex specific information of certain functions, such as their derivatives and maxima/minima.

gPlotter is a free program for Linux, written in C, which uses the GTK+ and Cairo libraries for UI element and graphics rendering, respectively.

##Feature List
* REPL ( **in progress** )
  - dynamic calling of native functions (`sin`,`erf`,`exp`,etc) ( **done** )
  - function evaluation ( **mostly** )
  - defining functions via REPL 
* GUI ( **todo** )
* Core ( **in progress** )
  - Evaluating infix statements ( **done** )
  - Value substitution for variables ( **todo** )
  - Implicit functions ( **todo** )
* Exporting and Saving ( **todo** )
  - Export ( **todo** )
    - PNG ( **todo** )
    - SVG ( **todo** )
    - LaTeX ( **todo** )
    - Pure MathML ( **todo** )
  - Save As (MathML XML document with additional info as .gplt) ( **todo** )
