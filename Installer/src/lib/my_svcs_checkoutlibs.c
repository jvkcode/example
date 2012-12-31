//
//  my_svcs_libcheckout.c
//  my_svcs
//
//  Created by Julia Karastoianova on 12/17/12.
//  Copyright (c) 2012 Julia Karastoianova. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#include "my_svcs.h"

int svcs_checkout_option(struct arguments *arg, const char *opt)
{
    int i = 0;
    char *opt_l[] = {"-r\0\00\0","-t\0\01\0","\0\0"};
    
    for(i=0;opt_l[i][0] != '\0';i++)
    {
        if(opt[0] == opt_l[i][0] && opt[1] == opt_l[i][1])
        {
            i=atoi(&opt[2]);
            if(i < 0)
            {
                printf("Invalid option format: %s\n",opt);
                return 1;
            }
            strcpy(arg->option,opt);
            arg->base = i;
            break;
        }
    }
    return 0;
}

int svcs_checkout_revision(struct arguments *arg)
{
    DIR *dir = 0;
    
    // get latest revision number
    if(svcs_util_set_head(arg) != 0)
    {
        return 1;
    }
    // if no revision asked give HEAD
    if(arg->base == INVALID || arg->base == arg->head)
    {
        arg->base = arg->checkin = arg->head;
        strcpy(arg->wc_revs,arg->trunk);
    }
    else // see if that is revision or tag
    {
        svcs_util_strcat(arg->wc_revs,arg->path,2,arg->ds,
                         ((arg->option[1] == 'r'))?
                         (svcs_revision(arg->base)):
                         (svcs_tagNo(arg->base)));
    }
    if((dir = opendir(arg->wc_revs)) == NULL)
    {
        printf("Invalid revision: %i\n",arg->base);
        printf("Check option: %s",arg->option);
        rmdir(arg->working_copy);
        return 1;
    }
    (void)closedir(dir);
    
    return 0;
}
