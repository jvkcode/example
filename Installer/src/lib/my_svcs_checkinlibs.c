//
//  my_svcs_libcheckin.c
//  my_svcs
//
//  Created by Julia Karastoianova on 12/17/12.
//  Copyright (c) 2012 Julia Karastoianova. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#include "my_svcs.h"

int svcs_checkin_option(struct arguments *arg, const char *opt)
{
    int i= 0;
    char *option_list[] = {"--all\0", "\0\0"};
    
    for(i=0;option_list[i][0] != '\0';i++)
    {
        if(strcmp(opt,option_list[i]) == 0)
        {
            strcpy(arg->option,opt);
            break;
        }
    }
    if(arg->option[0] == '\0')
    {
        printf("Invalid option: %s\nUse help\n", opt);
        return 1;
    }
    return 0;
}
int svcs_checkin_add_record(struct arguments *arg, char *path_record, char *record)
{
    FILE *f = NULL;
    time_t tm;
    long size = 0;
    void * data = NULL;
    
    if((f=fopen(path_record,"rb")) == NULL)
    {
        printf("Checkin fail: Corrupted revision control\n");
        return 1;
    }
    fseek(f,0,SEEK_END);
    
    size = ftell(f);
    fseek(f,0,SEEK_SET);
    data = malloc(sizeof(size));
    fread(data,1,size,f);
    f=freopen(path_record,"wb",f);
    // write to file
    tm   = time(&tm);
    fprintf(f,"%s:%li\n",record, tm);
    fwrite(data,1,size,f);
    fclose(f);
    free(data);
    
    return 0;
}

// copy file(s) from working directory to repository
int svcs_checkin_save_new_revision(struct arguments *arg)
{
    char tmp[MAX_LEN];
    int i = 0;
    FILE *f = NULL;
    DIR *dir = NULL;
    struct dirent *dr = NULL;
    
    if(arg == NULL)
    {
        return 1;
    }
    // copy checkin file(s) to trunk depending on --all option
    if(arg->option[0] != '\0')
    {
        if(svcs_util_cp_files(arg->working_copy, arg->trunk, arg->ds) != 0)
        {
            remove(arg->revs); // remove dir before exit
            printf("Fail save files in trunk\nTry again\n");
            return 1; // cannot copy files
        }
    } else if(arg->in != NULL)
    {
        for(i=0;i<arg->in->size;i++)
        {
            svcs_util_strcat(tmp,arg->trunk,2,arg->ds,arg->in->f_name[1].n);
            if((f = fopen(tmp,"r")) != NULL)
            {
                fclose(f);
                unlink(tmp);
            }
            if(svcs_util_cp_file_to_dir(arg->working_copy, arg->in->f_name[i].n, arg->trunk,arg->ds) != 0)
            {
                remove(arg->revs); // remove dir before exit
                printf("Cannot complete checkin in\n");
                // implement here: copy previous revission into trunk
                return 1;
            }
        }
    } else
    {
        printf("Corrupted data,try again\n");
        return 1;
    }
    // copy all files from trunk to new revisionNo
    if(svcs_util_mkdir(arg->revs,arg->ds[0]) != 0)
    {
        printf("Cannot create directory: %s\n",arg->revs);
        return 1;
    }
    if((dir = opendir(arg->trunk)) == NULL)
    {
        printf ("Cannot save trunk files in: %s",arg->trunk);
        return 0;
    }
    while((dr = readdir(dir)) != NULL)
    {
        if(svcs_util_valid_file(dr->d_name) == 0)
        {
            if(svcs_util_cp_file_to_dir(arg->trunk, dr->d_name, arg->revs,arg->ds) != 0)
            {
                printf("Cannot copy files from trunk to: %s\n", arg->revs);
                closedir(dir);
                return 1;
            }
        }
    }
    closedir(dir);
    return 0;
}

