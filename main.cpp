//
//  TicTacToe by Logan Bateman 2017
//
//  using SDL2 library for graphics
//  It is unbeatable I believe.
//

#include <SDL2/SDL.h>
#include <stdio.h>
class TTTBoard
{
    private:
        uint8_t tttmap[3][3];
        uint8_t curTurn;
        uint8_t plays;
        void DrawBoard(SDL_Renderer *renderer)
        {
            int i, ii;
            for (i=0; i<2; i++)
            {
                for (ii=1; ii<3; ii++ )
                {

                    SDL_RenderDrawLine(renderer,(1-i)*ii*100, 0+(ii*100*i), (1-i)*ii*100+i*300, 300*(1-i)+(ii*100*i));
                }
            }
        }
        void DrawX(int x, int y, SDL_Renderer *renderer)
        {
            int centerX, centerY;
            centerX=x*100-50;
            centerY=y*100-50;
            SDL_RenderDrawLine(renderer,centerX-25,centerY-25,centerX+25, centerY+25);
            SDL_RenderDrawLine(renderer,centerX+25,centerY-25,centerX-25, centerY+25);
        }
        void DrawO(int x, int y, SDL_Renderer *renderer)
        {
            int centerX, centerY;
            centerX=x*100-50;
            centerY=y*100-50;
            const float pi=3.14159f;
            SDL_Point points[17];
            int i;
            for (i=0; i<17;i++ )
            {
                points[i].y=SDL_sinf(pi/8.f*(float)i)*25.0f+centerY;
                points[i].x=SDL_cosf(pi/8.f*(float)i)*25.0f+centerX;
            }
            SDL_RenderDrawLines(renderer, points,17);
        }
    public:
        TTTBoard()
        {
            for (int ii=0; ii<3;ii++)
            {
                for (int iii=0; iii<3;iii++)
                {
                    tttmap[ii][iii]=0;
                }
            }

            plays=0;
            curTurn=1;
        }
        uint8_t whosTurn()
        {
            return curTurn;
        }
        void setTurn(int turn)
        {
            curTurn=turn;
        }
        void test_function()
        {
            TTTBoard testboard1;
            printf("NextPlayCom: %d\n", testboard1.nextComputerPlay(1));
            SDL_Delay(2000);
            //(char*)&tttmap="Hello";
        }
        void Draw(SDL_Renderer *renderer)
        {
            DrawBoard(renderer);
            for (int ii=0; ii<3;ii++)
            {
                for (int iii=0; iii<3;iii++)
                {
                    if(tttmap[ii][iii]==1){DrawX(ii+1, iii+1, renderer);}
                    if(tttmap[ii][iii]==2){DrawO(ii+1, iii+1, renderer);}
                }
            }

        }
        void CopyTo(TTTBoard *board)
        {
            board->curTurn=this->curTurn;
            board->plays=this->plays;
            for (int ii=0; ii<3;ii++)
            {
                for (int iii=0; iii<3;iii++)
                {
                    board->tttmap[ii][iii]=this->tttmap[ii][iii];
                }
            }
            //board->tttmap=this->tttmap;
        }
        int play(uint8_t player, uint8_t position)
        {
            uint8_t x,y;
            x=(position-1)%3;
            y=(position-1)/3;
            if (tttmap[x][y]==0)
            {
                tttmap[x][y]=player;
                plays++;
                if (player==1){curTurn=2;}else{curTurn=1;}
                return 0;
            }else {return -1;}
        }
        // Returns 0 if plays can still be made
        //         1 if player 1 (x) won
        //         2 if player 2 (o) won
        //         3 if there is a tie
        int checkWin()
        {
            int i,ii,iii;
            for (i=1; i<3; i++)
            {
                if (tttmap[1][1]==i) //center
                {
                    if ((tttmap[1][0]==i)&(tttmap[1][2]==i))return i; //top to bottom Middle
                    if ((tttmap[0][1]==i)&(tttmap[2][1]==i))return i; //left middle to right Middle
                    if ((tttmap[0][0]==i)&(tttmap[2][2]==i))return i; //top left to bottom right
                    if ((tttmap[0][2]==i)&(tttmap[2][0]==i))return i; //bottom left to top right
                }
                if ((tttmap[0][0]==i)&(tttmap[1][0]==i)&(tttmap[2][0]==i))return i; //top 3
                if ((tttmap[0][0]==i)&(tttmap[0][1]==i)&(tttmap[0][2]==i))return i; //left 3
                if ((tttmap[0][2]==i)&(tttmap[1][2]==i)&(tttmap[2][2]==i))return i; //bottom 3
                if ((tttmap[2][0]==i)&(tttmap[2][1]==i)&(tttmap[2][2]==i))return i; //right 3
            }
            for (ii=0; ii<3;ii++)
            {
                for (iii=0; iii<3;iii++)
                {
                    if (tttmap[ii][iii]==0)return 0;
                }
            }
            return 3;
        }
        // This function returns the calculated best play for {player} based on the current board state
        uint8_t nextComputerPlay(uint8_t player) //assuming computer is {player}
        {
            TTTBoard testBoard;         //a new board for simulating/trying possibilities
            int i;
            bool notplayable[9]={false};
            uint8_t opponentplayer;
            uint32_t value=10000000000;  //ideally at each recursion in count_possibilities() the value decreases since the next move is more important than the last
            uint32_t wins_a[9]={0};      //holds a possible wins value for each move
            uint32_t losses_a[9]={0};    //holds a possible losses value for each move
            uint32_t ties_a[9]={0};      //holds a possible ties value for each move
            if (player==1){opponentplayer=2;}else{opponentplayer=1;}
            if (checkWin()!=0)return 0; //no more plays
            for (i=1; i<10; i++)    //do a play and check for each of the 9 spaces
            {
                this->CopyTo(&testBoard);
                if (testBoard.play(player,i)==0) //checks to see if the position was open
                {
                    if (testBoard.checkWin()==opponentplayer){losses_a[i-1]=value;memset(wins_a, 0, sizeof(wins_a));memset(ties_a, 0, sizeof(ties_a));i=10;}
                    if (testBoard.checkWin()==player){wins_a[i-1]=value;memset(losses_a, 0, sizeof(losses_a));memset(ties_a, 0, sizeof(ties_a));i=10;}
                    if (testBoard.checkWin()==3){ties_a[i-1]=value;}//not a winner
                    if (testBoard.checkWin()==0){testBoard.count_possibilities(player,&wins_a[i-1],&losses_a[i-1], &ties_a[i-1], value);}
                }
                else{notplayable[i-1]=true;}
                printf("Wins  %d:%d\n", i, wins_a[i-1]); //debugging info
                printf("Losses%d:%d\n", i, losses_a[i-1]);
                printf("Ties  %d:%d\n", i, ties_a[i-1]);
            }
            int iplayable=0;
            while (notplayable[iplayable]==true){iplayable++;}
            uint32_t low_loss[2]={losses_a[iplayable],iplayable}, high_win[2]={wins_a[iplayable],iplayable}, high_tie[2]={ties_a[iplayable],iplayable};
            for (i=1; i<9; i++)
            {
                if (notplayable[i]==false)
                {
                    if (low_loss[0]>losses_a[i]){low_loss[0]=losses_a[i], low_loss[1]=i;}
                    if (high_win[0]<wins_a[i]){high_win[0]=wins_a[i], high_win[1]=i;}
                    if (high_tie[0]<ties_a[i]){high_tie[0]=ties_a[i], high_tie[1]=i;}
                }
            }
            printf("Play# %d, Player:%d\n", this->plays+1, this->curTurn); //debugging info
            printf("LowLosses: %d at %d\n", low_loss[0],low_loss[1]+1);
            printf("HighWin  : %d at %d\n", high_win[0],high_win[1]+1);
            printf("HighTie  : %d at %d\n", high_tie[0],high_tie[1]+1);
            //if (low_loss==0){return high_win[1]+1;}else{return low_loss[1]+1;}
            if (high_win[0]==0){return high_tie[1]+1;}else{if (high_win[1]==low_loss[1]){return high_win[1]+1;}else if(low_loss[0]==0){return high_win[1]+1;}else{return low_loss[1]+1;}} //best working
            //return high_win[1]+1;
            //return low_loss[1]+1; //no go
        }
        //This is a function that uses recursion to calculate the outcomes of possible plays- the number of ways that the {checkplayer} can win, lose, or tie
        void count_possibilities(uint8_t checkplayer,uint32_t *wins, uint32_t *losses, uint32_t *ties, uint32_t value )
        {
            uint8_t opponentplayer;
            TTTBoard testBoard;
            int i;
            uint32_t wins_a[9]={};
            uint32_t losses_a[9]={};
            uint32_t ties_a[9]={};
            if (checkplayer==1){opponentplayer=2;}else{opponentplayer=1;}
            //if (board.checkWin()==0)return 0;
            for (i=1; i<10; i++)
            {
                this->CopyTo(&testBoard);
                if (testBoard.play(testBoard.curTurn,i)==0)
                {
                    if (testBoard.checkWin()==opponentplayer){losses_a[i-1]=value;memset(wins_a, 0, sizeof(wins_a));memset(ties_a, 0, sizeof(ties_a));i=10;}
                    if (testBoard.checkWin()==checkplayer){wins_a[i-1]=value;memset(losses_a, 0, sizeof(losses_a));memset(ties_a, 0, sizeof(ties_a));i=10;}
                    if (testBoard.checkWin()==3){ties_a[i-1]=value;}//not a winner
                    if (testBoard.checkWin()==0){testBoard.count_possibilities(checkplayer,&wins_a[i-1],&losses_a[i-1], &ties_a[i-1], value /10);}
                }
                else{ }
            }
            for (i=0; i<9; i++)
            {
                *wins+=wins_a[i];
                *losses+=losses_a[i];
                *ties+=ties_a[i];
            }
        }

};
// This function returns a value from 1-9 from either a click on a position or a keypress of a number 1-9
// It also refreshes the screen while looping
int getPlayerInput(SDL_Window* window, SDL_Renderer *renderer)
{
    SDL_Event e;
    int cursorX, cursorY;
    int height, width;
    bool gotInput;
    while (gotInput!=true)
    {
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&e)!=0)
        {
            if (e.type==SDL_MOUSEBUTTONDOWN)
            {
                SDL_GL_GetDrawableSize(window,&width, &height);
                SDL_GetMouseState(&cursorX,&cursorY);
                return (int)((float)cursorX/(float)width*3.0)+(int)((float)cursorY/(float)height*3.0)*3+1;
            }
            if (e.type==SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_1:
                        return 1;
                    case SDLK_2:
                        return 2;
                    case SDLK_3:
                        return 3;
                    case SDLK_4:
                        return 4;
                    case SDLK_5:
                        return 5;
                    case SDLK_6:
                        return 6;
                    case SDLK_7:
                        return 7;
                    case SDLK_8:
                        return 8;
                    case SDLK_9:
                        return 9;
                }
            }

        }
    }
}
int main(int argc, char *argv[]) //main routine
{
    SDL_Init(SDL_INIT_VIDEO);
    //printf("this is a test.\n");
    SDL_Window *windowMain;
    //SDL_Surface *surfMain;
    SDL_Renderer *rendererMain;

    windowMain=SDL_CreateWindow("TicTacToe SDL2 by Logan Thomas Bateman",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,300, 300, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    rendererMain=SDL_CreateRenderer(windowMain,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int nextplay=1;
    TTTBoard Board;
    SDL_RenderSetLogicalSize(rendererMain, 300,300);
    SDL_SetRenderDrawColor(rendererMain, 0xFF,0x00,0x00,0x00); //Red
    SDL_RenderClear(rendererMain);
    SDL_SetRenderDrawColor(rendererMain, 0xFF,0xFF,0x00,0x00); //Yellow
    Board.Draw(rendererMain);
    Board.setTurn(1);
    while (nextplay!=0 & Board.checkWin()==0)
    {
        if (Board.whosTurn()==1)
        {
            printf("Your Turn -> #1-9 : ");
            //scanf("%d",&nextplay ); //old function for before getPlayerInput()
            nextplay=getPlayerInput(windowMain, rendererMain);
        }else
        {
            SDL_Delay(750);
            nextplay=Board.nextComputerPlay(Board.whosTurn());
        }
        int error=Board.play(Board.whosTurn(),nextplay);
        SDL_RenderSetLogicalSize(rendererMain, 300,300);
        SDL_SetRenderDrawColor(rendererMain, 0xFF,0x00,0x00,0x00); //Red
        SDL_RenderClear(rendererMain);
        SDL_SetRenderDrawColor(rendererMain, 0xFF,0xFF,0x00,0x00); //Yellow
        Board.Draw(rendererMain);
        SDL_RenderPresent(rendererMain);

    }

    SDL_Delay(1000);
    //gameloop(rendererMain);

    SDL_DestroyRenderer(rendererMain); //release resources/close stuff
    SDL_DestroyWindow(windowMain);

    return 0; //end
}
