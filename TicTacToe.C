#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>//to refresh display and clear screen
#include<stdbool.h>//for boolean values
#include<time.h>//for random number generation

//for customization
#define COLOR_RED     "\x1b[31m"//replaces COLOR_RED with the escape code for red color
#define COLOR_GREEN   "\x1b[32m"
#define yellow        "\e[1;93m"
#define RESET         "\x1b[0m"
#define BOLD          "\x1b[1m"
#define boldblue      "\x1b[1;34m"
#define cyan          "\e[1;96m"
#define blink         "\x1b[5m"
#define pink          "\e[1;95m"
#define pink_slowblink "\x1b[5;95m"
#define white         "\e[1;97m"
#define brightblue     "\e[1;94m"
#define bluebackground    "\x1b[44m"
#define pinkbackground    "\x1b[45m"
#define magenta       "\e[1;35m"
#define magentaslowblink "\x1b[5;35m"
#define padding "                                                            "
#define namewidth 15 //to set the width of the names so the spacing is even


//to store the choice of the user for the game mode
 int choice=0;

//score tracking variables
int p1win=0;
int p2win=0;
int draw=0;
int gamerun=0;
//player name variables
char player1[20];
char player2[20];

struct PlayerScores
{
    char name[20];
    int wins;
}user[100];
struct PlayerScores temp;

//count variable to keep track of the number of players in the leaderboard
int count=0;

//board
char board[3][3]={
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'}
};
//color coding for pieces
void print_piece(char piece)
{
   if (piece =='X')
   {
      printf(COLOR_RED BOLD " %c" RESET, piece);
   }
   else if (piece=='O')
   {
     printf(COLOR_GREEN BOLD " %c" RESET,piece);
   }
   else
   {
    printf(brightblue " %c" RESET,piece);
   }
}
//drawing the board
void drawboard(){
    printf("\n");
    for(int i=0;i<3;i++)
    {
    	printf("%s", padding);
        print_piece(board[i][0]); 
		printf(boldblue " | " RESET);
        print_piece(board[i][1]); 
		printf(boldblue " | " RESET);
        print_piece(board[i][2]);
        printf("\n");
        if(i!=2)
        {
            printf("%s" boldblue "-------------\n" RESET,padding);
        }
        printf("\n");
    }
} 

//FOR VS COMPUTER

struct storeMove//to store the row and column of the best move
{
    int row;
    int column;
};

char computer='O', human='X';

bool isMovesLeft(char board[3][3])//to check wether there are any moves left or not
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(board[i][j]!='X'&&board[i][j]!='O')
            {
                return true;
            }
        }
    }
    return false;
}

// leaderboard 
void leaderboard()
{
 // Sort the scores in descending order
    for (int i=0; i<count-1; i++)
    {
        for (int j=0; j<count-i-1; j++)
        {
            if (user[j].wins<user[j+1].wins)
            {
                temp=user[j];//temp structure
                user[j]=user[j+1];
                user[j+1]=temp;
            }
        }
    }
    //leaderboard display
    printf("\n" padding  boldblue "======= LEADERBOARD =======\n" RESET);
    for (int i=0; i<count; i++)
    {
        if(i==0)
        {
            printf(padding BOLD yellow blink "%d. %-*s %d wins\n" RESET ,i + 1, namewidth, user[i].name, user[i].wins);
        }
        else
        {
            printf(padding "%d. %-*s %d wins\n",i + 1, namewidth, user[i].name, user[i].wins);
        }
    }
}

