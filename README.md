# Introduction

Wildflower Cross-stitch ("WFXS") allows you to create counted cross-stitch
patterns.

NOTE: This is pre-alpha software, and currently is probably only of interest to
developers.

# Alternatives

My wife cross-stitches, but in our house we try to avoid un-free operating
systems such as Windows due to security and privacy concerns.

Closed-source cross stitch programs exist, but they are only available on
Windows.  These are not viable options for us.  Hence WFXS.

My goal is to have WFXS available on any platform with a C++ compiler.  Help
welcome.  But my interest in cluttering the code with Windows-isms is muted, at
best.

# Building

Building requires CMake.  For most situations, building is as simple as:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

# License

Wildflower Cross-stitch (WFXS)
Copyright (C) 2015 Chuck Coffing <clc@alum.mit.edu>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301  USA
