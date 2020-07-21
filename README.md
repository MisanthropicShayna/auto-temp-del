# Automatic Temporary Folder Deleter
I've always hated the fact that whenever I create temporary files such as quick disposable test programs, I never end up deleting them like I imagined I would when choosing names like "deleteme.txt" or "useless.cpp", until eventually my desktop is littered with temp files. So as any programmer would, I decided to automate the deletion process :)

This program monitors a specified folder for files that haven't been modified for a long period of time (configurable)
and automatically deletes them if the containing folder hasn't been deemed modified for the same period of time (prevents the process from deleting files just for having old attributes, as adding the file causes the folder to be considered modified).