void usage(){
    fprintf(stderr, "\n usage as follows:\n -c, –-classify: append file type indicators: ’/’ for directories; ’@’ for symbolic links; ’*’ for executables.");
    fprintf(stderr,  "\n -d, –disk-usage: specify file size on disk, based on its number of allocated blocks. This option takes the file system block size (in bytes) as a mandatory parameter.");
    fprintf(stderr, "\n-l, –long-listing: use detailed (long) listing format, printing in order:");
    fprintf(stderr, "\n\t– inode number");
    fprintf(stderr, "\n\t– mode (using the same “drwxrwxrwx” format of the standard ls command. – owner (you must convert the uid to the proper user name)");
    fprintf(stderr, "\n\t– group (you must convert the gid to the proper group name)");
    fprintf(stderr, "\n\t– file size (in bytes unless otherwise specified)");
    fprintf(stderr, "\n\t– file modification time");
    fprintf(stderr, "\n-f, –follow-symlinks: follow the targets of symbolic links 1");
    fprintf(stderr, "\n-h, –human-readable: if long listing format is specified, print file sizes in a human readable format.");
    fprintf(stderr, "\n-r, –recursive: list subdirectories recursively using depth-first, pre-order traversal.");
}
void main(int argv, char** argc){
    usage();
}
