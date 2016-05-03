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
#include <math.h>
#define mmalloc(type, count) ( type * ) malloc ((count) * sizeof(type))

struct flags {
    int c;
    int d;
    int l;
    int f;
    int r;
    int h;
} flag;

void convert(long long unsigned mem)
{
    char* result = "B"; 
    float i_r = 0;
    if(flag.h){    
        if (mem > 0x80000000)
        {
            i_r = (float)mem/0x80000000;
            result = "G";
            i_r = floorf(i_r * 100) / 100;
            fprintf(stdout, "%3.1f%s ",i_r, result);
        }
        else if (mem > 0x100000)
        {
            i_r = (float)mem/0x100000;
            result = "M";
            i_r = floorf(i_r * 100) / 100;
            fprintf(stdout, "%3.1f%s ",i_r, result);
        }
        else if (mem > 0x800)
        {
            i_r = (float)mem/0x800;
            result = "K";
            i_r = floorf(i_r * 100) / 100;
            fprintf(stdout, "%3.1f%s ",i_r, result);
        }
        else
            fprintf(stdout, "%llu%s ",mem, result);
    }
    else
        fprintf(stdout, "%llu", mem);
}

void my_ls_file(char *arg)
{
    struct stat   *stats;
    stats = mmalloc(struct stat, 1);
         lstat(arg, stats);
                if (flag.l)
                {
                    fprintf(stdout, "%llu ", stats->st_ino);//inode numbers
                    fprintf(stdout, (S_ISDIR(stats->st_mode) ) ? "d" : "-");
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
                    if(flag.d)
                    {
                        convert(( ((stats->st_size / flag.d) + ((stats->st_size % flag.d) ? 1 : 0)) * flag.d));
                    }
                    else    
                        convert(stats->st_size);
                    char buf[20];
                    time_t t_now;
                    struct tm *l_time;
                    time(&t_now);
                    int year = localtime(&t_now)->tm_year; 
                    l_time = localtime(&stats->st_mtime);
                    (l_time->tm_year == year)? strftime(buf, 20, "%b %e %R", l_time) : strftime(buf, 20, "%b %e %Y", l_time);
                    printf("%s ", buf);
                }
                fprintf(stdout, "%s", arg);
                if (flag.c)
                {
                    if (S_ISLNK(stats->st_mode)) 
                        fprintf(stdout, "@");

                    else if (stats->st_mode & S_IXUSR) 
                        fprintf(stdout, "*");
                }
                    fprintf(stdout, " -> %s", realpath(arg, NULL));
                fprintf(stdout, "\n");
}
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
    struct stat   *stats;
    stats = mmalloc(struct stat, 1);

    //print out all the files
    char **temp = argv;
    //check if it is a directory
    temp = argv;

    while(*temp)
    {
        fprintf(stdout, "%s:\n", *temp);
        if((dir_p = opendir(*temp)) != NULL)
        {
            while((dirent_p =readdir(dir_p)) != NULL)
            {
                lstat(dirent_p->d_name, stats);
                if (flag.l)
                {
                    fprintf(stdout, "%llu ", dirent_p->d_ino);//inode numbers
                    fprintf(stdout, (S_ISDIR(stats->st_mode) ) ? "d" : "-");
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
                    if(flag.d)
                    {
                        convert(( ((stats->st_size / flag.d) + ((stats->st_size % flag.d) ? 1 : 0)) * flag.d));
                    }
                    else    
                        convert(stats->st_size);
                    char buf[20];
                    time_t t_now;
                    struct tm *l_time;
                    time(&t_now);
                    int year = localtime(&t_now)->tm_year; 
                    l_time = localtime(&stats->st_mtime);
                    (l_time->tm_year == year)? strftime(buf, 20, "%b %e %R", l_time) : strftime(buf, 20, "%b %e %Y", l_time);
                    printf("%s ", buf);
                }
                fprintf(stdout, "%s", dirent_p->d_name);
                if (flag.c)
                {
                    if (DT_DIR == dirent_p->d_type)
                        fprintf(stdout,"/");
                    else if (S_ISLNK(stats->st_mode)) 
                        fprintf(stdout, "@");

                    else if (stats->st_mode & S_IXUSR) 
                        fprintf(stdout, "*");
                }
                if(S_ISLNK(stats->st_mode))
                    fprintf(stdout, " -> %s", realpath(dirent_p->d_name, NULL));
                fprintf(stdout, "\n");
            }
            closedir(dir_p);
        }
        else
        {
            fprintf(stderr, "\n FILE ERROR %s", *temp);
            perror("file does not exsist");
            exit(EXIT_FAILURE);
        }
        temp++;
    }
    temp = argv;

    if(flag.r)
    {
        char** recursive = mmalloc(char*,2); 
        while(*temp)
        {
            if ((dir_p = opendir(*temp)) != NULL)
            {
                while((dirent_p = readdir(dir_p)) != NULL)
                {
                    if(DT_DIR == dirent_p->d_type)
                    {
                        if(!(strcmp(".",dirent_p->d_name)==0 || strcmp("..",dirent_p->d_name) == 0 ))
                        { 
                            recursive[1] = NULL;
                            char* tempstring = NULL;
                            asprintf(&tempstring,"%s/%s",*temp, dirent_p->d_name);
                            fprintf(stdout, "%s:\n", tempstring);
                            recursive[0] = tempstring;
                            my_ls(recursive);
                        }
                    }                    
                }
                closedir(dir_p);
            }
            ++temp;  
        }
        free(recursive);
    }
    free(stats);
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
        {"classify",        no_argument, &flag.c, 'c'},
        {"disk-usage",      required_argument, &flag.d, 'd'},
        {"long-listing",    no_argument, &flag.l, 'l'},
        {"follow-symlinks", no_argument, &flag.f, 'f'},
        {"recursive",       no_argument, &flag.r, 'r'},
        {"human-readable",  no_argument, &flag.h, 'h'},
        {0,                  0,                0,        0 }
    };

    while((option = getopt_long(argc, argv, "cd:lfrh", longopt, &index)) != -1){
        switch(option){
            case 'c':
                ++flag.c;
                break;
            case 'd':
                flag.d =atoi(optarg);
                break;
            case 'l':
                ++flag.l;
                break;
            case 'f':
                ++flag.f;
                break;
            case 'h':
                ++flag.h;
                break;
            case 'r':
                ++flag.r;
                break;
            case '?':
                usage();
        }
    }
    char **temp; 
    int j = 1;
    if (argc == option)  ++j;
    temp = mmalloc(char* , (argc - optind + j));
    int i = 0;
    temp[i] = NULL;
    temp[argc - optind + 1] = NULL;
    asprintf(&temp[i],".");
    while (optind  < argc )
    {
        temp[i] = NULL;
        stat(argv[optind], stats);
        if(S_ISDIR(stats->st_mode)){
               //if(S_ISLNK(stats->st_mode) && )
               //     fprintf(stdout, " -> %s", realpath(dirent_p->d_name, NULL));
            temp[i] = malloc(sizeof(argv[optind])+1);
            strcpy(temp[i], argv[optind]);
            i++;
            optind++;
        }
        else
        {
            my_ls_file(argv[optind++]);
        }

    }
    my_ls(temp);
    return 0;
}
