# SHELL

OS assignment #2

## Part A - File Operatations

Implementation of 2 small programs that acts as regular CMD tools.

### Tool 1: `cmp`

Tool to **compare** two files.

* Returns an `INT`:
    * `0` if the two files are ***EQUAL***.
    * `1` if the two files are **NOT EQUAL**.
* Flag support:
    * `-v` - Print `equal` or `distinct` (in addition to returning the int value).
    * `-i` - Ignore upper/lower case while comparing the files.

#### Command

To compare files `<file_1>` and `<file_2>`:

* Compare only: `./cmp <file_1_path> <file_2_path>`
* Compare and print result: `./cmp <file_1_path> <file_2_path> -v`
* Compare ignoring upper/lower case: `./cmp <file_1_path> <file_2_path> -i`
* Compare ignoring upper/lower case and print result: `./cmp <file_1_path> <file_2_path> -v -i`

### Tool 2: `copy`

Tool to copy a file to another place and/or name. The tool wil create a new file, but it will not overwrite it if
already exists (unless forced to).

* Returns an `INT`:
    * `0` if copy succeeded.
    * `1` if copy failed.
* Flag support:
    * `-v` - Print `success`, `target file exist` or `general failure` accordingly  (in addition to returning the int
      value).
    * `-f` - Force OVERWRITE (if target file already exists).

### Command

To copy file `<og_file>` to `<target_file>`:

* Copy only (no overwrite): `./cmp <og_file_path> <target_file_path>`
* Copy and print result (no overwrite): `./cmp <og_file_path> <target_file_path> -v`
* Copy allowing overwite: `./cmp <og_file_path> <target_file_path> -f`
* Copy allowing overwite and print result: `./cmp <og_file_path> <target_file_path> -v -f`

## Part B - Shared Libraries

Implementation of 2 tools  `encode` and `decode`, utilizing the 2 shared libraries `codecA` and `codecB`.

The tools will get a string and convert it according to the selected library.

### `codecA`

Covert all lower case chars to upper case, and all upper case to lower case.
All other chars will remain unchanged.

### `codecB`

Convert all chars to the 3rd next char (add 3 to the ASCII value).

### Command

* `./encode codecA 'aVBa\$aS dF\!c'` will outupt: `AvbA$As Df!C`
* `./decode codecB EEEddd` will output `BBBaaa`

## Part C - Basic Shell

Implementation of a shell program named stshell (st for students).

### Features

* Run CMD tools existing on system.
* Stop running tool by `ctrl-c` (NOT killing the shell itself).
* Redirect output with `>`, replacing the corrent content.
* Redirect output with `>>`, adding to the current content.
* Allow piping with `|` (for at least 2 consecutive pipes).
* Stop the shell by `exit` command.