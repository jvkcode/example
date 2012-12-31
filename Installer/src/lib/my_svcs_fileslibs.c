//
//  my_svcs_defines.c
//  my_svcs
//
//  Created by Julia Karastoianova on 12/17/12.
//  Copyright (c) 2012 Julia Karastoianova. All rights reserved.
//

#include <stdio.h>
char *svcs_commands(int i)
{
    static char *command_list[] = { "help\0\00", "init\0\01", "history\0\02", "commit\0\03", "checkin\0\04", "checkout\0\05", "tag\0\06", "\0\0"};
    return command_list[i];
}

char *svcs_mysvcs_dir()
{
    static char dir[] = ".mysvcs\0";
    return dir;
}
char * svcs_revs_controldir()
{
    static char dir[] = ".mysvcs/revs\0";
    return dir;
}
char * svcs_conf_dir()
{
    static char conf[] = ".mysvcs/conf\0";
    return conf;
}
char *svcs_conf_sys()
{
    static char s[] = ".mysvcs/conf/sys\0";
    return s;
}
char *svcs_conf_pw()
{
    static char pw[] = ".mysvcs/conf/passwd";
    return pw;
}
char *svcs_conf_config()
{
    static char conf[] = ".mysvcs/conf/mysvcs.conf\0";
    return conf;
}
char * svcs_working_copy()
{
    static char dir[] = "svcs_working_copy\0";
    return dir;
}
char * svcs_trunk_dir()
{
    static char dir[] = "trunk";
    return dir;
}
char *svcs_working_metadata()
{
    static char meta[] = ".metadata\0";
    return meta;
}
char *svcs_readme()
{
    static char rm[] = "README.mysvcs\0";
    return rm;
}
char *svcs_usr()
{
    static char usr[] = "mysvcs_user\0";
    return usr;
}
char *svcs_passwd()
{
    static char ps[] = "mysvcs_user123\0";
    return ps;
}
char *svcs_revision(int r)
{
    static char rev[] = "rev123456789123456789\0";
    sprintf(rev,"rev%i",r);
    return rev;
}
char *svcs_tagNo(int r)
{
    static char tag[] = "rev123456789123456789\0";
    sprintf(tag,"tag%i",r);
    return tag;
}