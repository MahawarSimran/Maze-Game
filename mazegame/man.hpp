#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>


class man{
public:
    static SDL_Renderer* Renderer;
    man(int n);
    void HandleEvent(SDL_Event* event);
    int checkCollision(tuple<int,int,int,int,int,int,int> positiondog);
    void update();
    int updateAngle();
    bool tryChanging();
    void keepMoving();
    int type = 0; // 0 for man and 1 for dog
    int coinsEaten = 0;
    int frame = 0;
    string nextDirection = "still";
    string currDirection = "still";
    int x;
    int y;
    int nxtRow;
    int nxtCol;
    int row;
    int col;
    int cellWidth = 24;
    int cellHeight = 24;
    int angle=0;    
};

man::man(int n){
    type = n;
    if (type ==0){
        row = nxtRow = col = nxtCol = 1;
        x = cellWidth;
        y = cellHeight; 
    }
    else{   
        row = col = 27;
        x = y = row*cellWidth;
    }
}

void man::HandleEvent(SDL_Event* event){
    if( event->type == SDL_KEYDOWN )
    {
        switch( event->key.keysym.sym )
        {
            case SDLK_UP: {nextDirection = "up"; break;}
            case SDLK_DOWN: nextDirection = "down"; break;
            case SDLK_LEFT: nextDirection = "left"; break;
            case SDLK_RIGHT: nextDirection = "right"; break;
        }
    }
    
}

int man::checkCollision(tuple<int,int,int,int,int,int,int> positiondog){
    int rowdog = get<3>(positiondog);
    int coldog = get<4>(positiondog);
    int xdog = get<0>(positiondog);
    int ydog = get<1>(positiondog); 
    if(abs(xdog- x)<=5 && abs(ydog- y)<=5){
        return 1;       
    }
    return 0;   // when nothing happens
}

bool man::tryChanging(){
    int tcol;
    int trow;
    if(nextDirection=="still" || nextDirection==currDirection){
        return false;
    }
    else if(nextDirection=="left"){
        if(y%cellWidth!=0){return false;}
        if(x%cellWidth==0 && y%cellWidth==0){
        	row = y/cellWidth;
        	col = x/cellWidth;
            if(maze[row][col]==2 && type ==0){
                maze[row][col]=0;
                coinsEaten++;
            }
            else if(maze[row][col]==3 && type ==0){
                maze[row][col]=0;
            }
        }
        trow = row;
        tcol = col-1;
    }
    else if(nextDirection=="right"){
        if(y%cellWidth!=0){return false;}
        if(x%cellWidth==0 && y%cellWidth==0){
        	row = y/cellWidth;
        	col = x/cellWidth;
            if(maze[row][col]==2 && type ==0){
                maze[row][col]=0;
                coinsEaten++;
            }
            else if(maze[row][col]==3 && type ==0){
                maze[row][col]=0;
            }
        }
        trow = row;
        tcol = col+1;
        
    }
    else if(nextDirection=="up"){
    	//cout<<"f";
        if(x%cellWidth!=0){return false;}
        if(x%cellWidth==0 && y%cellWidth==0){
        	row = y/cellWidth;
        	col = x/cellWidth;
            if(maze[row][col]==2 && type==0){
                maze[row][col]=0;
                coinsEaten++;
            }
            else if(maze[row][col]==3 && type ==0){
                maze[row][col]=0;
            }
        }
        trow = row-1;
        tcol = col;
        
    }
    else if(nextDirection=="down"){
        if(x%cellWidth!=0){return false;}
        if(x%cellWidth==0 && y%cellWidth==0){
        	row = y/cellWidth;
        	col = x/cellWidth;
            if(maze[row][col]==2 && type==0){
                maze[row][col]=0;
                coinsEaten++;
            }
            else if(maze[row][col]==3 && type ==0){
                maze[row][col]=0;
            }
        }
        trow = row+1;
        tcol = col;
        
    }
    else if(nextDirection=="still"){
        trow = row;
        tcol = col;
    }

    if(maze[trow][tcol]==1)return false;
    nxtRow = trow;
    nxtCol = tcol;
    currDirection = nextDirection;
    nextDirection = "still";
    return true;
}

void man::keepMoving(){
    if(currDirection == "still")return;
    else if(currDirection=="left"){
        if(x%cellWidth!=0){x=x-3;return;}
        else{
        	if(x%cellWidth==0 && y%cellWidth==0){
		    	row = y/cellWidth;
		    	col = x/cellWidth;
                if(maze[row][col]==2 && type==0){
                    maze[row][col]=0;
                    coinsEaten++;
                }
                else if(maze[row][col]==3 && type ==0){
                    maze[row][col]=0;
                }
		    }
            if(maze[row][col-1]==1){
                currDirection = "still";
            }
            else{
            	nxtCol = col-1;
            	nxtRow = row;
                x-=3;
            }
        }
    }
    else if(currDirection=="right"){
        if(x%cellWidth!=0){x+=3;return;}
        else{
        	if(x%cellWidth==0 && y%cellWidth==0){
		    	row = y/cellWidth;
		    	col = x/cellWidth;
                if(maze[row][col]==2 && type==0){
                    maze[row][col]=0;
                    coinsEaten++;
                }
                else if(maze[row][col]==3 && type ==0){
                    maze[row][col]=0;
                }
		    }
            if(maze[row][col+1]==1){
                currDirection = "still";
            }
            else{
                nxtCol = col+1;
            	nxtRow = row;
                x+=3;
            }
        }
    }
    else if(currDirection=="up"){
        if(y%cellWidth!=0){y-=3;return;}
        else{
        	if(x%cellWidth==0 && y%cellWidth==0){
		    	row = y/cellWidth;
		    	col = x/cellWidth;
                if(maze[row][col]==2 && type==0){
                    maze[row][col]=0;
                    coinsEaten++;
                }
                else if(maze[row][col]==3 && type ==0){
                    maze[row][col]=0;
                }
		    }
            if(maze[row-1][col]==1){
                currDirection = "still";
            }
            else{
                nxtCol = col;
            	nxtRow = row-1;
                y-=3;
            }
        }
    }
    else if(currDirection=="down"){
        if(y%cellWidth!=0){y+=3;return;}
        else{
        	if(x%cellWidth==0 && y%cellWidth==0){
		    	row = y/cellWidth;
		    	col = x/cellWidth;
                if(maze[row][col]==2 && type==0){
                    maze[row][col]=0;
                    coinsEaten++;
                }
                else if(maze[row][col]==3 && type ==0){
                    maze[row][col]=0;
                }
		    }
            if(maze[row+1][col]==1){
                currDirection = "still";
            }
            else{
                nxtCol = col;
            	nxtRow = row+1;
                y+=3;
            }
        }
    }
}

void man::update(){
    if(!tryChanging())keepMoving();
}

int man::updateAngle(){
    if (currDirection =="still") return angle;  // this will retain the angle
    if (currDirection =="down") angle = 90;
    else if (currDirection == "left") angle = 180;
    else if (currDirection =="up") angle = 270;
    else if (currDirection == "right") angle = 0;
    return angle;   // else we will update the angle
}
