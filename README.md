# Automatic Temporary Folder Deleter
I've always hated the fact that whenever I create temporary files such as quick disposable test programs, I never end up deleting them like I imagined I would when choosing names like "deleteme.txt" or "useless.cpp", until eventually my desktop is littered with temp files. So as any programmer would, I decided to automate the deletion process :)

This program monitors a specified folder for files that haven't been modified for a long period of time (configurable) and automatically deletes them if the containing folder hasn't been deemed modified for the same period of time (prevents the process from deleting files just for having old attributes, as adding the file causes the folder to be considered modified).

### Compiling
Assuming you have version of clang that supports C++17 (`std::filesystem`) in your path, and are on Windows, then running the makefile should be all you need to do to compile. Compiling on Linux should be easy too, as the only Windows specific code is a single line that hides the console window, which you can just replace with the Linux equivalent. Either way, it's a single source file, and not a large one at that, so compiling it should be trivial for anyone who knows their way around a compiler.

### Pre-compiled
If you don't have access to a compiler (a user? browsing github?!) then you can check the releases section for one. There will probably be one available by the time you're reading this. 

### Usage.. well a quote of the `-h` page really
```
autotempdel.exe --help
```
```
--target (-t) [target_directory]                Required argument, specifies the directory that should be watched.

--max-time-seconds (-mt) [seconds]              The maximum amount of time that a file can remain unmodified within
                                                the folder, before it is deleted. This argument also applies to the
                                                amount of time that the target folder can remain unmodified for
                                                (adding a file counts as modifying) before its files are evaluated.
                                                This means that old files added to a folder will not immediately meet
                                                the condition, and won't get deleted until the folder's last modification
                                                date exceeds this argument. Default: 21600 Seconds (6 hours).

--scan-frequency-ms (-sf) [milliseconds]        The frequency at which the target folder should be scanned for changes.
                                                Default: 3600 Seconds (1 hour).

--background (-b)                               Hide the main window of the program when monitoring.

--help (-h)                                     Shows this help text.
```