//for checking the current status of the game
//return 1 for win and return 2 for draw  and return 0 for ongoing game 
//for vs computer return +10 for computer win and -10 for human win and 0 for draw
int checkwin(char board[3][3])
{
    int i=0,j=0;
    //for rows
    for(i=0;i<3;i++)
    {
        if(board[i][0]==board[i][1]&&board[i][1]==board[i][2])
        {
            if(choice==1)
            {
                return 1;
            }
            else if(choice==2)
            {
                if (board[i][0]==computer)
                return +10;
                else if (board[i][0]==human)
                return -10;
            }
        }
    }
    //for columns
    for(i=0;i<3;i++)
    {
        if(board[0][i]==board[1][i]&&board[1][i]==board[2][i])
        {
            if(choice==1)
            {
                return 1;
            }
            else if(choice==2)
            {
                if (board[0][i]==computer)
                return +10;
                else if (board[0][i]==human)
                return -10;
            }
        }
    }
    //for diagonals
    if(board[0][0]==board[1][1]&&board[1][1]==board[2][2])
    {
        if(choice==1)
        {
            return 1;
        }
        else if(choice==2)
        {
            if (board[0][0]==computer)
            return +10;
            else if (board[0][0]==human)
            return -10;
        }
    }
    if(board[0][2]==board[1][1]&&board[1][1]==board[2][0])
    {
        if(choice==1)
        {
            return 1;
        }
        else if(choice==2)
        {
            if (board[0][2]==computer)
            return +10;
            else if (board[0][2]==human)
            return -10;
        }
    }
    //for draw
    
    if(isMovesLeft(board)==false)
    {
        if(choice==1)
        {
            return 2;//return 2 for draw in player vs player mode
        }
        else if(choice==2)
        {
            return 0;
        }
    }
    return 0;
}

//minimax algorithm for vs computer
int minimax(char board[3][3], int depth, bool isMax)//bool isMax is used to catch the boolean value sent from the findBestMove function to check wether it is maximizer's move or minimizer's move
{
    int score=checkwin(board);
    if(score==10)//if maximizer has won the game return score and minus the depth to choose the fastest win
    {
        return score-depth;
    }
    if(score==-10)//if minimizer has won the game return score and add the depth to choose the slowest loss
    {
        return score+depth;
    }
    if(isMovesLeft(board)==false)//if there are no moves left and no one has won then it is a draw
    {
        return 0;
    }
    if(isMax)//if it is maximizer's move i.e. if ismax=false
    {
        int best=-1000;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if(board[i][j]!='X'&&board[i][j]!='O')//if the cell is empty then only we can make a move 
                {
                    char backup=board[i][j];//backup the cell number(1,2,3...) before making a move so that we can restore it later
                    board[i][j]=computer;//to  set the cell to O
                    int currentValue=minimax(board,depth+1,!isMax);
                    if(currentValue>best)
                    {
                        best=currentValue;
                    }
                    board[i][j]=backup;
                }
            }
        }
        return best;//return the best value for maximizer
    }
    else//if it is minimizer's move
    {
        int best=1000;
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                if(board[i][j]!='X'&&board[i][j]!='O')
                {
                    char backup=board[i][j];
                    board[i][j]=human;//to  set the cell to the X
                    //call minimax recursively and choose the minimum value
                    int currentValue=minimax(board,depth+1,!isMax);
                    //if ismax starts as true then ! changes it to false and vice versa so that the next move is made by the other player
                    if(currentValue<best)
                    {
                        best=currentValue;
                    }
                    board[i][j]=backup;
                }
            }
        }
        return best;//return the best value for minimizer
    }
}

//to find best move for the computer
struct storeMove findBestMove(char board[3][3])//making a structure to store the row and column of the best move
{
    int bestVal=-1000;//initialize the best value to a very low value
    struct storeMove bestMove;
    bestMove.row=-1;//because the row and column are 0 indexed so -1 is an invalid value so that we can check if the best move is found or not
    bestMove.column=-1;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(board[i][j]!='X'&&board[i][j]!='O')
            {
                char backup=board[i][j];//backup the cell number(1,2,3...) before making a move so that we can restore it later
                board[i][j]=computer;
                int moveVal=minimax(board,0,false);//call minimax recursively and choose the maximum value
                board[i][j]=backup;
                if(moveVal>bestVal)//if the value of the move is greater than the best value then update the best value and best move
                {
                    bestMove.row=i;
                    bestMove.column=j;
                    bestVal=moveVal;//update the best value
                }
            }
        }
    }
    return bestMove;//return the best move
}


//board reset for new rounds 
void init_board() 
{
    char refresh='1';
    for (int i=0;i<3;i++) 
    {
        for (int j=0;j<3;j++) 
        {
            board[i][j]=refresh++;
        }
    }
}

