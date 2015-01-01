The hush shell for GNO
======================

This is a port of the hush shell to the GNO multitasking environment for the
Apple IIgs.  Hush is a Bourne/POSIX-style shell that was originally part
of BusyBox.  In general, it implements most of the features you would expect,
although there are a few missing ones (such as tilde expansion and aliases).

Supported features include:

* if/then/elif/else/fi conditionals
* for/in/do/done loops
* while loops
* case/esac
* functions
* redirections (including specifying file descriptors)
* here documents
* arithmetic with `$((...))`
* command substitution with backticks or `$(...)`
* glob pattern expansion (including globbing across multiple directory levels)
* command line editing with history and tab completion
* job control
* and much more...

The (useful) command line options are:

    hush [-nxl] [-c 'SCRIPT' [ARG0 [ARGS]] / FILE [ARGS]]

These all behave similarly to other Bourne/POSIX-style shells (e.g. Bash).
Run `help` within hush to get a list of built-in commands.

Installation
------------
To use hush, you need GNO 2.0.6.  Just copy the "hush" binary to a suitable
location. (You may want to install it as /bin/sh to support portable scripts
that expect a Bourne-type shell in that location).

Building
--------
To build it, you also need ORCA/C 2.1.x and plenty of memory (8MB is enough).
You also need to have a copy of the ltermcap library from GNO 2.0.4 installed
as `/usr/lib/libtermcap.204` (the GNO 2.0.6 version is broken and won't work).
Run `make` or (if the source files don't have correct filetypes) `make build`.
You should also be able to build it on modern *nix systems by running `make`,
although this is meant mainly for testing purposes.

GNO-specific notes and known issues
-----------------------------------
* $PATH is expected to be in the usual GNO format (searched backwards, with 
  spaces as separators).

* File descriptors 0, 1, and 2 appearing in shell operations like redirections
  are translated to 1, 2, and 3, respectively.  This maintains compatibility
  with Unix shell scripts that expect these to be stdin, stdout, and stderr.

* Glob expansion doesn't work with patterns containing : directory separators.
  Use / separators instead.

* In TMTerm, command-line editing operations can produce messed-up formatting.

Startup files
-------------
This version of hush will read commands from several files at startup, if they
are present:

* First, it runs commands from `/etc/hushenv`, then from `$HOME/hushenv`.

* Next, if it is a login shell, it runs commands from `/etc/profile`, then
  from `$HOME/hushprofile` (if it is present) or `$HOME/profile`.

* Finally, if it is an interactive shell, it runs commands from 
  `$HOME/hushrc`.

(It also supports versions of the user startup files with names starting with 
`.`, which will be used preferentially if present, but these names are not 
compatible with ProDOS filesystems.)

The `etc` directory contains sample `hushenv` and `profile` scripts, suitable
for installation in `/etc` or in your home directory.

License
-------
Hush is licensed under the GNU General Public License version 2, contained in
the file LICENSE.  Some individual source files contain code licensed under 
other GPL-compatible permissive licenses.  See the individual source files for
full licensing details and copyright notices.

The compiled hush binary also contains code from the ORCA/C runtime libraries,
to which the following notice applies:

This program contains material from the ORCA/C Run-Time Libraries, 
copyright 1987-1996 by Byte Works, Inc. Used with permission.
