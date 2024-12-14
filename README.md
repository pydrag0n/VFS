# VFS

## cli funcs

- exit

### file

- create <filename\>
- write <filename\> <content (no spaces)> mode
- read <filename\>
- delete <filename\>
- set_permission <filename\> <permission (1...4)>

### user

- login <username\> <password\>
- create_user <username\> <password\> <permission\>
- get_permission <username\>
- set_main_user <username\>

### VFS img save

- save
- load

> file .bin must be located in the same directory
> do not change the name of the file after creating it using the <save\> command

### other

- display_all - print all VFS data

## VFSAPI funcs

[check](src\include\vfs.h)