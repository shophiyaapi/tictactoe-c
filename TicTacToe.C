#include<stdio.h>
#include<stdlib.h>
#include<windows.h>//to refresh display and clear screen

//board
char board[3][3]={
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'}};

//drawing the board
void drawboard(){
    for(int i=0;i<3;i++)
    {
        printf(" %c  |  %c  |  %c\n",board[i][0],board[i][1],board[i][2]);
        if(i!=2)
        {
            printf("---------------\n");
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
    printf("Let's Play Tic Tac Toe!\n");
    drawboard();
    int row,column,move,status;
    int player=1;
    while(1)
    {
        player=(player%2)?1:2;
        printf("Player %d: ",player);
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
            printf("\n");
            drawboard();
            printf("player %d WINS!\n",player);
            printf("player %d is a LOSER!\n",3-player);
            break;
         }
         if(status==2)
         {
            printf("\n");
            drawboard();
            printf("It's a Draw!\n");
            break;
         }
         player++;
    }
    system("pause");

}