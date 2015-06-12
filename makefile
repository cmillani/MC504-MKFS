all: mkfs bash.fs
	
mkfs:
	gcc mkfs.c mkfs_error.c mkfs_parser.c mkfs_create.c mkfs_struct.c -o mkfs
bash.fs:
	gcc bash.fs.c bash_apps/*.c mkfs_error.c mkfs_struct.c bash.fs_parser.c bash.fs_bsmode.c bash.fs_reader.c Stack/stack.c -o bash.fs
clean:
	rm bash.fs mkfs
