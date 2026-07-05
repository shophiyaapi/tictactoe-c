#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>//to refresh display and clear screen
#include<graphics.h>

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
#define namewidth 25 //to set the width of the names so the spacing is even


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

//drawing the board
void drawboard(){
    printf("\n");
    for(int i=0;i<3;i++)
    {
        printf(padding boldblue "   %c  |  %c  |  %c \n",board[i][0],board[i][1],board[i][2]);
        if(i!=2)
        {
            printf(padding "  ---------------\n" RESET);
        }

    }
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
    printf("\n" padding  boldblue "=========== LEADERBOARD ===========\n" RESET);
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
    int check=0;
    for(i=0;i<3;i++)//to check wether the board is filled or not
    {
        for(j=0;j<3;j++)
        {
            if(board[i][j]!='X'&&board[i][j]!='O')
            {
                check++;

            }
        }
    }
    if(check==0)
    {
        return 2;
    }
    return 0;
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
    printf(padding bluebackground  yellow  "|  %-*s (O): %d wins  |\n" RESET, namewidth, player2, p2win);
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

    char replaychoice;

    //game menu
    printf(padding  magenta "===" blink " Welcome to TicTacToe! " RESET magenta "===\n" RESET);
    printf(padding BOLD pinkbackground "1. Start Game                " RESET "\n");
    printf(padding BOLD pinkbackground "2. Check Leaderboard         " RESET "\n");
    printf(padding BOLD pinkbackground "3. Exit                      " RESET "\n");
    printf(padding  magenta "=============================\n" RESET);
    int choice;
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
                    scanf("%c", &replaychoice);
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
                    else
                    {
                        printf("\n" padding "Continuing with the same names: %s and %s\n", player1, player2);
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
                    player=(player%2)?1:2;
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
                       printf("\n" padding cyan "Player %s" RESET COLOR_GREEN  " WINS!" RESET, (player==1)?player1:player2);
                       printf("\n" padding yellow "Player %s" RESET " is a " RESET COLOR_RED   "LOSER!" RESET, (player==2)?player1:player2);
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
                if(replaychoice!='n' && replaychoice!='N')
                {
                    printf("\n" padding pink"Thank you for playing TicTacToe!\n");
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
            printf("\n" padding COLOR_GREEN "Scores saved to successfully!" RESET);
            system("pause");
            return 0;
            break;

        case 2:
            leaderboard();
            printf("\n" padding "Press any key to return to the main menu...");
            getchar(); //Wait for user input
            getchar(); //Consume the newline character left after last key stroke
            system("cls");
            main(); //Restart the main function to show the menu again
            return 0; //Exit the current instance of main after returning to the menu

        case 3:
            printf("\n" padding pink "Thank you for playing TicTacToe!\n");
            exit(0);

        default:
            printf(padding COLOR_RED "Invalid choice!\n" RESET);
            getchar();
            system("cls");
            main();
    }
}
