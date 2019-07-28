/**
 * Tarea # 1
 * Sistemas Empotrados de Alto Desepeno
 * Integrantes: 
 *            - Fernando Paris
 *            - Esteban Rivera
 *            - Kevin Viquez
 * 
 *  REFERENCES: 
 *            - https://linux.die.net/man/3/fork
 *            - http://www.goldsborough.me/c/low-level/kernel/2016/08/29/16-48-53-the_-ld_preload-_trick/
 *            - https://github.com/tharina/heap-tracer
 *            - https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/
 *            - https://codeday.me/es/qa/20181230/35479.html
              - https://stackoverflow.com/questions/27541910/how-to-use-execvp
 */

//#include <config.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET "\x1b[0m"

/**
 * Open a terminal and executes the command. 
 * Using Fork. 
 */
int open_terminal(char* filename)
{
  pid_t child;              // the pid of the child process
  child = fork();
  if( child == 0 )
  {
    // Printing the entire command didn't work. So using setenv
    // for uploading the env variable before executing the commmand.
    setenv("LD_PRELOAD","../lib/libmemecheck.so",1);
    char* param[] = {filename, NULL};

    if (execvp(filename, param) < 0) 
    {
      printf( RED "*** ERROR: exec failed\n" RESET);
      exit(1);
    }
    
  } 
  else
  {
    exit( 0 );
  }
}

/**
 * Verifies if the file exists. 
 * If not abort the program.
 */
void FileExist(char* filename)
{
  if( access( filename, F_OK ) != -1 ) 
  {
    // file exists
    printf( CYAN "file %s exist! \n" RESET , filename);
  }
  else
  {
    // file doesn't exist
    printf( RED "file %s does not exist! \n" RESET, filename);
    abort();
  }
}

/**
 * Displays the author information in console.
 */
void displayAuthorInfo()
{
  printf ( YELLOW "/**\n  * Tarea # 1\n  * Sistemas Empotrados de Alto Desepeno\n  * Integrantes:\n  *            - Fernando Paris\n  *            - Esteban Rivera\n  *            - Kevin Viquez\n  *  \n  */ \n" RESET );
}

void displayHelp()
{
  printf( YELLOW "\n /*************** Help Information ***************/ \n");
  printf( YELLOW "     -a  Displays author information.  \n");
  printf( YELLOW "     -p  Argument: File (Path)/name. File to be proccessed by this application.  \n");
  printf( YELLOW "     -h  Displays this help.  \n" RESET );
}

/**
 * main method of the program
 */
int main (int argc, char **argv)
{


  int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int index;
  int c;

  opterr = 0;
  char* name;


  while ((c = getopt (argc, argv, ":p:ah")) != -1)
    switch (c)
      {
      case 'a':
        displayAuthorInfo();   
        break;
      case 'p':
        name = optarg;
        FileExist(name);
        if( open_terminal(name) != 0 ) 
        {
          fprintf( stderr, RED "\n  Could not create terminal! \n" RESET );
          return -1;
        }
        break;
      case 'h':
        displayHelp();
        return 1;
      case '?':
        printf("unknown option: %c\n", optopt);
        break;
      default:
        displayHelp();
      }

  return 0;
}