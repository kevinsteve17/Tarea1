/**
 * Tarea # 1
 * Sistemas Empotrados de Alto Desepeno
 * Integrantes: 
 *            - Fernando Paris
 *            - Esteban Rivera
 *            - Kevin Viquez
 *  
 */

//#include <config.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define YELLOW "\x1b[33m"

char* Command = {"LD_PRELOAD=$PWD/libmemecheck.so "};
char* filepath = "";

/**
 * Creates a terminal for executing the app
 */
int create_terminal()
{

  char terminal_args[256];  // the --pid= arguments fo tail
  pid_t child;              // the pid of the child process 
  pid_t parent;             // the pid of the parent process
  FILE* log;                // log file
  int fn;                   // file number of log


  //terminal_args = "LD_PRELOAD=$PWD/libmemecheck.so  ./case4";

  puts("Got into create terminal");


  //Open the log file 
  log = fopen("/tmp/tarea1.log", "w");
  fn = fileno(log);

  //Get the parent id
  parent = getpid();
  sprintf(terminal_args, "--pid=%d", parent);


  // Create a child process
  child = fork();

  if(child == 0)
  {
    // Replace child process with a gnome-terminal :
    // tail -f /tmp/tarea1.log --pid=<parent_pid>

    execlp("gnome-terminal", "gnome-terminal", "-x", "tail", "-f", "/tmp/tarea1.log", terminal_args, NULL);
    
    // if fails, print the error message and exit
    perror("execlp()");
    exit( -1 );
    
  }
  else
  {
    // Parent process
    close(1); // close stdout

    int ok = dup2(fn, 1); // replace stdout with the file

    if(ok!=1)
    {
      perror("dup2()");
      return -1;
    }

    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  }

  return 0;

}

/**
 * 
 */
int open_terminal(char **argv)
{
  pid_t child;              // the pid of the child process
  child = fork();

  puts("Opening terminal ... ");


  if( child == 0 )
  {
    execlp("gnome-terminal", "gnome-terminal", "ls", argv[0], "--noconsole", NULL );
  } 
  else
  {
    exit( 0 );
  }

}

void generateCommand(char* filename)
{
   char* newCommand = strcat(Command, filename);
   printf("The command is: %s \n", newCommand);

}

void displayAuthorInfo()
{

  printf ( YELLOW "/**\n  * Tarea # 1\n  * Sistemas Empotrados de Alto Desepeno\n  * Integrantes:\n  *            - Fernando Paris\n  *            - Esteban Rivera\n  *            - Kevin Viquez\n  *  \n  */ \n");

}

void displayHelp()
{
  printf( "\n /*************** Help Information ***************/ \n");
  printf( "     -a  Displays author information.  \n");
  printf( "     -p  Argument: File (Path)/name. File to be proccessed by this application.  \n");
  printf( "     -h  Displays this help.  \n");
}

/**
 * main method of the program
 */
int main (int argc, char **argv)
{
  //puts ("Hello World!");
  //puts ("This is " PACKAGE_STRING ".");

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
        printf("file path is: %s \n", name);
        generateCommand(name);
        //if( open_terminal(argv) != 0 ) 
        //{
        //  fprintf( stderr, "Could not create terminal!\n" );
        //  return -1;
        //}
        break;
      case 'h':
        displayHelp();
        return 1;
      case '?':
        printf("unknown option: %c\n", optopt);
        break;
      default:
        displayHelp();
        //abort ();
      }

  return 0;
}