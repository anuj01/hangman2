/****************************************************************************************************************
 *     						!!! Hangman !!!                                                 *
 * Players - Two	                                                                                        *
 * Protocol Used - UDP                                                                                          *
 * GCC Version - 4.1.1 20061011 (Red Hat 4.1.1-30)                                                              *
 * Created By :- Anuj Aggarwal (PRN No - 043)                                                                   *
 ***************************************************************************************************************/
#include <stdio.h>              
#include <stdlib.h>             
#include <sys/socket.h>         
#include <netinet/in.h>         
#include <arpa/inet.h>          
#include <unistd.h>             
#include <string.h>
#include <signal.h>

#ifdef CURSES
#include <curses.h>
#endif

#undef DPRINT
#ifdef DEBUG
#	define DPRINT(fmt,arg...) printf(fmt, ## arg);
#else
#	define DPRINT(fmt,arg...)
#endif

#define MAXSIZE 20

#define X_COL   30

#define P1_PORT  11703 		//atoi(argv[2])

#define P2_PORT  11800		//atoi(argv[3])

//char greet_msg[]="!!! WELCOME TO HANGMAN !!!" ;

char play1_game(int sockfd,struct sockaddr_in *player2,socklen_t *player2_len);
char play2_game(int sockfd,struct sockaddr_in *player2,socklen_t *player2_len);
int check_char(char ch,char string[],int pos[]);

struct info{
        int count;
        int lives;
        int invalid;
        int win_flag;
        int pos[20];
};

struct query_hint
{
	int string_len;
	char hint_str[MAXSIZE];
};
