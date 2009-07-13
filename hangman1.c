/****************************************************************************************************************
 *     						!!! Hangman !!!                                                 *
 * Players - Two	                                                                                        *
 * Protocol Used - UDP                                                                                          *
 * GCC Version - 4.1.1 20061011 (Red Hat 4.1.1-30)                                                              *
 * Created By :- Anuj Aggarwal (PRN No - 043)                                                                   *
 ***************************************************************************************************************/


#include "hangman.h"           	

#define P1_PORT	 11703 			

#ifdef CURSES
WINDOW *board;
int row,col ;
#endif


/* Signal Handler for SIGINT (ctrl+c)  & SIGTSTP (ctrl+z)  signals */
void sighandler(int signum)
{
#ifdef CURSES
	mvwprintw(board,row-4,20,"Thanks for Playing Hangman !!! Have a nice Day !!!");
	mvwprintw(board,row-3,20,"GAME OVER !!!");
	wrefresh(board);
	refresh();
	sleep(5);
	delwin(board);
	endwin();
#else
	write(STDOUT_FILENO,"\n\t Thanks for playing Hangman !!!\n\n\t Have a nice day !!!\n",57);
	write(STDOUT_FILENO,"\n\t Game Over !!!\n\n",18);
#endif
	exit(3);
}
 

int main(int argc,char *argv[])
{
	int sockfd;
	char more;
	struct sockaddr_in player1,player2 ;
	struct sigaction act; 
	socklen_t player2_len;
	
	/* Create Socket */
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0) { perror(" SOCKFD() failed"); exit(1);}


	/* Construct player 1 address structure */
	bzero(&player1,sizeof(player1));
	player1.sin_family 	= AF_INET;
	player1.sin_addr.s_addr = htonl(INADDR_ANY);
	player1.sin_port 	= htons(P1_PORT);

	player2_len = sizeof(player2);

	/* Bind to the player 1 address */
	if(bind(sockfd,(struct sockaddr *) &player1, sizeof(player1))<0) { perror("BIND() failed"); exit(2);}

	/* Installing Signal Handler for proper termination of the game */
	act.sa_handler=sighandler;
	sigfillset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGINT,&act,NULL);
	sigaction(SIGTSTP,&act,NULL);

#ifdef CURSES
	initscr();
	getmaxyx(stdscr,row,col);
	refresh();

	if(!has_colors())
	{
		endwin();
		fprintf(stderr,"no colors supported\n");
		exit(1);
	}

	if(start_color() != OK)
	{
		endwin();
		fprintf(stderr,"colors cant be initialized\n");
		exit(1);
	}	

	init_pair(1,COLOR_GREEN,COLOR_BLUE);

	board = newwin(row,col,0,0);
	box(board,0,0);
//	wattron(board,COLOR_PAIR(1));
	wrefresh(board);
//	refresh();
//	sleep(1);
	
#endif

	do{
#ifdef CURSES
		wclear(board);
		box(board,0,0);
		wrefresh(board);
		mvwprintw(board,2,20,"WELCOME TO HANGMAN !!!");
		wrefresh(board);
#else
		system("clear");
		printf("\n\t\t\t\t\t WELCOME TO HANGMAN !!!\n");
#endif
		more = play1_game(sockfd,&player2,&player2_len);
	//	printf("\n\n\n The socket no of Player 2 : %d ", ntohs(player2.sin_port));
	//	printf("\n The Ip  of Player 2 : %s ", inet_ntoa(player2.sin_addr));
		if('y' == more)
		{
//			system("clear");
#ifdef CURSES
			flushinp();
			wclear(board);
			box(board,0,0);
			wrefresh(board);
			mvwprintw(board,2,20,"WELCOME TO HANGMAN !!!");
			wrefresh(board);
#else
			printf("\n\t\t\t\t\t WELCOME TO HANGMAN !!!\n");
#endif
			more = play2_game(sockfd,&player2,&player2_len);
			
		}
		else
		{
			break;
		}
		
	}while('y'== more);

	close(sockfd);

#ifdef CURSES
	mvwprintw(board,row-4,20,"Thanks for Playing Hangman !!! Have a nice Day !!!");
	mvwprintw(board,row-3,20,"GAME OVER !!!");
	wrefresh(board);
	refresh();
	sleep(5);
	delwin(board);
	endwin();
	fflush(stdin);
#else
	printf("\n\t Thanks for Playing Hangman !!!\n\n\t Have a nice Day !!!\n");
	printf("\n\t GAME OVER !!!\n");
#endif

	exit(0);
			
}

	


	













