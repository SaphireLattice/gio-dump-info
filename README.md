# GIO file associations info print

Just a little utility to query GIO's GAppInfo "database" for what application is responsible for a given mimetype.

Ideally it would be nice to print the subclass/alias info from the relevant XML(s) but GIO does not export the header files for its own handling of xdgmime XML and such.

Usage:
- Compile with CMake (e.g. `mkdir build && cd build && cmake .. && make`)
- `giotest application/rss+xml`
- Tada! Question why the application at the very top handles this mimetype (e.g. Firefox using defaults to open RSS and being one to handle that so it opens it again and again)
