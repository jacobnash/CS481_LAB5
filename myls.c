#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define mmalloc(type, count) ( type * ) malloc ((count) * sizeof(type))
#define isFile_1 0x8
struct flags {
    int c;
    int d;
    int l;
    int f;
    int r;
    int h;
}flag;
int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
void usage()
{
    fprintf(stderr, "\n usage: \n\tmyls [OPTIONS] ... [FILE] ...");
    fprintf(stderr, "\n options:\n\t-c, -classify: append file type indicators: ’/’ for directories; ’@’ for symbolic links; ’*’ for executables.");
    fprintf(stderr, "\n\t-d, –disk-usage: specify file size on disk, based on its number of allocated blocks. This option takes the file system block size (in bytes) as a mandatory parameter.");
    fprintf(stderr, "\n\t-l, –long-listing: use detailed (long) listing format, printing in order:");
    fprintf(stderr, "\n\t\t– inode number");
    fprintf(stderr, "\n\t\t– mode (using the same “drwxrwxrwx” format of the standard ls command. – owner (you must convert the uid to the proper user name)");
    fprintf(stderr, "\n\t\t– group (you must convert the gid to the proper group name)");
    fprintf(stderr, "\n\t\t– file size (in bytes unless otherwise specified)");
    fprintf(stderr, "\n\t\t– file modification time");
    fprintf(stderr, "\n\t-f, –follow-symlinks: follow the targets of symbolic links 1");
    fprintf(stderr, "\n\t-h, –human-readable: if long listing format is specified, print file sizes in a human readable format.");
    fprintf(stderr, "\n\t-r, –recursive: list subdirectories recursively using depth-first, pre-order traversal.");
    fprintf(stderr, "\n");
    exit(0);
}
void my_ls(char** argv)//assume that i only pass dirs
{

    struct dirent *dirent_p;
    DIR           *dir_p;
    FILE          *file_p;
    struct stat   *stats;
    struct stat   *nlstats;
    stats = mmalloc(struct stat, 1);
    nlstats = mmalloc(struct stat, 1);

    //print out all the files
    char **temp = argv;
    //check if it is a directory
    temp = argv;
    if(strcmp(".",*temp)!=0) 
        fprintf(stdout, "%s:\n", *temp);
    while(*temp)
    {
        if((dir_p = opendir(*temp)) != NULL)
        {
            while((dirent_p =readdir(dir_p)) != NULL)
            {
                lstat(dirent_p->d_name, stats);
                if (flag.l)
                {
                    fprintf(stdout, "%llu ", dirent_p->d_ino);//inode numbers
                    fprintf(stdout, (S_ISDIR(stats->st_mode)) ? "d" : "-");
                    fprintf(stdout, (stats->st_mode & S_IRUSR) ? "r" : "-");
                    fprintf(stdout, (stats->st_mode & S_IWUSR) ? "w" : "-");
                    fprintf(stdout, (stats->st_mode & S_IXUSR) ? "x" : "-");
                    fprintf(stdout, (stats->st_mode & S_IRGRP) ? "r" : "-");
                    fprintf(stdout, (stats->st_mode & S_IWGRP) ? "w" : "-");
                    fprintf(stdout, (stats->st_mode & S_IXGRP) ? "x" : "-");
                    fprintf(stdout, (stats->st_mode & S_IROTH) ? "r" : "-");
                    fprintf(stdout, (stats->st_mode & S_IWOTH) ? "w" : "-");
                    fprintf(stdout, (stats->st_mode & S_IXOTH) ? "x " : "- ");
                    fprintf(stdout, "%s ", getpwuid(stats->st_uid)->pw_name);
                    fprintf(stdout, "%s ", getgrgid(stats->st_gid)->gr_name);
                    fprintf(stdout, "%llu ", stats->st_size);
                    fprintf(stdout, "%lu ", stats->st_mtimespec.tv_sec);
                    /*
                     *
                     * time part here
                     * 
                     */ 
                }
                if(flag.d)
                {
                    fprintf(stdout, "%llu ",(stats->st_blocks));
                }
                fprintf(stdout, "%s", dirent_p->d_name);
                if (flag.c)
                {
                    if ( S_ISDIR(stats->st_mode))
                        fprintf(stdout,"/");
                    else if (S_ISLNK(stats->st_mode)) 
                        fprintf(stdout, "@");
                    else if (stats->st_mode & S_IXUSR) 
                        fprintf(stdout, "*");
                }
                if(flag.f)
                {
                    if(S_ISLNK(stats->st_mode))
                        fprintf(stdout, " -> %s", realpath(dirent_p->d_name, NULL));
                }
                fprintf(stdout, "\n");
            }
        }
            temp++;
    }

}

int main(int argc, char** argv)
{
    struct stat   *stats;
    stats = mmalloc(struct stat, 1);
    int option = 0;
    int index = 0; 
    flag.c = 0;
    flag.d = 0;
    flag.l = 0;
    flag.f = 0;
    flag.r = 0;
    flag.h = 0;

    struct option longopt[] = {
        {"classify",        optional_argument, &flag.c, 'c'},
        {"disk-usage",      optional_argument, &flag.d, 'd'},
        {"long-listing",    optional_argument, &flag.l, 'l'},
        {"follow-symlinks", optional_argument, &flag.f, 'f'},
        {"recursive",       optional_argument, &flag.r, 'r'},
        {"human-readable",  optional_argument, &flag.h, 'h'},
        {0,                 0,           0,               0}
    };

    while((option = getopt_long(argc, argv, "cdlfrh", longopt, &index)) != -1){
        switch(option){
            case 'c':
                ++flag.c;
                break;
            case 'd':
                ++flag.d;
                break;
            case 'l':
                ++flag.l;
                fprintf(stderr, "long listing\n");
                break;
            case 'f':
                ++flag.f;
                fprintf(stderr, "follow links\n");
                break;
            case 'h':
                ++flag.h;
                fprintf(stderr, "human readble\n");
                break;
            case 'r':
                ++flag.r;
                fprintf(stderr, "recursive\n");
                break;
            case '?':
                usage();
            default :
                fprintf(stderr, "ls");
        }
    }
    char **temp; 
    int j = 1;
    if (argc == option)
    {
        ++j;
    }
    temp = mmalloc(char* , (argc - optind + j));
    int i = 0;
    if (argc == optind)
    {
        temp[i] = mmalloc(char, sizeof("."));
        temp[i++] = "."; 
    }
    while (optind  < argc )
    {
        //check for sym link or executible.
        if(is_regular_file(argv[optind]))
        {
            lstat(argv[optind], stats);
            if(flag.d)
            {
                fprintf(stdout, "%llu ",(stats->st_blocks));
            }
            fprintf(stdout, "%s", argv[optind]);
            if (flag.c)
            {
                if ( S_ISDIR(stats->st_mode))
                    fprintf(stdout,"/");
                else if (S_ISLNK(stats->st_mode)) 
                    fprintf(stdout, "@");
                else if (stats->st_mode & S_IXUSR) 
                    fprintf(stdout, "*");
            }
            fprintf(stdout, "\n");
            ++optind;

        }
        else{
            temp[i] = mmalloc(char, sizeof(argv[optind]));
            temp[i++] = argv[optind++];
        }
    }
    temp[i] = NULL; 

    my_ls(temp);

    return 0;
}

