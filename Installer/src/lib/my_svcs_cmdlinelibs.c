//
//  my_svcs_cmdlinelibs.c
//  my_svcs
//
//  Created by Julia Karastoianova on 12/17/12.
//  Copyright (c) 2012 Julia Karastoianova. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include "my_svcs.h"

int svcs_saved_command(struct arguments *arg, const char *cmd)
{
    int i = 0;
    char *tmp;
    
    if(arg == NULL || cmd == NULL)
    {
        return 1;
    }
    for(i=0;;i++)
    {
        tmp = svcs_commands(i);
        if(tmp[0] == '\0')
        {
            break;
        }
        if(strcmp(cmd,tmp) == 0)
        {
            strcpy(arg->command,tmp);
            arg->cmd_ind = i;
            break;
        }
    }
    if( arg->cmd_ind == INVALID)
    {
        printf("Invalid command: %s\nUse help\n", cmd);
        return 1;
    }
    return 0;
}

int svcs_saved_others(struct arguments *arg,const char *args[],int opt, int all)
{
    int list = 0;
    
    if( arg == NULL || args == NULL || all < 1 || opt < 0)
    {
        return 1; // do nothing
    }
    switch(arg->cmd_ind)
    {
        case INIT:
        case CHECKOUT:
            strcpy(arg->path,args[opt]);
            svcs_util_set_ds(arg,arg->path);
            if(arg->ds[0] == '\0')
            {
                printf("Invalid path: %s\n", arg->path);
                return 1;
            }
            svcs_util_set_reps_dirs(arg);
            
            if(arg->cmd_ind == CHECKOUT)
            {
                strcpy(arg->wc_dir_opt,(opt+1 < all)?args[opt+1]:svcs_working_copy());
            }
            break;
        case CHECKIN: // one or more files listed
            if(arg->option[0] != '\0')
            {
                printf("Error: Use option %s -OR- file(s)\n",arg->option);
                return 1;
            }
            list = all - opt; //number of files in command line
            if(svcs_util_init_files_list(&(arg->in),list+1) != 0)
            {
                return 1; // cannot realocate memory for dynamic array
            }
            for(arg->in->size=0;opt < all;opt++, arg->in->size+=1)
            {
                strcpy(arg->in->f_name->n,args[opt]);
            }
            break;
        case TAG: // tag name to save
            strcpy(arg->tag_name,args[opt]);
            if(opt+1 != all)
            {
                printf("tag accepts one argument\nError: %s\n", args[opt+1]);
                return 1;
            }
            break;
    } // the end of switch(arg->cmd_ind)
    return 0;
}
