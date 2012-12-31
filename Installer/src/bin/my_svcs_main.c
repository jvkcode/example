//
//  main.c
//  my_svcs
//
//  Created by Julia Karastoianova on 12/12/12.
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

void print_help_out()
{
    int i = 0;

    printf("Next command can be used:\n");
    for(i=0;;i++)
    {
        char *cmd = svcs_commands(i);
        if(cmd[0] != '\0')
        {
            printf("  %s ", cmd);
        }
        else
        {
            break;
        }
        switch (i)
        {
            case HELP:
                printf("\n");
                break;
            case INIT:
                printf("/path/to/repository\n");
                printf("\t - init repository in given path\n");
                break;
            case HISTORY :
                printf("\n\t- prints revision history\n");
                break;
            case COMMIT:
                printf("\n\t- sorry,not implemented use checkin\n");
                break;
            case CHECKIN:
                printf("[--all|files]\n");
                printf("\t- saves all files or listed back to repository\n");
                printf("\t- required username and password\n");
                break;
            case CHECKOUT:
                printf("[options] /path/to/repository [checkout_dir]\n");
                printf("\toptional: -rN checkout revision number\n");
                printf("\toptional: -tN checkout tag number\n");
                printf("\t- if no option set the lates revision is checked out\n");
                printf("\t- if no checkout_dir, default to working_copy\n");
                printf("\t- NOTE: program does not accept /path/to/checkout_dir\n");
                break;
            case TAG:
                printf(" [TAG_LABEL]\n");
                printf("\t- if TAG_LABEL is not set, tag history is printed\n\n");
                break;
        }
    }
}


int svcs_get_command_line_args( struct arguments *arg, int all, const char *args[])
{
    int opt = 0;

    if(arg == NULL || args == NULL)
    {
        return 1; // do nothing
    }
    if(all < 2)
    {
        printf("Invalid options\nUse help\n");
        return 1;
    }
    svcs_util_init_args(arg);
    for(opt=1;opt<all;opt++)
    {
        switch (opt)
        {
            case COMMAND:
                if(svcs_saved_command(arg,args[opt]) != 0)
                {
                    return 1;
                }
                break;
            case OPTION:
                if(arg->cmd_ind == CHECKIN && args[opt][0] == '-')
                {
                    if(svcs_checkin_option(arg,args[opt]) !=  0)
                    {
                        return 1; // invalid option
                    }
                    break;
                }
                if(arg->cmd_ind == CHECKOUT && args[opt][0] == '-')
                {
                    if(svcs_checkout_option(arg,args[opt]) !=  0)
                    {
                        return 1; // invalid option
                    }
                    break;
                }
                // skip down if no option set
            case TODO:
                if(svcs_saved_others(arg,args,opt,all) != 0)
                {
                    return 1; // invalid argument is found
                }
                opt = all; // exit function we are done
                break;
        } // the end of switch (opt)
    } // the end of for(opt=1;opt<all;opt++)

    return 0;
}


void svcs_create_repository(struct arguments *arg)
{
    DIR *dir = NULL;

    if(arg == NULL)
    {
        return; // do nothing
    }
    // check if path was given
    if(arg->path[0] == '\0')
    {
        printf("Repository path is missing:\n\tinit /path/to/mysvcs\n");
        return;
    }
    // check if given directory exist
    if((dir = opendir(arg->path)) != NULL)
    {
        printf("Cannot init repository\nDirectory exists: %s\n", arg->path);
        closedir(dir);
        return;
    }
    // create repository directory
    if(svcs_util_mkdir(arg->path,arg->ds[0]) != 0)
    {
        return;
    }
    // init repository directory with complete tree
    svcs_init_repository(arg);
    printf("Repository is created in %s\n",arg->path);
    printf("Please read %s file and modify login\n",svcs_readme());
    printf("Good luck\n");
}

int svcs_check_repository(struct arguments *arg)
{
    DIR *dir = NULL;

    if(arg == NULL)
    {
        return 1;
    }
    if((dir = opendir(arg->path)) == NULL)
    {
        printf("Invalid repository: %s\n",arg->path);
        return 1;
    }
    closedir(dir);
    return 0;
}


void svcs_checkout(struct arguments *arg)
{
    if(arg == NULL)
    {
        return; // do nothing
    }
    if(svcs_check_repository(arg) != 0)
    {
        return; // invalid path to repository
    }
    // get curent directory
    if(svcs_util_pwd(arg) != 0)
    {
        return; //dir name too long
    }
    // working directory with given name or hardcoded
    svcs_util_strcat(arg->working_copy,arg->pwd,2,arg->ds,arg->wc_dir_opt);
    if(svcs_util_mkdir(arg->working_copy,arg->ds[0]) != 0)
    {
        return;
    }
    // get prompted revision == arg->base or head if no prompt
    if(svcs_checkout_revision(arg) != 0)
    {
        return; // not found
    }
    // copy all files into working directory
    if(svcs_util_cp_files(arg->wc_revs, arg->working_copy, arg->ds) != 0)
    {
        return; // cannot copy files
    }
    // create metadata in working directory
    svcs_util_save_metadata_in_wd(arg, 1);
}

