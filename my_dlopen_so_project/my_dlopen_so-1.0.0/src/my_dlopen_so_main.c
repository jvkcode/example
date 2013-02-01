#define _GNU_SOURCE             // for use Dl_info dladdr

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>               // readlink
#include <stdint.h>               // gives 32 or 62 
#include <dlfcn.h>                // _GNU_SOURCE extensions here

#define LOCAL_MAX_PATH 500        // max char for /path/to/lib.so
/* GLOBAL VARIABLE */
void *handle = NULL;              // to load shared lib
typedef void (*my_funct)();       // to get function from shared lib
my_funct my_dlopen_libs_info;     // give my function name

// GNU constractor - works before main starts
// loads shared lib from current dir
// gets address of load library function
// if success - handle is not NULL
void __attribute ((constructor))  
my_dlopen_init(void)
{
    static char *lib_so_name = "libmy_dlopen_so.so.4\0"; // lib.so name
    int i=0,len=0, lib_so_name_len=25;                   // lib.so length
    char *error = NULL, *p = NULL;                       
    Dl_info dl_info;                                     // _GNU_SOURCE

    printf("Constructor: ");                             // report
    // obtain how we run this program with PATH or /path/exec
    if(dladdr((void*)"my_idlopen_init", &dl_info) == 0)
    {
        printf("cannot be found %s\n", dlerror());       // exit if error
        return;
    }

    p = NULL;
    // find if we got absolute or relative path
    for(i=0;dl_info.dli_fname[i] != '\0';i++);          // get length
    len = i;                                            // of /path/exec
    for(;len>= 0 && dl_info.dli_fname[len] != '/';len--);// or exec only
    // if no path is given
    // we might use PATH or /proc/self/exe (linux only)
    if(len < 0)
    {
        if((p = malloc(sizeof(char)*LOCAL_MAX_PATH)) == NULL)
        {
            printf("Cannot allocate memory\n");
            return;
        } 
        // works on linux but not on Mac OS X   
        if((len = readlink("/proc/self/exe",p,LOCAL_MAX_PATH)) < 0)
        {
            printf("Error resolving symlink /proc/self/exe\n");
            printf("PATH search is not implemented\n");
            return;
        }
        // if len is exceed given size - exit function
        p[len] = '\0';
        if( len > LOCAL_MAX_PATH - lib_so_name_len)
        {
            printf("Path is exced limit %s\n",p);
            return;
        }
        for(;len>= 0 && p[len] != '/';len--); // find the last '/'
    }
 
    // at this point /path/to/binary is found
    if(p== NULL) // copy /path from struct to p
    {
        if((p = malloc(sizeof(char)*(len + lib_so_name_len))) == NULL)
        {  
            printf("Cannot allocate memory\n");
            return;
        }      
        for(i=0;i <=len && dl_info.dli_fname[i] != '\0';i++)
        {
            p[i] = dl_info.dli_fname[i];
        }
    }
    // add shared libirary name that hardcoded in lib_so_name
    p[len++] = '/';
    for(i=0;lib_so_name[i] != '\0';i++)
    {
        p[len++] = lib_so_name[i];
    }
    p[len] = '\0';

    // try to load library with absolute or relative path
    if((handle = dlopen(p, RTLD_LAZY)) == NULL)
    {
        printf("Error on %s\n", dlerror());
        printf("Library must be in the same directory as executable\n");
        return;
    }
    // get function address to execute later in main
    my_dlopen_libs_info =
       (my_funct)dlsym(handle, "my_dlopen_libs_info");
    error=dlerror();
    if(error)
    {
        printf("Error on dlsym: %s\n", error);
        return;
     }
    free(p);
    printf("done its' work!\n");
}

// destructor - works after main is quite
// releases loaded shared library
void __attribute ((destructor))
my_svcs_fnit(void)
{
    if(handle){
        dlclose(handle);
    }
    printf("Destructor.\n");
}

int main(int argc, const char * argv[])
{
    // check if our shared library is loaded
    if(handle == NULL)
    {
        printf("main: Fails to load shared library\n");
        return 1;
    }
    // use function from loaded shared library
    my_dlopen_libs_info();

    return 0;
}