//live console scoreboard
void display_scoreboard()
{
    printf("\n" padding bluebackground   pink_slowblink "====== CURRENT SCOREBOARD =======\n" RESET);
    printf(padding bluebackground   cyan   "|  %-*s (X): %d wins  |\n" RESET, namewidth, player1, p1win);
    printf(padding bluebackground  yellow  "|  %-*s (O): %d wins  |\n" RESET, namewidth,((choice==2)?"Computer":player2), p2win);
    printf(padding bluebackground  white  "|  %-*s (=): %d draws |\n" RESET, namewidth, "Draws", draw);
    printf(padding bluebackground  pink_slowblink  "=================================\n" RESET);
}


int main()
{
    //file for reading names and scores
    FILE *readfile;
    readfile=fopen("tictactoe_scores.txt","r");
    if (readfile==NULL) 
    {
        printf( padding COLOR_RED "Error opening scores file!\n" RESET);
        return 1;
    }
    else
    {
        //reading the scores from the file
        while (fscanf(readfile, "%s wins: %d", user[count].name, &user[count].wins) == 2)
        {
            count++;
        }
    }
    fclose(readfile);

    //file i/o score tracking 
    FILE *file;
    file=fopen("tictactoe_scores.txt","a");
    if (file == NULL) 
    {
        printf(padding COLOR_RED "Error saving scores to file!\n" RESET);
        return 1;
    }

    char replaychoice = '\0';

    //game menu
    printf(padding  magenta "===" blink " Welcome to TicTacToe! " RESET magenta "===\n" RESET);
    printf(padding BOLD pinkbackground "1. PLAYER VS PLAYER          " RESET "\n");
    printf(padding BOLD pinkbackground "2. PLAYER VS COMPUTER        " RESET "\n");
    printf(padding BOLD pinkbackground "3. Check Leaderboard         " RESET "\n");
    printf(padding BOLD pinkbackground "4. Exit                      " RESET "\n");
    printf(padding  magenta "=============================\n" RESET);
    printf("\n" padding yellow "Enter your choice: " RESET);
    scanf("%d", &choice);

    //switch case for menu options  
    switch (choice) 
    {
        case 1:
            do //replay menu do while use garera 
            {
            //player name input
                if(gamerun==0)
                {
                    p1win = 0;
                    p2win = 0;
                    draw = 0;
                    printf("\n" padding cyan "Player 1 (x) Enter Your Name: ");
                    scanf("%s", player1);
                    for(int i=0;i<count;i++)
                    {
                        int cmp=strcmp(player1,user[i].name);
                        if(cmp==0)
                        {
                            printf(padding cyan "Name not available, please choose a different name.\n");
                            printf("\n" padding cyan "Player 1 (x) Enter Your Name: ");
                            scanf("%s", player1);
                            i=-1;
                        }
                    }
                    printf("\n" padding yellow "Player 2 (o) Enter Your Name: ");
                    scanf("%s", player2);
                    for(int i=0;i<count;i++)
                    {
                        int cmp=strcmp(player2,user[i].name);
                        int cmp1=strcmp(player1,player2);
                        if((cmp==0)||(cmp1==0))
                        {
                            printf(padding yellow "Name not available, please choose a different name.\n");
                            printf("\n" padding yellow "Player 2 (o) Enter Your Name: ");
                            scanf("%s", player2);
                            i=-1;
                        }
                    }
                }
                else
                {
                    printf("\n" padding brightblue "Do you want to use the same names(Y/N)?: ");
                    fflush(stdin);
                    scanf(" %c", &replaychoice);
                    if (replaychoice=='n'|| replaychoice=='N') 
                    {
                        p1win = 0;
                        p2win = 0;
                        draw = 0;
                        printf("\n" padding cyan "Player 1 (x) Enter Your Name: ");
                        scanf("%s", player1);
                        for(int i=0;i<count;i++)
                        {
                           int cmp=strcmp(player1,user[i].name);
                           if(cmp==0)
                            {
                                printf(padding "Name not available, please choose a different name.\n");
                                printf("\n" padding cyan "Player 1 (x) Enter Your Name: ");
                                scanf("%s", player1);
                                i=-1;
                            }
                        }
                        printf("\n" padding yellow "Player 2 (o) Enter Your Name: ");
                        scanf("%s", player2);
                        for(int i=0;i<count;i++)
                        {
                            int cmp=strcmp(player2,user[i].name);
                            int cmp1=strcmp(player1,player2);
                            if((cmp==0)||(cmp1==0))
                            {
                            printf(padding "Name not available, please choose a different name.\n");
                            printf("\n" padding yellow "Player 2 (o) Enter Your Name: ");
                            scanf("%s", player2);
                            i=-1;
                            }
                        }
                    }
                    else if(replaychoice=='y'|| replaychoice=='Y')
                    {
                        printf("\n" padding "Continuing with the same names: %s and %s\n", player1, player2);
                    }
                    else
                    {
                        printf("\n" padding COLOR_RED "Invalid choice! Continuing with the same name" RESET);
                        system("pause");
                    }
                }

                init_board(); // Clears board array for a new match
                system("cls");
                display_scoreboard(); // Shows the live scores
                printf("\n" padding BOLD magentaslowblink " Let's Play TicTacToe!\n" RESET);
                drawboard();
                int row,column,status;
                int move =0; 
                int player=1;
         
              //main game loop
                while(1) //loop running infiinitely until a brake statement is encountered
                {
                    player=(player%2)?1:2;//if remainder is 1(i.e. true) then player 1's turn else player 2's turn
                    if (player==1)
                    {
                        printf("\n" padding RESET cyan "  %s(X): " RESET , player1);
                    }
                    else
                    {
                        printf("\n" padding RESET yellow "  %s(O): " RESET , player2);
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
                        printf("\n" padding COLOR_RED "Invalid move! Try Again.\n" RESET);
                        fflush(stdin);
                        continue;
                    }
                    system("cls");//to refresh the board
                    drawboard();
                    status=checkwin(board);
                    if(status==1)
                    {
                       system("cls");
                       drawboard();
                       printf("\n" padding cyan "Player %s" RESET COLOR_GREEN  " WINS!" RESET, (player==1)?player1:player2);
                       printf("\n" padding yellow "Player %s" RESET " is a " RESET COLOR_RED   "LOSER!\n" RESET, (player==2)?player1:player2);
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
                        printf("\n" padding white "It's a Draw!" RESET );
                        draw++;
                        break;         
                    }
                    player++;
                } //end of inner while(1) loop

                gamerun++;
                display_scoreboard();
                printf("\n" padding brightblue "Do you want to play another round? (y/n): ");
                fflush(stdin); 
                scanf(" %c", &replaychoice);
                if(replaychoice!='y' && replaychoice!='Y' && replaychoice!='n' && replaychoice!='N')
                {
                    printf("\n" padding COLOR_RED "INVALID CHOICE!\n" RESET);
                    getchar();
                    getchar();
                    system("cls");
                    main();
                }
            }
            while (replaychoice == 'y' || replaychoice == 'Y');//if the condition is true it starts from the start of the do loop if false it exits
            //do while loop false vayera  exit vayepaxi chai yo code vetera run hunxa
            fprintf(file, "%s wins: %d\n", player1, p1win);
            fprintf(file, "%s wins: %d\n", player2, p2win);
            fclose(file);
            system("pause");
            printf("\n" padding pink"Thank you for playing TicTacToe!\n");
            getchar();
            system("cls");
            main();


        case 2:
            do //replay menu do while use garera 
            {
                if(gamerun==0)
                {
                    int computerwin=0;
                    int humanwin=0;
                    printf("\n" padding cyan "Player (x) Enter Your Name: ");
                    scanf("%s", player1);
                    for(int i=0;i<count;i++)
                    {
                        int cmp=strcmp(player1,user[i].name);
                        if(cmp==0 && strcmp(player1,"Computer")!=0 && strcmp(player1,"COMPUTER")!=0)
                        {
                            printf(padding cyan "Name not available, please choose a different name.\n");
                            printf("\n" padding cyan "Player (x) Enter Your Name: ");
                            scanf("%s", player1);
                            i=-1;
                        }

                    }
                    
                }
                else
                {
                    printf("\n" padding brightblue "Do you want to use the same name(Y/N)?: ");
                    fflush(stdin);
                    scanf(" %c", &replaychoice);
                    if (replaychoice=='n'|| replaychoice=='N') 
                    {
                        p1win=0;
                        p2win=0;
                        draw=0;
                        printf("\n" padding cyan "Player (x) Enter Your Name: ");
                        scanf("%s", player1);
                        for(int i=0;i<count;i++)
                        {
                           int cmp=strcmp(player1,user[i].name);
                           if(cmp==0 && strcmp(player1,"Computer")!=0 && strcmp(player1,"COMPUTER")!=0)
                            {
                                printf(padding "Name not available, please choose a different name.\n");
                                printf("\n" padding cyan "Player (x) Enter Your Name: ");
                                scanf("%s", player1);
                                i=-1;
                            }
                        }
                    }
                    else if(replaychoice=='y'|| replaychoice=='Y')
                    {
                        printf("\n" padding "Continuing with the same name: %s\n", player1);
                    }
                    else
                    {
                        printf("\n" padding COLOR_RED "Invalid choice! Continuing with the same name" RESET);
                        system("pause");
                    }
                }

                init_board(); // Clears board array for a new match
                system("cls");
                display_scoreboard(); // Shows the live scores
                printf("\n" padding BOLD magentaslowblink " Let's Play TicTacToe!\n" RESET);
                drawboard();
                int row,column,status;
                int move =0; 
                int player=1;//player 1 is human and player 2 is computer
                while(1)
                {
                    player=(player%2)?1:2;
                    if (player==1)
                    {
                        printf("\n" padding RESET cyan "  %s(X): " RESET , player1);
                        scanf("%d",&move);
                        row=(move-1)/3;
                        column=(move-1)%3;
                        if((board[row][column]!='X'&&board[row][column]!='O')&&(move>=1&&move<=9))
                        {
                            board[row][column]=human;
                        }
                        else
                        {
                            printf("\n" padding COLOR_RED "Invalid move! Try Again.\n" RESET);
                            fflush(stdin);
                            player--;
                        }
                    }
                    else
                    {
                        struct storeMove bestMove=findBestMove(board);
                        row=bestMove.row;
                        column=bestMove.column;
                        board[row][column]=computer;
                    }
                    system("cls");//to refresh the board
                    drawboard();
                    status=checkwin(board);
                    if(status==10)
                    {
                       system("cls");
                       drawboard();
                       printf("\n" padding yellow "Computer" RESET COLOR_GREEN  " WINS!" RESET);
                       printf("\n" padding cyan "%s" RESET " is a " RESET COLOR_RED   "LOSER!\n" RESET, player1);
                       p2win++;
                       break;
                    }         
                    if(status==-10)
                    {
                       system("cls");
                       drawboard();
                       printf("\n" padding cyan "%s" RESET COLOR_GREEN  " WINS!" RESET, player1);
                       printf("\n" padding yellow "Computer" RESET " is a " RESET COLOR_RED   "LOSER!\n" RESET);
                       p1win++;
                       break;
                    }         
                    if(status==0 && isMovesLeft(board)==false)
                    {
                        system("cls");
                        drawboard();
                        printf("\n" padding white "It's a Draw!" RESET );
                        draw++;
                        break;         
                    }
                    player++;
                } //end of inner while(1) loop
                gamerun++;

                display_scoreboard();
                printf("\n" padding brightblue "Do you want to play another round? (y/n): ");
                fflush(stdin); 
                scanf(" %c", &replaychoice);
                if(replaychoice!='y' && replaychoice!='Y' && replaychoice!='n' && replaychoice!='N')
                { 
                    printf("\n" padding COLOR_RED "INVALID CHOICE! GOING TO MAIN MENU\n" RESET);
                    system("pause");
                    getchar();
                    system("cls");
                    main();
                }
            }
            while(replaychoice == 'y' || replaychoice == 'Y');//if the condition is true it starts from the start of the do loop if false it exits
            //do while loop false vayera  exit vayepaxi chai yo code vetera run hunxa
            fprintf(file, "%s wins: %d\n", player1, p1win);
            fclose(file);
            system("pause");
            printf("\n" padding pink"Thank you for playing TicTacToe!\n");
            getchar();
            system("cls");
            main();
            
            
        case 3:
            leaderboard();
            printf("\n" padding "Press any key to return to the main menu...");
            getchar(); //Wait for user input
            getchar(); //Consume the newline character left after last key stroke
            system("cls");
            main ();
            return 0;//to exit current main
        

        case 4:
            printf("\n" padding pink "Thank you for playing TicTacToe!\n");
            system("pause");
            exit(0);

        default:
            printf(padding COLOR_RED "Invalid choice!\n" RESET);
            getchar();
            getchar();//consume newline character
            system("cls");
            main();
    }
}