void svcs_history(struct arguments *arg)
{
    if(arg == NULL)
    {
        return;
    }
    if(svcs_util_setup_wc_values(arg,0) != 0)
    {
        return; // something wrong
    }
    // if no path - exit
    if(svcs_util_wc_metadata_info(arg) != 0)
    {
        return; // corrupted worring copy
    }
    // print revision history
    svcs_util_print_history(arg);

}

void svcs_commit(struct arguments *arg)
{
    printf("Not implemented, use checkin\n");
}

void svcs_checkin(struct arguments *arg)
{
    char tmp[MAX_LEN];
    char rec[MAX_LEN];

    if(arg == NULL)
    {
        return; // do nothing
    }
    if(svcs_util_setup_wc_values(arg,0) != 0)
    {
        return; // something wrong
    }

    // ask password
    if(scvs_util_ask_passwd(arg) != 0)
    {
        return;
    }

    // if repository head != working_copy base - ask what to do
    if(arg->head > arg->base)
    {
        printf("Your working directory has base revision: %i\n",arg->base);
        printf("Your repository has latest revision: %i\n",arg->head);
        printf("Do merge by hand if needed before continue\n");
        printf("Or type yes to continue checkin: ");
        scanf("%s", tmp);
        if(tmp[0] != 'y' || tmp[1] != 'e' || tmp[3] != 's')
        {
            printf("Checkin is terminated\n");
            return;
        }
    }
    // create new revision directory in repository julia
    arg->checkin=(++(arg->head));
    svcs_util_strcat(arg->revs,arg->path,2,arg->ds,svcs_revision(arg->checkin));
    if(svcs_checkin_save_new_revision(arg) != 0)
    {
        return;
    }
    // update revision record
    svcs_util_strcat(tmp,arg->revs_control,2,arg->ds,svcs_revision(0));
    sprintf(rec,"%i",arg->checkin);
    svcs_checkin_add_record(arg,tmp,rec);

    printf("Checkin is done\n");
    // update metadata in working copy
    svcs_util_save_metadata_in_wd(arg, 0);
}

void svcs_tag(struct arguments *arg)
{
    char rec[MAX_LEN];

    if(arg == NULL)
    {
        return;
    }
    if(svcs_util_setup_wc_values(arg,0) != 0)
    {
        return; // something wrong
    }

    //it must be working copy directory

    // if no tag name - returns all tags
    if(arg->tag_name[0] == '\0')
    {
        svcs_util_print_history(arg);
        return;
    }

    // ask password
    if(scvs_util_ask_passwd(arg) != 0)
    {
        return;
    }
    // create new tag directory
    svcs_util_strcat(arg->tag_path,arg->path,2,arg->ds,svcs_tagNo(arg->head));

    if(svcs_util_mkdir(arg->tag_path,arg->ds[0]) != 0)
    {
        printf("Tag for this revision is in place\n");
        return;
    }
    if(svcs_util_cp_files(arg->trunk, arg->tag_path, arg->ds) != 0)
    {
        remove(arg->tag_path); // remove dir before exit
        printf("Create tag in: %s\n",arg->tag_path);
        return; // cannot copy files
    }
    // create new record in revs for tag only
    svcs_util_strcat(arg->dir_misc,arg->revs_control,2,arg->ds,svcs_tagNo(0));
    sprintf(rec,"%i:%s",arg->head,arg->tag_name);
    svcs_checkin_add_record(arg,arg->dir_misc,rec);

    printf("Tag is created for revision %i\n",arg->head);
}

int main(int argc, const char * argv[])
{
    struct arguments arg_list;

    if(svcs_get_command_line_args(&arg_list, argc, argv) != 0)
    {
        return 1; // invalid command line options
    }
       switch (arg_list.cmd_ind)
    {
        case INIT :
            svcs_create_repository(&arg_list);
            break;
        case CHECKOUT :
            svcs_checkout(&arg_list);
            break;
        case HISTORY :
            svcs_history(&arg_list);
            break;
        case COMMIT:
            svcs_commit(&arg_list); // not imlemented
            break;
        case CHECKIN :
            svcs_checkin(&arg_list);
            break;
        case TAG:
            svcs_tag(&arg_list);
            break;
        case HELP :
            print_help_out();
            break;
    }
    return 0;
}

