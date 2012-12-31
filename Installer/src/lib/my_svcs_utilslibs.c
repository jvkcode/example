//
//  my_svcs_utils.c
//  my_svcs
//
//  Created by Julia Karastoianova on 12/17/12.
//  Copyright (c) 2012 Julia Karastoianova. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "my_svcs.h"

void svcs_util_chomp(char *w)
{
    int i = 0;

    if(w == NULL)
    {
        return;
    }
    for(i=0;w[i]!='\n'&&w[i]!='\0';i++);
    w[i]='\0';
}

int svcs_util_init_files_list(struct files_list **array,int capacity)
{
    if(*array != NULL)
    {
        printf("svcs_util_init_files_list: Internal error\n");
        return 1; // do nothing
    }
    if((*array = (struct files_list*)realloc(NULL,sizeof(struct files_list))) == NULL)
    {
        printf("Memory allocation problem\n");
        return 1;
    }
    (*array)->max = capacity;
    if(((*array)->f_name = (struct name*)realloc(NULL,sizeof(struct name)*capacity)) == NULL)
    {
        printf("Memory allocation problem\n");
        return 1;
    }
    return 0;
}

void svcs_util_strcat(char *tostr, char *s2, int count, ...)
{
    int i = 0;
    va_list args;

    if(tostr == NULL || s2 == NULL || count < 0)
    {
        return;
    }
    va_start(args,count);

    strcpy(tostr,s2);
    for(i=0;i<count;i++)
    {
        strcat(tostr,va_arg(args,char*));
    }
    va_end(args);
}

int svcs_util_valid_file(char *file)
{
    char bad = '.';
    char e = '\0';
    // bad file
    if(file[0] == bad && (file[1]== bad || file[1]== e))
    {
        return 1;
    }
    // good file
    return 0;
}

void svcs_util_set_ds(struct arguments *arg, char *path)
{
    char ds[] = "\\/\0";
    char *p = NULL;
    int i = 0;

    if(arg == NULL)
    {
        return; //do nothing
    }
    arg->ds[0] = '\0';
    for(i=0;ds[i] != '\0';i++)
    {
        if((p = strchr(path,ds[i])) != NULL)
        {
            arg->ds[0] = p[0];
            arg->ds[1] = '\0';
            break;
        }
    }
}

int svcs_util_mkdir( char *new_dir, char ds)
{
    char *p = NULL;
    DIR *dir = NULL;

    if( new_dir == NULL || new_dir[0] == '\0')
    {
        return 1;
    }

    if((p=strchr(&new_dir[1],ds)) == NULL)
    {
        printf("Error: invalid dir %s", new_dir);
        return 1;
    }
    p[0]='\0';
    if((dir = opendir(new_dir)) == NULL)
    {
        printf("Error: Root directory has to be in place: %s\n", new_dir);
        p[0] = ds;
        return 1;
    }

    p[0] = ds;
    for (p=strchr(p+1,ds); p; p=strchr(p+1, ds))
    {
        p[0]='\0';
        if (mkdir(new_dir) == INVALID)
        {
            if (errno!= EEXIST) {
                p[0]=ds;
                return 1;
            }
        }
        p[0]=ds;
    }
    if(mkdir(new_dir) == INVALID)
    {
        printf((errno == EEXIST)?"Directory exist: %s\n":"Cannot create dir: %s",new_dir);
        return 1;
    }
    return 0;
}

int svcs_util_cp_file_to_dir(char *from_dir, char *file_name, char *to_dir, char *ds)
{
    char to_file[MAX_LEN];
    char from_f[MAX_LEN];
    FILE *f = NULL;
    long size = 0;
    void * data = NULL;

    if(from_dir == NULL || file_name == 0 || to_dir == NULL || ds == NULL ||
       from_dir[0] == '\0' || file_name[0] == '\0' || to_dir[0] == '\0')
    {
        return 1;
    }
    svcs_util_strcat(to_file, to_dir, 2, ds,file_name);
    svcs_util_strcat(from_f, from_dir, 2,ds,file_name);

    if((f=fopen(from_f,"rb")) == NULL)
    {
        printf("Invalid file: %s\n",file_name);
        return 1;
    }
    fseek(f,0,SEEK_END);

    if((size = ftell(f)) != 0) // if file is not empty
    {
        fseek(f,0,SEEK_SET);
        data = malloc(sizeof(size));
        fread(data,1,size,f);
    }
    fclose(f);
    // write to file
    if((f=fopen(to_file,"wb")) == NULL)
    {
        printf("Cannot open file: %s",to_file);
        if(data) free(data);
        return 1;
    }
    if(data) fwrite(data,1,size,f);
    fclose(f);
    if(data) free(data);

    return 0;
}

int svcs_util_wc_metadata_path(struct arguments *arg, int create)
{
    char metadata_dir[MAX_LEN];

    if(arg == NULL)
    {
        return 1;
    }
    svcs_util_strcat(metadata_dir,arg->working_copy,2,arg->ds,svcs_mysvcs_dir());
    if(create && svcs_util_mkdir(metadata_dir, arg->ds[0]) != 0)
    {
        printf("Cannot create directory: %s\n", metadata_dir);
        return 1;
    }
    svcs_util_strcat(arg->wc_metadata,arg->working_copy,4,arg->ds,svcs_mysvcs_dir(),arg->ds,svcs_working_metadata());
    return 0;
}

