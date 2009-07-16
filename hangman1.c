/* Player 2 */
#include "hangman.h"           	

#ifdef CURSES
WINDOW *board ;
int row,col;
#endif

char greet_msg[]="!!! WELCOME TO HANGMAN !!!" ;

void sighandler(int signum)
{
#ifdef CURSES
	mvwprintw(board,row-4,X_COL,"Thanks for playing Hangman !!! Have a nice day !!!");
	mvwprintw(board,row-3,X_COL,"GAME OVER !!!");
	wrefresh(board);
	sleep(5);
	delwin(board);
	endwin();
#else
        write(STDOUT_FILENO,"\n\t Thanks for playing Hangman !!!\n\n\t Have a nice day !!!\n",57);
        write(STDOUT_FILENO,"\n\t Game Over !!!\n\n",18);
#endif
        exit(1);
}

int main(int argc,char *argv[])
{
	int sockfd;
	char more;
	struct sockaddr_in player1;
	struct sigaction act;
	socklen_t player1_len;

	if(argc != 2) { fprintf(stderr," SYNTAX : %s <player1-IP> \n ", argv[0]); exit(1);} 
	
	/* Create Socket */
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0) { perror(" SOCKFD() failed"); exit(1);}


	/* Construct player 1 address structure */
	bzero(&player1,sizeof(player1));
	player1.sin_family 	= AF_INET;
	player1.sin_port 	= htons(P1_PORT);
	if(inet_pton(AF_INET,argv[1],&player1.sin_addr)<0) { perror(" INET_PTON() failed "); exit(2);}
		

	/* Install the signal handler */	
	act.sa_handler=sighandler;
        sigfillset(&act.sa_mask);
        act.sa_flags=0;
        sigaction(SIGINT,&act,NULL);
        sigaction(SIGTSTP,&act,NULL);

	player1_len = sizeof(player1);

#ifdef CURSES
	initscr();
	getmaxyx(stdscr,row,col);
	refresh();
	if(!has_colors())
	{
		endwin();
		fprintf(stderr,"no colors supported");
		exit(1);
	}

	if(start_color() != OK)
	{
		endwin();
		fprintf(stderr,"colors can't be initialized");
		exit(1);
	}

	init_pair(1,COLOR_GREEN,COLOR_BLUE);

	board = newwin(row,col,0,0);
	box(board,0,0);
//	refresh();
//	sleep(1);
#endif



	do{
#ifdef CURSES
		wclear(board);
		box(board,0,0);
		wrefresh(board);
		mvwprintw(board,2,(col-sizeof(greet_msg))/2,"%s",greet_msg);
		wrefresh(board);
		flushinp();
#else
		system("clear");
		printf("\n\t\t\t\t\t WELCOME TO HANGMAN !!!\n");
#endif
		more = play2_game(sockfd,&player1,&player1_len);
		if('y'==more)
		{
#ifdef CURSES
			wclear(board);
			box(board,0,0);
			wrefresh(board);
			mvwprintw(board,2,(col-sizeof(greet_msg))/2,"%s",greet_msg);
			wrefresh(board);
#else
			system("clear");
			printf("\n\t\t\t\t\t WELCOME TO HANGMAN !!!\n");
#endif

			DPRINT("\n player2 port no = %d", ntohs(player2.sin_port));
			DPRINT("\n player2 IP = %s", inet_ntoa(player2.sin_addr));

			more = play1_game(sockfd,&player1,&player1_len);
		}
		else
		{
			break;
		}
	}while('y'==more);
	
	close(sockfd);
#ifdef CURSES
	mvwprintw(board,row-4,X_COL,"Thanks for playing Hangman !!! Have a nice day !!!");
	mvwprintw(board,row-3,X_COL,"GAME OVER !!!");
	wrefresh(board);
	sleep(5);
	delwin(board);
	endwin();
#else
	printf("\n\t Thanks for playing Hangman !!!\n\n\t Have a nice day !!!\n");
	printf("\n\t GAME OVER !!!\n");
#endif
}
