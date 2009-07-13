/****************************************************************************************************************
 *    						!!! Hangman !!!                                                 *
 * Players - Two	                                                                                        *
 * Protocol Used - UDP                                                                                          *
 * GCC Version - 4.1.1 20061011 (Red Hat 4.1.1-30)                                                              *
 * Created By :- Anuj Aggarwal (PRN No - 043)                                                                   *
 ***************************************************************************************************************/

#include"hangman.h"

extern WINDOW *board ;
extern int row,col ;


char play1_game(int sockfd,struct sockaddr_in *sendAddr,socklen_t *sendAddr_len)
{
        int i,j,string_len,matched=0;
	char pick_array[] = "abcdefghijklmnopqrstuvwxyz" ;
        char name[MAXSIZE],string[MAXSIZE],ch,*show,more;
        char used[MAXSIZE];					// array contains already used characters
        struct info update;
        struct sockaddr_in fromAddr;
        socklen_t addrLen;

        update.count=0;
        update.lives=6;
        update.invalid=0;
        update.win_flag=0;
        used[0]='\0';

        addrLen = sizeof(fromAddr);

#ifdef CURSES
	mvwprintw(board,5,20,"Waiting for a player...");
	wrefresh(board);
#else
        printf("\n Waiting for a player...\n");
#endif

        /* Get the name of player 2*/
        if((recvfrom(sockfd,name,sizeof(name),0,(struct sockaddr *)&fromAddr, &addrLen ))<0)
        { 
		perror(" RECVFROM() failed "); 
		exit(1);
	}
#ifdef CURSES
	mvwprintw(board,7,20,"%s : wants to play the game",name);
	wrefresh(board);
#else
        printf("%s : wants to play the game\n",name);
#endif

	DPRINT("\n The socket no of Player 2 : %d ", ntohs(fromAddr.sin_port));
	DPRINT("\n The Ip  of Player 2 : %s ", inet_ntoa(fromAddr.sin_addr));

        /* Assigning the sendAddr's Address and length to the corresponding variables*/
        *sendAddr =  fromAddr;
        *sendAddr_len = addrLen;

        /* input the string to be guessed and send its length to Player 2*/
#ifdef CURSES
	mvwprintw(board,8,20,"Enter the string for %s :",name);
	wrefresh(board);
	flushinp();
//	refresh();
#else
        printf("\n Enter the string for %s :\n",name);
#endif
	do
	{
		mvwgetnstr(board,9,20,string,MAXSIZE);
//        	scanf("%s",string);
	}while(0);

        string_len = strlen(string);
	if((sendto(sockfd,&string_len,4,0,(struct sockaddr *)sendAddr, *sendAddr_len ))<0)
        { 
		perror(" SENDTO() failed "); 
		exit(2);
	}

        /* initialize the show[] string*/
        show=(char *)malloc(string_len);
        for(i=0;i<string_len;i++)
	{
                show[i]='-';
	}
	show[i] = '\0' ;

#ifdef CURSES
	mvwprintw(board,10,20,"Now Wait and Watch...");
//	wmove(board,11,20);
	wattron(board,A_BOLD);
	mvwprintw(board,11,20,"%s",pick_array);
	wattroff(board,A_BOLD);
	wrefresh(board);
	mvwprintw(board,12,20,"%s",show);
	wrefresh(board);
	mvwprintw(board,12,42,"Lives Remaining - %d",update.lives);
	wrefresh(board);
#else
        printf("\n\n\t Now Wait and Watch... \n\n");
	printf("%s",pick_array);
        printf("\n\t\t\t %s",show);
        printf("\t Lives Remaining - %d",update.lives);
#endif

        for(i=0;(update.win_flag!=1) && update.lives>0;i++)
        {
                update.invalid=0;
                if((recvfrom(sockfd,&ch,1,0,(struct sockaddr *)&fromAddr, &addrLen ))<0)
                { 
			perror(" RECVFROM() failed "); 
			exit(3);
		}

             /* check for validity */  
                for(j=0;used[j]!='\0';j++)
                {
                        if(ch==used[j])
                        {
                                update.invalid=1;
                                break;
                        }
                }


                if(update.invalid==0)
                {
                        for(j=0;used[j]!='\0';j++);
                        used[j]=ch;
 			used[j+1]='\0';

                        matched+=update.count = check_char(ch,string,update.pos); 
                        if(update.count==0 && update.lives!=0)		       //  if character is not matched
			{ 
                                update.lives--;
			}
                        if(matched==string_len)		    // if all characters matched means player2 win
			{
                                update.win_flag=1;		
			}
                }

                /* update the show with matched characters */
                for(j=0;j<update.count;j++)
		{
                        show[update.pos[j]]=ch;
		}
#ifdef CURSES
		pick_array[ch-97]=' ';
		wattron(board,A_BOLD);
		mvwprintw(board,11,20,"%s",pick_array);
		wattroff(board,A_BOLD);
		mvwprintw(board,12,20,"%s",show);
		wrefresh(board);
		mvwprintw(board,12,42,"Lives Remaining - %d",update.lives);
		wrefresh(board);
#else
		pick_array[ch-97]=' ';
		printf("%s",pick_array);
        	printf("\n\t\t\t %s ",show);
        	printf("\t Lives Remaining - %d",update.lives);
#endif
                
		if((sendto(sockfd,&update,sizeof(struct info),0,(struct sockaddr *) sendAddr, *sendAddr_len ))<0)
                { 
			perror(" SENDTO() failed "); 
			exit(4);
		}

        }

        if(update.win_flag==0)
        {
                if((sendto(sockfd,string,sizeof(string),0,(struct sockaddr *) sendAddr, *sendAddr_len ))<0)
                { 
			perror(" SENDTO() failed "); 
			exit(4);
		}
#ifdef CURSES 
		mvwprintw(board,row-8,20,"CONGRATULATIONS : YOU WIN");
		wrefresh(board);
#else
        	printf("\n\n CONGRATULATIONS : YOU WIN\n");
#endif
        }

        else
	{
#ifdef CURSES
		mvwprintw(board,row-6,20,"Better Luck Next Time...");
		wrefresh(board);
#else
        	printf("\n Better Luck Next Time...\n");
#endif
	}

#ifdef CURSES
	mvwprintw(board,row-5,20,"Waiting for %s's response...",name);
	wrefresh(board);
#else
	printf("\n\n Waiting for %s's response...\n\n",name);
#endif

        if((recvfrom(sockfd,&more,1,0,(struct sockaddr *)&fromAddr , &addrLen ))<0)
        { 
		perror(" RECVFROM() failed "); 
		exit(3);
	}
	
        return more;
}

/* check_char() function checks the received character in the string and returns - 
	 i) no. of matched characters
	ii) positions in the string at which the character is matched
*/
int check_char(char ch,char string[],int pos[])
{
        int i,j,count=0;
        for(i=0,j=0;i<strlen(string);i++)
        {
                if(ch==string[i])
                {
                        pos[j++]=i;			// pos[] contains the positions of matched characters
                        count++;			// no. of characters matched
                }
        }

        return count;
}
