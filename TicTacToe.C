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
int main()
{
    printf(pink_slowblink "Let's Play TicTacToe!\n" RESET);
    drawboard();
    int row,column,move,status;
    int player=1;
    while(1)
    {
        player=(player%2)?1:2;
        printf("\nPlayer %d: ",player);
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
            printf("Invalid move! Try Again.\n");
            player--;//so that the increment doesnt change the player
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
            break;
         }
         if(status==2)
         {
            system("cls");
            drawboard();
            printf(COLOR_YELLOW "It's a Draw!" RESET "\n");
            break;
         }
         player++;
    }
    system("pause");

}