# File Modification Checker

This program scans specified directories for files with specific extensions and checks if they have been modified within a given time frame. It allows for customization of directories to scan, file extensions to consider, and directories to ignore.

## Usage

./file_modification_checker <hours>



Replace `<hours>` with the maximum age of files to consider (in hours).

## Configuring Paths, Extensions, and Ignored Folders

### Paths

Paths to directories to scan are read from a config file named `latest.config`. Each path should be on a separate line.

Example `latest.config`:

/Users/username/Documents
/Users/username/Pictures



### Extensions

Extensions to look for are read from a config file named `extensions.txt`. Each extension should be on a separate line and include the period (e.g., `.pdf`, `.txt`).

Example `extensions.txt`:

.pdf
.txt
.java


### Ignored Folders

Folders to ignore are read from a config file named `ignore_me.txt`. Each folder to ignore should be on a separate line.

Example `ignore_me.txt`:


node_modules
temp



## Building the Program

To compile the program, use a C compiler. For example:

gcc -o file_modification_checker file_modification_checker.c

## Running the Program

Run the compiled program with the desired hours as a parameter:


./file_modification_checker <hours>



Replace `<hours>` with the maximum age of files to consider (in hours).

The program will list the files with specified extensions modified within the specified time frame.

## Note

Ensure that the `extensions.txt`, `latest.config`, and `ignore_me.txt` files are present in the appropriate directory structure to define the configuration for paths, extensions, and ignored folders.

~/.latest/config/extensions.txt
~/.latest/config/latest.config
~/.latest/config/ignore_me.txt