int svcs_util_save_metadata_in_wd(struct arguments *arg, int create)
{
    FILE *f = NULL;

    if(svcs_util_wc_metadata_path(arg, create) != 0)
    {
        return 1;
    }

    if((f = fopen(arg->wc_metadata,"w")) == NULL)
    {
        printf("Error open File: %s", arg->wc_metadata);
        return 1;
    }

    fprintf(f,"BASE:%i\nCHECKIN:%i\nREPS:%s",arg->base,arg->checkin,arg->path);
    fclose(f);
    return 0;
}

void svcs_util_set_reps_dirs(struct arguments *arg)
{
    svcs_util_strcat(arg->trunk,arg->path,2,arg->ds,svcs_trunk_dir());
    svcs_util_strcat(arg->revs_control,arg->path,2,arg->ds,svcs_revs_controldir());
    svcs_util_strcat(arg->mysvcs,arg->path,2,arg->ds,svcs_mysvcs_dir());
    svcs_util_strcat(arg->mysvcsconf,arg->path,2,arg->ds,svcs_conf_dir());
    svcs_util_strcat(arg->mysvcsconfconf,arg->path,2,arg->ds,svcs_conf_config());
    svcs_util_strcat(arg->mysvcspasswd,arg->path,2,arg->ds,svcs_conf_pw());
    svcs_util_strcat(arg->mysvcssys,arg->path,2,arg->ds,svcs_conf_sys());
}

void svcs_util_init_args(struct arguments *arg)
{
    char e = '\0';

    arg->command[0] = e;
    arg->cmd_ind = INVALID;
    arg->option[0] = e;
    arg->path[0] = e;           // /path/.
    arg->trunk[0] = e;          // /path/trunk
    arg->tag_path[0] = e;       // /path/tagN
    arg->revs[0] = e;           // /path/revsN
    arg->revs_control[0] = e;   // /path/.mysvcs/revs
    arg->mysvcs[0] = e;         // /path/.mysvcs
    arg->mysvcsconf[0] = e;     // /path/.mysvcs/conf
    arg->mysvcsconfconf[0] = e; // /path/.mysvcs/conf/mysvcs.conf
    arg->mysvcspasswd[0] = e;   // /path/.mysvcs/conf/passwd
    arg->mysvcssys[0] = e;      // /path/.mysvcs/conf/sys
    arg->pwd[0] = e;            // pwd
    arg->working_copy[0] = e;   // directory or /path/dir
    arg->wc_dir_opt[0] = e;     // command line dir or /path/dir
    arg->wc_metadata[0] = e;
    arg->tag_name[0] = e;
    arg->dir_misc[0] = e;       // reserved
    arg->ds[0] = e;
    arg->in = NULL;
    arg->head = INVALID;    // the latest revision in repository
    arg->wc_head = INVALID;    // the working dir head
    arg->base = INVALID;   // in working directory umodified
    arg->checkin = INVALID; // checked back to repository
    arg->prev = INVALID;    // checkin-1;

}

int svcs_util_pwd(struct arguments *arg)
{
    char *tmp = NULL;

    tmp = (char *)realloc(NULL,sizeof(char)*MAX_LEN);
    tmp = getcwd(tmp,MAX_LEN);
    if (errno == ERANGE)
    {
        printf("Working directory name too long\n");
        free(tmp);
        return 1;
    }
    strcpy(arg->pwd,tmp);
    free(tmp);
    return 0;
}

int svcs_util_wc_metadata_info(struct arguments *arg)
{
    FILE *f =0;
    char *p = NULL;
    char working_copy[MAX_LEN];

    if(arg == NULL)
    {
        return 1;
    }
    if((f = fopen(arg->wc_metadata,"r")) == NULL)
    {
        printf("Metadata not found\nDo checkout again\n");
        return 1;
    }
    // get repository location
    arg->path[0] = '\0';
    while(fgets(working_copy,MAX_LEN,f))
    {
        p=strchr(working_copy,':');
        p[0]='\0';
        if(strcmp(working_copy,"BASE") == 0)
        {
            arg->base = atoi(&p[1]);
        }
        else if(strcmp(working_copy,"CHECKIN") == 0)
        {
            arg->checkin = atoi(&p[1]);
        }
        else if(strcmp(working_copy,"REPS") == 0)
        {
            strcpy(arg->path,&p[1]);
            svcs_util_chomp(arg->path);
            break;
        }
    }
    fclose(f);
    if(arg->path[0] == '\0')
    {
        printf("Cannot find repository\nMake checkout again\n");
        return 1;
    }
    return 0;
}

