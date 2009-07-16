
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
	struct query_hint queryhint;
        socklen_t addrLen;
        struct info update;
	
	/* Initialization of update structure */
        update.count=0;
        update.lives=6;
        update.invalid=0;
        update.win_flag=0;

        addrLen=sizeof(fromAddr);

#ifdef CURSES
	mvwprintw(board,5,X_COL,"Enter your name : ");
	wrefresh(board);
#else
        printf("\n Enter your name : ");
#endif

#ifdef CURSES
	mvwgetnstr(board,5,X_COL+18,name,MAXSIZE);
#else
        scanf("%s",name);
#endif

        if((sendto(sockfd,name,strlen(name)+1,0,(struct sockaddr* )sendAddr, *sendAddr_len ))<0)
                { perror(" SENDTO() failed "); exit(4);}

#ifdef CURSES
	mvwprintw(board,7,X_COL,"Waiting for the string... ");
	wrefresh(board);
#else
        printf("\n\t Waiting for the string...\n");
#endif
        if((recvfrom(sockfd,&queryhint,sizeof(queryhint),0,(struct sockaddr *)&fromAddr, &addrLen ))<0)
	{ 
		perror(" RECVFROM() failed "); 
		exit(3);
	}
#ifdef CURSES
	mvwprintw(board,9,X_COL,"The string to be guessed is of %d characters\n",queryhint.string_len);
	wrefresh(board);
	mvwprintw(board,11,X_COL,"hint : %s",queryhint.hint_str);
	wrefresh(board);
#else
        printf("\n\t The string to be guessed is of %d characters\n",queryhint.string_len);
#endif

        
	show=(char *)malloc(queryhint.string_len);
        for(i=0;i<queryhint.string_len;i++)
                show[i]='-';
	show[i] = '\0' ;

#ifdef CURSES
	wattron(board,A_BOLD);
	mvwprintw(board,13,X_COL,"%s",pick_array);
	wattroff(board,A_BOLD);
	mvwprintw(board,15,X_COL,"%s",show);
        mvwprintw(board,15,X_COL+22,"Lives Remaining - %d",update.lives);
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
			mvwprintw(board,row-3,X_COL,"The Character is already used");
			wrefresh(board);
			sleep(1);
			wmove(board,row-3,X_COL);
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
		mvwprintw(board,13,X_COL,"%s",pick_array);
		wattroff(board,A_BOLD);
		mvwprintw(board,15,X_COL,"%s",show);
		mvwprintw(board,15,X_COL+22,"Lives Remaining - %d",update.lives);
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
			mvwprintw(board,row-8,X_COL," CONGRATULATIONS : YOU WIN !!! ");
			wrefresh(board);
#else
                        printf("\n CONGRATULATIONS : YOU WIN !!! \n");
#endif
	}
        else
        {
		string=(char *)malloc(queryhint.string_len+1);
		if((recvfrom(sockfd,string,queryhint.string_len+1,0,(struct sockaddr *)&fromAddr, &addrLen ))<0)
                        { perror(" RECVFROM() failed "); exit(3);}
#ifdef CURSES
                	mvwprintw(board,14,X_COL,"%s",string);
			mvwprintw(board,row-6,X_COL," Better Luck Next Time... !!!");
			wrefresh(board);
#else
                    printf("\n\t\t\t\t\a %s",string);
                    printf("\n Better Luck Next Time... !!! \n");
#endif
		free(string);
        }

#ifdef CURSES
	mvwprintw(board,row-6,X_COL," Do you want to play more??? (y\\n) ");
	wrefresh(board);
#else
        printf("\n Do you want to play more??? (y\\n) \n");
#endif
	do
	{
		more=wgetch(board);
	}while(more == '\n');
        

        if((sendto(sockfd,&more,1,0,(struct sockaddr *)sendAddr, *sendAddr_len ))<0)
        { 
		perror(" SENDTO() failed "); 
		exit(4);
	}

        return more;

}

