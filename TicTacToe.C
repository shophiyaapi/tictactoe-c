#include<stdio.h>
#include<stdlib.h>
#include<windows.h>//to refresh display and clear screen

#define COLOR_RED     "\x1b[31m"//replaces COLOR_RED with the escape code for red color
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define RESET         "\x1b[0m"
#define BOLD          "\x1b[1m"
#define PINK          "\x1b[35m"
#define blue          "\x1b[34m"
#define cyan          "\x1b[36m"
#define pink_slowblink "\x1b[5;35m"

//board
char board[3][3]={
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'}};

//drawing the board
void drawboard(){
    for(int i=0;i<3;i++)
    {
        printf(BOLD" %c  |  %c  |  %c\n" ,board[i][0],board[i][1],board[i][2]);
        if(i!=2)
        {
            printf("---------------\n" RESET);
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
int player1_wins = 0;
int player2_wins = 0;
int draws = 0;
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
void display_scoreboard() {
    printf("\n" BOLD "=== CURRENT SCOREBOARD ===\n" RESET);
    printf(cyan "Player 1 (X): %d wins\n" RESET, player1_wins);
    printf(blue "Player 2 (O): %d wins\n" RESET, player2_wins);
    printf(COLOR_YELLOW "Draws       : %d\n" RESET, draws);
    printf("==========================\n");
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

    char replay_choice;
do 
{
    init_board(); // Clears board array for a new match
    system("cls");
    display_scoreboard(); // Shows the live scores
    printf(pink_slowblink "Let's Play TicTacToe!\n" RESET);
    drawboard();
    int row,column,move,status;
    int player=1;
    while(1) //loop running infiinitely until a brak statement is encountered
    {
        player=(player%2)?1:2;
        printf("\nPlayer %d: ",player);
        scanf("%d",&move);
        //row and column chosen by the player
        if(move>=1 && move<=9)
        {
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
       }
        else
        {
            printf("Invalid move! Try Again.\n");
            player--;//so that the increment doesnt change the player
            system("pause");
            continue;
        }
        system("cls");//to refresh the board
        drawboard();
         status=checkwin();
         if(status==1)
         {
            system("cls");
            drawboard();
            printf(COLOR_GREEN "Player %d WINS!" RESET "\n",player);
            printf(COLOR_RED "Player %d is a LOSER!" RESET "\n",3-player);
            if (player == 1) player1_wins++;
             else player2_wins++;
            break;
         }
         if(status==2)
         {
            system("cls");
            drawboard();
            printf(COLOR_YELLOW "It's a Draw!" RESET "\n");
            draws++;
            break;
         }
         player++;
        } //end of inner while(1) loop
         printf("\nDo you want to play another round? (y/n): ");
    fflush(stdin); 
    scanf(" %c", &replay_choice);
   }

 while (replay_choice == 'y' || replay_choice == 'Y');
  {
    fprintf(file, "=== TIC-TAC-TOE MATCH SUMMARY ===\n");
    fprintf(file, "Player 1 (X) Wins: %d\n", player1_wins);
    fprintf(file, "Player 2 (O) Wins: %d\n", player2_wins);
    fprintf(file, "Total Draws       : %d\n", draws);
    fclose(file);
    printf(COLOR_GREEN "\nScores successfully saved to 'tictactoe_scores.txt'!\n" RESET);
    system("pause");
  }
  return 0;
}