int scvs_util_ask_passwd(struct arguments *arg)
{
    char usr[MAX_LEN];
    char passwd[MAX_LEN];
    char r_usr[MAX_LEN];
    char r_passwd[MAX_LEN];
    int i =0;
    int try = 3;
    char *p = NULL;
    FILE *f = NULL;

    // get saved r_usr and r_passwd
    if((f = fopen(arg->mysvcspasswd,"r")) == NULL)
    {
        printf("Reset your passwd: %s\n", arg->mysvcspasswd);
        return 1;
    }
    if(fgets(r_usr,MAX_LEN,f) == NULL)
    {
        printf("Cannot read file: %s",arg->mysvcspasswd);
        fclose(f);
        return 1;
    }
    fclose(f);
    p=strchr(r_usr,':');
    if(p == NULL)
    {
        printf("passwd is corrupted. Fix it\n");
        return 1;
    }
    p[0]='\0';
    strcpy(r_passwd,&p[1]);
    for(i=0;r_passwd[i]!='\n';i++);
    r_passwd[i]='\0';
    try = 3;
    do
    {
        printf("mysvcs user name: ");
        scanf("%s",usr);
        // remove new line if any
        for(i=0;usr[i]!='\n' && usr[i]!='\0';i++);
        usr[i]='\0';

        printf("mysvcs password: ");
        scanf("%s",passwd);
        // remove new line
        for(i=0;usr[i]!='\n' && usr[i]!='\0';i++);
        usr[i]='\0';

        try=(strcmp(r_usr,usr) == 0 && strcmp(r_passwd,passwd) == 0)?0:try-1;

    }while(try);

    return 0;
}

int svcs_util_cp_files(char *from_dir, char *to_dir, char *ds)
{
    DIR *dir = NULL;
    struct dirent *dn;

    // open directory or exit
    if((dir = opendir(from_dir)) == NULL)
    {
        printf("Unable open %s",from_dir);
        return 1;
    }
    // copy files to directory
    while((dn = readdir(dir)) != NULL)
    {
        if(dn->d_name[0] != '.' && svcs_util_cp_file_to_dir(from_dir,dn->d_name,to_dir, ds) != 0)
        {
            printf("Unable copy file:%s\n",dn->d_name);
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

int svcs_util_set_head(struct arguments *arg)
{
    char revs_file[MAX_LEN];
    char revs_num[MAX_LEN];
    FILE *f = NULL;

    svcs_util_strcat(revs_file,arg->revs_control,2,arg->ds,svcs_revision(0));
    if((f = fopen(revs_file,"r")) == NULL)
    {
        printf("Corrapted repository\nCannot open:%s\n",revs_file);
        return 1;
    }
    if((fgets(revs_num,MAX_LEN,f)) != NULL)
    {
        char *p = strchr(revs_num,':');
        if(p == NULL)
        {
            printf("Corrupted record\n");
            fclose(f);
            return 1;
        }
        p[0] = '\0';
        arg->head = atoi(revs_num);
    }
    fclose(f);
    return 0;
}

int svcs_util_setup_wc_values(struct arguments *arg, int create)
{
    DIR *dir = NULL;

    if(arg == NULL)
    {
        return 1; // do nothing
    }
    if(DEBUG) chdir("mytest");

    // this must be working dir
    if((dir = opendir(svcs_mysvcs_dir())) == NULL)
    {
        printf("Data is corupted\n");
        return 1;
    }
    (void)closedir(dir);

    // get working dir metadata info
    if(svcs_util_pwd(arg) != 0)
    {
        return 1; // name too long
    }
    strcpy(arg->working_copy,arg->pwd);
    svcs_util_set_ds(arg,arg->pwd);
    if(svcs_util_wc_metadata_path(arg,create) != 0)
    {
        return 1;
    }
    if(svcs_util_wc_metadata_info(arg) != 0)
    {
        return 1;
    }
    svcs_util_set_reps_dirs(arg);

    // get HEAD revision from repository
    if(svcs_util_set_head(arg) != 0)
    {
        return 1; // not found
    }
    return 0;
}

void svcs_util_print_history(struct arguments *arg)
{
    char rev_path[MAX_LEN];
    char history[MAX_LEN];
    long t = 0;
    char *p = NULL;
    FILE *f = NULL;


    // if no path - exit
    if(svcs_util_wc_metadata_info(arg) != 0)
    {
        return; // corrupted worring copy
    }

    svcs_util_strcat(rev_path,arg->revs_control,2,arg->ds,(arg->cmd_ind == TAG)?svcs_tagNo(0):svcs_revision(0));
    if((f=fopen(rev_path,"r"))== NULL)
    {
        printf((arg->cmd_ind == TAG)?"No tag history\n":"No revision history\n");
        return;
    }
    printf((arg->cmd_ind == TAG)?"TAG : LABEL : DATE\n":"REVISION : DATE\n");
    while(fgets(history,MAX_LEN,f))
    {
        p=strchr(history,':');
        if(arg->cmd_ind == HISTORY)
        {
            p[0] = '\0';
        } else
        {
            p=strchr(&p[1],':');
            p[0]='\0';
        }
        t = atol(&p[1]);
        printf("%s:%s", history,ctime(&t));
    }
}

