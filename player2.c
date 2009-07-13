
/****************************************************************************************************************
 *     						!!! Hangman !!!                                                 *
 * Players - Two	                                                                                        *
 * Protocol Used - UDP                                                                                          *
 * GCC Version - 4.1.1 20061011 (Red Hat 4.1.1-30)                                                              *
 * Created By :- Anuj Aggarwal (PRN No - 043)                                                                   *
 ***************************************************************************************************************/

#include"hangman.h"

extern WINDOW *board;
extern int row,col;

char play2_game(int sockfd,struct sockaddr_in *sendAddr,socklen_t *sendAddr_len)
{
        int i,j;
        int string_len;
	char pick_array[] = "abcdefghijklmnopqrstuvwxyz" ;
        char name[MAXSIZE],ch,*show,more,*string;
        struct sockaddr_in fromAddr;
        socklen_t addrLen;
        struct info update;
	
	/* Initialization of update structure */
        update.count=0;
        update.lives=6;
        update.invalid=0;
        update.win_flag=0;

        addrLen=sizeof(fromAddr);

#ifdef CURSES
	mvwprintw(board,5,20,"Enter your name : ");
	wrefresh(board);
#else
        printf("\n Enter your name : ");
#endif

#ifdef CURSES
	mvwgetnstr(board,5,38,name,MAXSIZE);
#else
        scanf("%s",name);
#endif

        if((sendto(sockfd,name,strlen(name)+1,0,(struct sockaddr* )sendAddr, *sendAddr_len ))<0)
                { perror(" SENDTO() failed "); exit(4);}

#ifdef CURSES
	mvwprintw(board,9,20,"Waiting for the string... ");
	wrefresh(board);
#else
        printf("\n\t Waiting for the string...\n");
#endif
        if((recvfrom(sockfd,&string_len,4,0,(struct sockaddr *)&fromAddr, &addrLen ))<0)
	{ 
		perror(" RECVFROM() failed "); 
		exit(3);
	}
#ifdef CURSES
	mvwprintw(board,10,20,"The string to be guessed is of %d characters\n",string_len);
	wrefresh(board);
#else
        printf("\n\t The string to be guessed is of %d characters\n",string_len);
#endif

        
	show=(char *)malloc(string_len);
        for(i=0;i<string_len;i++)
                show[i]='-';
	show[i] = '\0' ;

#ifdef CURSES
	wattron(board,A_BOLD);
	mvwprintw(board,11,20,"%s",pick_array);
	wattroff(board,A_BOLD);
	mvwprintw(board,12,20,"%s",show);
        mvwprintw(board,12,42,"Lives Remaining - %d",update.lives);
//	mvwprintw(board,14,20,"Guess the characters one by one:");
	wrefresh(board);
#else
        printf("\n\t\t\t\t %s",show);
        printf("\t Lives Remaining - %d",update.lives);
        printf("\n");
        
	printf("\n\n\t\t Guess the characters one by one:\n");
#endif       
	 for(i=0;(update.win_flag!=1)&& update.lives>0;i++)
        {
		do{
			ch=getc(stdin);
		}while(ch=='\n');

                if((sendto(sockfd,&ch,1,0,(struct sockaddr *)sendAddr, *sendAddr_len ))<0)
                        { perror(" SENDTO() failed "); exit(4);}

                if((recvfrom(sockfd,&update,sizeof(struct info),0,(struct sockaddr *)&fromAddr, &addrLen ))<0)
                        { perror(" RECVFROM() failed "); exit(3);}

                if(1==update.invalid)
                {
#ifdef CURSES
			mvwprintw(board,row-3,20,"The Character is already used");
			wrefresh(board);
			sleep(1);
			wmove(board,row-3,20);
			wclrtoeol(board);
			wrefresh(board);
#else
                        printf("\n\t\t\t\t The character is already used\n");
                	printf("\n\t\t\t\t %s",show);
                	printf("\t Lives Remaining - %d",update.lives);
                	printf("\n");
#endif
	                continue;
                }

	/* Print the show[] string */
                for(j=0;j<update.count;j++)
                        show[update.pos[j]]=ch;
#ifdef CURSES
		pick_array[ch-97]=' ';
		wattron(board,A_BOLD);
		mvwprintw(board,11,20,"%s",pick_array);
		wattroff(board,A_BOLD);
		mvwprintw(board,12,20,"%s",show);
		mvwprintw(board,12,42,"Lives Remaining - %d",update.lives);
		wrefresh(board);
#else
                printf("\n\t\t\t\t %s",show);
                printf("\t Lives Remaining - %d",update.lives);
                printf("\n");
#endif
        }

        if(update.win_flag==1)
	{
#ifdef CURSES
			mvwprintw(board,row-8,20," CONGRATULATIONS : YOU WIN !!! ");
			wrefresh(board);
#else
                        printf("\n CONGRATULATIONS : YOU WIN !!! \n");
#endif
	}
        else
        {
		string=(char *)malloc(string_len+1);
		if((recvfrom(sockfd,string,string_len+1,0,(struct sockaddr *)&fromAddr, &addrLen ))<0)
                        { perror(" RECVFROM() failed "); exit(3);}
#ifdef CURSES
                	mvwprintw(board,14,20,"%s",string);
			mvwprintw(board,row-6,20," Better Luck Next Time... !!!");
			wrefresh(board);
#else
                    printf("\n\t\t\t\t\a %s",string);
                    printf("\n Better Luck Next Time... !!! \n");
#endif
		free(string);
        }

#ifdef CURSES
	mvwprintw(board,row-6,20," Do you want to play more??? (y\\n) ");
	wrefresh(board);
#else
        printf("\n Do you want to play more??? (y\\n) \n");
#endif
	do
	{
		more=wgetch(board);
	}while(0);
        

        if((sendto(sockfd,&more,1,0,(struct sockaddr *)sendAddr, *sendAddr_len ))<0)
        { 
		perror(" SENDTO() failed "); 
		exit(4);
	}

        return more;

}

