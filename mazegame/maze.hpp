#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int height = 29;
const int width = 29;

int maze[height][width]={};  //1 for wall, 0 for empty, 2 for egg, 3 for vaccine
int visit[height][width]={};
int coins;




void initMaze(){
    for(int i=0;i<height;i+=1){
        for(int j=0;j<width;j++){
            maze[i][j]=1;
            visit[i][j]=1;
        }
    }

    for(int j=1;j<16;j++){
        maze[1][j]=0;
        maze[19][j]=0;
    }
    for(int j=4;j<width -1;j++){
        maze[24][j]=0;
    }
    for(int j=0;j<5;j++){
        maze[9][j+10]=0;
        maze[11][j+15]=0;
        maze[4][j+15]=0;
        maze[4][j+18]=0;
        maze[7][j+15]=0;
        maze[7][j+20]=0;
        maze[7][22 + j]=0;
        maze[7+j][14]=0;
        maze[20][j+19]=0;
        maze[27][j+2] = 0;
        maze[19][j+14] = 0;
        maze[11][6+j] = 0;
    }
    for(int i=1;i<height-1;i++){
        maze[i][1]=0;
        maze[i][3]=0;
        maze[i][width -2]=0;
    }
    maze[height -4][3] = 1;
    maze[height -2][6] = 1;
    maze[height -3][3] = 1;

    for(int i=0;i<5;i++){
        maze[i+19][6]=0;
        maze[i+19][14]=0;
        maze[i+19][18]=0;
        maze[i+3][10]=0;
        maze[i+7][10]=0;
    }
    for(int i=0;i<11;i++){
        maze[i+8][6]=0;
        maze[i+10][width -5]=0;
    }
    maze[2][3] = 1;
    maze[9][width -5];
    maze[18][15]=0;
    maze[18][16]=0;
    maze[17][16]=0;
    maze[17][17]=0;
    maze[16][17]=0;
    maze[16][18]=0;
    maze[15][18]=0;
    maze[14][18]=0;
    maze[14][19]=0;
    maze[13][19]=0;
    maze[12][19]=0;
    maze[12][20]=0;
    maze[11][20]=0;
    maze[10][20]=0;
    maze[10][21]=0;
    maze[9][21]=0;
    maze[8][21]=0;
    maze[6][22]=0;
    maze[5][22]=0;
    maze[4][22]=0;
    maze[4][23]=0;
    maze[3][23]=0;
    maze[3][24]=0;
    maze[2][24]=0;
    maze[1][24]=0;
    maze[1][25]=0;
    maze[1][26]=0;
    maze[8][5]=0;
    maze[8][4]=0;
    maze[2][15]=0;
    maze[3][15]=0;
    maze[height -3][5]=0;
    maze[height -4][5]=0;
    maze[8][25]=0;
    maze[9][25]=0;
    maze[10][25]=0;
    maze[5][18]=0;
    maze[6][18]=0;
    
}





void fillcoins(){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(maze[i][j]==0){
                maze[i][j]=2;
                coins++;
            }
        }
    }
    maze[1][1]=0;
    coins--;
}



bool junction(int i,int j){
    // cout<<i<<" "<<j<<endl;
    if(maze[i][j-1]==1 && maze[i][j+1]==1 && maze[i-1][j]==0 && maze[i+1][j]==0)return false;
    if(maze[i][j-1]==0 && maze[i][j+1]==0 && maze[i-1][j]==1 && maze[i+1][j]==1)return false;
    return true;
}



void clearMaze(){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            maze[i][j]=0;
            visit[i][j]=0;
        }
    }
}

void formMaze(){
    coins=0;
    srand(time(0));
    clearMaze();
    initMaze();
    fillcoins();
}
