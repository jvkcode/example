//
//  my_svcs_initrepositorylibs.c
//  my_svcs
//
//  Created by Julia Karastoianova on 12/17/12.
//  Copyright (c) 2012 Julia Karastoianova. All rights reserved.
//

#include <stdio.h>
#include <time.h>

#include "my_svcs.h"

void svcs_init_repository(struct arguments *arg)
{
    time_t time_curr;
    FILE *f = NULL;
    char freadme[MAX_LEN];
    
    if(arg == NULL)
    {
        return;
    }
    
    // create readme file
    svcs_util_strcat(freadme,arg->path,2,arg->ds,svcs_readme());
    if((f = fopen(freadme,"w+")) == NULL)
    {
        printf("Cannot open file for writing: %s", freadme);
        return;
    }
    fputs("This is a Simple Version Control System repository; modify files are:\n", f);
    fputs("conf/mysvcs.conf\nconf/passwd\n",f);
    fflush(f);
    fclose(f);
    
    // create trunk dir
    if(svcs_util_mkdir(arg->trunk,arg->ds[0]) != 0)
    {
        return;
    }
    // create /path/.mysvcs
    if(svcs_util_mkdir(arg->mysvcs,arg->ds[0]) != 0)
    {
        return;
    }
    // create /path/.mysvcs/conf
    if(svcs_util_mkdir(arg->mysvcsconf,arg->ds[0]) != 0)
    {
        return;
    }
    // create /path/.mysvcs/revs
    if(svcs_util_mkdir(arg->revs_control,arg->ds[0]) != 0)
    {
        return;
    }
    // create conf/passwd
    if((f = fopen(arg->mysvcspasswd,"w+")) == NULL)
    {
        printf("Cannot open file: %s",arg->mysvcspasswd);
        return;
    }
    fprintf(f,"%s:%s", svcs_usr(), svcs_passwd());
    fflush(f);
    fclose(f);
    
    // create config.mysvcs.config with permition for single usr
    if((f = fopen(arg->mysvcsconfconf,"w+")) == NULL)
    {
        printf("Cannot open file: %s",arg->mysvcsconfconf);
        return;
    }
    fprintf(f,"%s:write",arg->mysvcsconf);
    fflush(f);
    fclose(f);
    
    // create sys file with system info
    if((f = fopen(arg->mysvcssys,"w+")) == NULL)
    {
        printf("Cannot open file: %s",arg->mysvcssys);
        return;
    }
    fprintf(f,"%s:%s",arg->path,arg->ds);
    fflush(f);
    fclose(f);
    
    // set up revision record 0
    svcs_util_strcat(freadme,arg->revs_control,2, arg->ds,svcs_revision(0));
    if((f = fopen(freadme,"w+")) == NULL)
    {
        printf("Cannot open file: %s",freadme);
        return;
    }
    time_curr = time(&time_curr);
    fprintf(f,"0:%li", time_curr);
    fflush(f);
    fclose(f);
    
    // set up tag record 0
    svcs_util_strcat(freadme,arg->revs_control,2, arg->ds,svcs_tagNo(0));
    if((f = fopen(freadme,"w+")) == NULL)
    {
        printf("Cannot open file: %s",freadme);
        return;
    }
    time_curr = time(&time_curr);
    fprintf(f,"0:IT_IS_REPOSITORY_CREATION_TIME:%li", time_curr);
    fflush(f);
    fclose(f);
}
