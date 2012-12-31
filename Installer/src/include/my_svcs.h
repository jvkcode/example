//
//  my_svcs.h
//  my_svcs
//
//  Created by Julia Karastoianova on 12/17/12.
//  Copyright (c) 2012 Julia Karastoianova. All rights reserved.
//
//
// my_svcs.h
//
// Created by Julia Karastoianova on 12/12/18
//

#ifndef _MY_SVCS_H_
#define _MY_SVCS_H_

/* HEADERS */
#include <stdio.h>

/* MACROS*/
#define MAX_LEN 255

/* define command line options by number*/
#define COMMAND  1
#define OPTION   2
#define TODO     3

/* define all commands by number here */
#define HELP     0
#define INIT     1
#define HISTORY  2
#define COMMIT   3
#define CHECKIN  4
#define CHECKOUT 5
#define TAG      6

#define INVALID -1

#define DEBUG 1

/* STRUCTURES*/
struct name
{
    char n[MAX_LEN];
};

// dynamic array holds string as filename
struct files_list
{
    struct name *f_name;
    int size;
    int max;
};

struct arguments
{
    char command[MAX_LEN];
    int cmd_ind;
    char option[MAX_LEN];
    char path[MAX_LEN];          // /path/.
    char trunk[MAX_LEN];         // /path/trunk
    char tag_path[MAX_LEN];      // /path/tagN
    char revs[MAX_LEN];          // /path/revsX
    char revs_control[MAX_LEN];     // /path/.mysvcs/revs
    char mysvcs[MAX_LEN];        // /path/.mysvcs
    char mysvcsconf[MAX_LEN];    // /path/.mysvcs/conf
    char mysvcsconfconf[MAX_LEN];// /path/.mysvcs/conf/mysvcs.conf
    char mysvcspasswd[MAX_LEN];  // /path/.mysvcs/conf/passwd
    char mysvcssys[MAX_LEN];     // /path/.mysvcs/conf/sys
    char pwd[MAX_LEN];           // from wich dir command is called
    char working_copy[MAX_LEN];  // /path/working_dir
    char wc_dir_opt[MAX_LEN];    // command line dir or /pat/dir
    char wc_metadata[MAX_LEN];   // /path/working_dir/.metadata
    char wc_revs[MAX_LEN];       // one of /path/revsX
    char tag_name[MAX_LEN];
    char dir_misc[MAX_LEN];      // for diff needs
    char ds[2];
    struct files_list *in;
    int head;    // the latest revision in repository
    int wc_head; // working copy head
    int base;    // in working directory unmodified
    int checkin; // checked back to repository
    int prev;    // checkin-1;
};

/* FUNCTIONS */

// my_svcs_defines.h
char *svcs_commands(int i);    // commands list
char *svcs_mysvcs_dir();       // ".mysvcs\0";
char * svcs_revs_controldir(); // ".mysvcs/revs\0";
char * svcs_conf_dir();        // ".mysvcs/conf\0"
char *svcs_conf_sys();         // ".mysvcs/conf/sys\0";
char *svcs_conf_pw();          // ".mysvcs/conf/passwd";
char *svcs_conf_config();      // ".mysvcs/conf/mysvcs.conf\0";
char * svcs_working_copy();    // default "svcs_working_copy\0"
char *svcs_trunk_dir();        // "trunk";
char *svcs_working_metadata(); // .metadata
char *svcs_readme();           // "README.mysvcs\0";
char *svcs_usr();              // "mysvcs_user\0";
char *svcs_passwd();           // "mysvcs_user123\0";
char *svcs_revision(int r);    // revX
char *svcs_tagNo(int r);       // tagX

void print_help_out();
void svcs_util_chomp(char *w);
int svcs_util_init_files_list(struct files_list **array,int capacity);
void svcs_util_strcat(char *tostr, char *s2, int count, ...);
int svcs_util_valid_file(char *file);
void svcs_util_set_ds(struct arguments *arg, char *path);
int svcs_util_mkdir( char *new_dir, char ds);
int svcs_saved_command(struct arguments *arg, const char *cmd);
int svcs_checkin_option(struct arguments *arg, const char *opt);
int svcs_checkout_option(struct arguments *arg, const char *opt);
int svcs_util_wc_metadata_path(struct arguments *arg, int create);
int svcs_util_save_metadata_in_wd(struct arguments *arg, int create);
void svcs_util_set_reps_dirs(struct arguments *arg);

int svcs_saved_others(struct arguments *arg,const char *args[],int opt, int all);
void svcs_util_init_args(struct arguments *arg);

int svcs_util_pwd(struct arguments *arg);
int svcs_util_wc_metadata_info(struct arguments *arg);
int scvs_util_ask_passwd(struct arguments *arg);
int svcs_get_command_line_args( struct arguments *arg, int all, const char *args[]);


void svcs_init_repository(struct arguments *arg);
void svcs_create_repository(struct arguments *arg);
int svcs_check_repository(struct arguments *arg);

int svcs_util_cp_file_to_dir(char *from_dir, char *file_name, char *to_dir, char *ds);
int svcs_util_cp_files(char *from_dir, char *to_dir, char *ds);

int svcs_util_set_head(struct arguments *arg);
int svcs_checkout_revision(struct arguments *arg);
void svcs_checkout(struct arguments *arg);

int svcs_checkin_add_record(struct arguments *arg, char *path_record, char *record);
int svcs_util_setup_wc_values(struct arguments *arg, int create);
int svcs_checkin_save_new_revision(struct arguments *arg);
void svcs_commit(struct arguments *arg);
void svcs_checkin(struct arguments *arg);

void svcs_util_print_history(struct arguments *arg);
void svcs_history(struct arguments *arg);
void svcs_tag(struct arguments *arg);



/* init repository creates
 ./.mysvcs
 ./.mysvcs/conf
 ./.mysvcs/conf/mysvcs.conf
 ./.mysvcs/conf/passwd
 ./.mysvcs/conf/sys
 ./.mysvcs/revs
 ./.mysvcs/revs/rev0
 ./.mysvcs/revs/tag0
 ./README.mysvcs
 ./trunk
 */

#endif /*_MY_SVCS_H_ */
