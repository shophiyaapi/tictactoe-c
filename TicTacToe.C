#include<stdio.h>
#include<stdlib.h>
#include<windows.h>//to refresh display and clear screen

#define COLOR_RED     "\x1b[31m"//replaces COLOR_RED with the escape code for red color
#define COLOR_GREEN   "\x1b[32m"
#define yellow        "\e[1;93m"
#define RESET         "\x1b[0m"
#define BOLD          "\x1b[1m"
#define boldblue      "\x1b[1;34m"
#define cyan          "\e[1;96m"
#define pink_slowblink "\x1b[5;95m"
#define white         "\e[1;97m"
#define bluebackground    "\x1b[44m"
#define magentaslowblink "\x1b[5;35m"
//board
char board[3][3]={
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'}};

//drawing the board
void drawboard(){
    printf("\n");
    for(int i=0;i<3;i++)
    {
        printf(boldblue "   %c  |  %c  |  %c \n",board[i][0],board[i][1],board[i][2]);
        if(i!=2)
        {
            printf("  ---------------\n" RESET);
        }

    }
} 
//for checking the current status of the game
//return 1 for win and return 2 for draw  and return 0 for ongoing game 
int checkwin()
{
    int i=0,j=0;
    //for rows
    for(i=0;i<3;i++)
    {
        if(board[i][0]==board[i][1]&&board[i][1]==board[i][2])
        {
            return 1;
        }
    }
    //for columns
    for(i=0;i<3;i++)
    {
        if(board[0][i]==board[1][i]&&board[1][i]==board[2][i])
        {
            return 1;
        }
    }
    //for diagonals
    if(board[0][0]==board[1][1]&&board[1][1]==board[2][2])
    {
        return 1;
    }
    if(board[0][2]==board[1][1]&&board[1][1]==board[2][0])
    {
        return 1;
    }
    //for draw
    int count=0;
    for(i=0;i<3;i++)//to check wether the board is filled or not
    {
        for(j=0;j<3;j++)
        {
            if(board[i][j]!='X'&&board[i][j]!='O')
            {
                count++;

            }
        }
    }
    if(count==0)
    {
        return 2;
    }
    return 0;
}
//score tracking variables
int p1win=0;
int p2win=0;
int draw=0;
 //board reset for new rounds 
    void init_board() 
    {
    char start_char = '1';
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            board[i][j] = start_char++;
        }
    }
   }
//live console scoreboard
void display_scoreboard()
{
    printf(bluebackground  "\n" pink_slowblink "=== CURRENT SCOREBOARD ===\n" RESET);
    printf(bluebackground   cyan   "|  Player 1 (X): %d wins  |\n" RESET, p1win);
    printf(bluebackground  yellow  "|  Player 2 (O): %d wins  |\n" RESET, p2win);
    printf(bluebackground  white  "|  Draws       : %d       |\n" RESET, draw);
    printf(bluebackground  pink_slowblink  "==========================\n" RESET);
}
int main()
{
    //file i/o score tracking 
    FILE *file;
    file=fopen("tictactoe_scores.txt", "w");
    if (file == NULL) 
    {
        printf(COLOR_RED "Error saving scores to file!\n" RESET);
        return 1;
    }

    char replaychoice;
    do //replay menu do while use garera 
    {
        init_board(); // Clears board array for a new match
        system("cls");
        display_scoreboard(); // Shows the live scores
        printf(magentaslowblink "\n Let's Play TicTacToe!\n" RESET);
        drawboard();
        int row,column,status;
        int move =0; 
        int player=1;

        while(1) //loop running infiinitely until a brak statement is encountered
        {
            player=(player%2)?1:2;
            if (player==1)
            {
                printf(RESET cyan "\n  Player 1(X): " RESET);
            }
            else
            {
                printf(RESET yellow "\n  Player 2(O): " RESET);
            }
            scanf("%d",&move);
         //row and column chosen by the player
            row=(move-1)/3;
            column=(move-1)%3;
         //checking wether the move is valid
            if((board[row][column]!='X'&&board[row][column]!='O')&&(move>=1&&move<=9))
            {
                if(player==1)
                {
                    board[row][column]='X';
                }
                else
                {
                    board[row][column]='O';
                }
            }
            else
            {
                printf(COLOR_RED "Invalid move! Try Again.\n" RESET);
                player--;//so that the increment doesnt change the player
                system("pause");
            }
            system("cls");//to refresh the board
            drawboard();
            status=checkwin();
            if(status==1)
            {
                system("cls");
                drawboard();
                printf(cyan "Player %d" RESET COLOR_GREEN  "WINS!" RESET "\n",player);
                printf(yellow "Player %d" RESET "is a" RESET COLOR_RED   "LOSER!" RESET "\n",3-player);
                if (player == 1) 
                {
                    p1win++;
                }
                else
                {
                    p2win++;
            
                }            
                break;
          
            }         
            if(status==2)
            {
                system("cls");
                drawboard();
                printf(white "It's a Draw!" RESET "\n");
                draw++;
                break;         
            }
            player++;
        } //end of inner while(1) loop
        display_scoreboard();
        printf("\n Do you want to play another round? (y/n): ");
        fflush(stdin); 
        scanf(" %c", &replaychoice);
    }
    while (replaychoice == 'y' || replaychoice == 'Y');//if the condition is true it starts from the start of the do loop if false it exits
    //do while loop false vayera  exit vayepaxi chai yo code vetera run hunxa
    fprintf(file, "=== TIC-TAC-TOE MATCH SUMMARY ===\n");
    fprintf(file, "Player 1 (X) Wins: %d\n", p1win);
    fprintf(file, "Player 2 (O) Wins: %d\n", p2win);
    fprintf(file, "Total Draws       : %d\n", draw);
    fclose(file);
    printf(COLOR_GREEN "\n Scores saved to successfully!" RESET);
    system("pause");
    return 0;
}
