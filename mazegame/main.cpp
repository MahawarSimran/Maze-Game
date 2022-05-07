#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "maze.hpp"
#include "man.hpp"
#include "initialise.hpp"
#include "network.hpp"
using namespace std;


void updateScreen(SDL_Texture* texture){
    SDL_Rect fillRect1 = { 5, 0, 700, 700 };
    SDL_RenderCopy(gameRenderer,gmazeTexture, NULL, &fillRect1);
    show_man();
	SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0x00, 0x00, 0xFF );
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){
			SDL_Rect fillRect = { j*cellWidth, i*cellHeight, cellWidth, cellHeight };
			
			 if(maze[i][j]==2){
				SDL_RenderCopy(gameRenderer, coinTexture, NULL, &fillRect);
			}
            
		}
	}
    SDL_Rect fillRect = { 0, 700, 250, 100 };
    SDL_RenderCopy(gameRenderer, gQuitTexture, NULL, &fillRect);
    show_man();
    student->update();
    student->updateAngle();
    if (texture == DogTexture){
        frender(DogTexture, {student->x, student->y, student-> angle, student->row, student->col,dog_alive,1 });
    }
    else frender(gstudentTexture, {student->x, student->y, student-> angle, student->row, student->col,manLives,1 });
}


int main(int argc, char *args[])
{
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    effect1 = Mix_LoadWAV("Resources/main.wav");
    effect2 = Mix_LoadWAV("Resources/lost.wav");
    const int FPS = 40;
    const int delay = 1000/FPS;
    
    Uint32 frameStart;
    int frameTime;

    if (!init())
    {
        printf("Failed to initialize!\n");
        close();
        return 0;
    }
    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        close();
        return 0;
    }

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Set default current texture
    gameCurrentTexture = gameKeyPressTextures[KEY_MENU];
    const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */        0, 0, "NO" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "YES" },
    };

    const SDL_MessageBoxButtonData leaveButtons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "OK" },      
    };


    const SDL_MessageBoxColorScheme colorScheme = {
        {
            { 0,   0,   0 }, {   0, 255,   0 }, { 255, 255,   0 }, {   0,   0, 255 }, { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        gameWindow, /* .window */
        "WARNING", /* .title */
        "Are you sure you want to quit? All your progress will be lost", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };

    const SDL_MessageBoxData messageboxLeave = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        gameWindow, /* .window */
        "OOPS...", /* .title */
        "The other player has left the game", /* .message */
        SDL_arraysize(leaveButtons), /* .numbuttons */
        leaveButtons, /* .buttons */
        &colorScheme /* .colorScheme */
    };

    //While application is running
    while (!quit)
    {
        //Going to the Main Menu
        while(!gameRunning && !quit && !gameServer && !gameClient){
            while (SDL_PollEvent(&e) != 0)
            {
                //User requests quit
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                //User presses a key
                else if (e.type == SDL_KEYDOWN)
                {
                    //Select surfaces based on key press
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_s:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_2P]){
                            // cout<<"hello"<<endl;
                            formMaze();
                            make_server();
                            createNewGame(0);
                            gameServer = true;
                            Mix_PlayChannel(-1, effect1, 0);
                        }
                        break;
                    case SDLK_c:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU] || gameCurrentTexture == gameKeyPressTextures[KEY_rules])
                                gameCurrentTexture = gameKeyPressTextures[KEY_CREDITS];
                        else if (gameCurrentTexture == gameKeyPressTextures[KEY_2P]){
                            formMaze();
                            make_client();
                            createNewGame(1);
                            gameClient = true;
                            Mix_PlayChannel(-1, effect1, 0);
                        }
                        break;
                    case SDLK_m:
                        gameCurrentTexture = gameKeyPressTextures[KEY_MENU];
                        break;

                    case SDLK_F1:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU]){
                            gameRunning=true;
                            formMaze();
                            createNewGame(0);
                            Mix_PlayChannel(-1, effect1, 0);
                        }
                        break;

                    case SDLK_F2:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU])
                            gameCurrentTexture = gameKeyPressTextures[KEY_2P];
                        break;

                    case SDLK_r:
                        if (gameCurrentTexture == gameKeyPressTextures[KEY_MENU] || gameCurrentTexture == gameKeyPressTextures[KEY_CREDITS])
                            gameCurrentTexture = gameKeyPressTextures[KEY_rules];
                        break;

                    case SDLK_z:
                        

                    default:
                        // gameCurrentTexture = gameKeyPressTextures[KEY_MENU];
                        break;
                    }
                }
                // User presses mouse
                else if ((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) && gameCurrentTexture == gameKeyPressTextures[KEY_MENU])
                {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    KeyPress_start pos = check_position(x, y);
                    if(e.button.button == SDL_BUTTON_LEFT && pos==1){
                        gameRunning=true;
                        formMaze();
                        createNewGame(0);
                        Mix_PlayChannel(-1, effect1, 0);
                    }
                    else if (e.button.button == SDL_BUTTON_LEFT)
                        gameCurrentTexture = gameKeyPressTextures[pos];
                }
            }

            //Clear screen
            SDL_RenderClear(gameRenderer);

            //Render texture to screen
            SDL_RenderCopy(gameRenderer, gameCurrentTexture, NULL, NULL);

            //Update screen
            SDL_RenderPresent(gameRenderer);
        }

        // While running in Single player
        while(gameRunning){
            
            gameCurrentTexture = gameKeyPressTextures[KEY_MENU];
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                {
                    gameRunning = false;
                    quit = true;
                }
                else if(e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_q){
                    //cout<<"yes"<<endl;
                	int buttonid;
			        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
				        SDL_Log("error displaying message box");
				        return 1;
			        }
			        if (buttonid == -1 || buttonid == 0) {
				        cout<<"no selection"<<endl;
			        } 
                    else {
				        cout<<"selection was "<< buttons[buttonid].text<<endl;
                        gameRunning = false;
                        goBackToMenu();
                        break;
			        }
                }
                handleEvent(&e);
            }



            SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderClear( gameRenderer );

            // different game winning/losing condiitons
            if (coinsComplete){
                SDL_RenderCopy(gameRenderer, gstudentWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
                gameRunning = false;
                goBackToMenu();
                break;
            }
            if (manLives<=0){
                SDL_RenderCopy(gameRenderer, gdogWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
                gameRunning = false;
                goBackToMenu();
                break;
            }

            // Moving the student			
            updateScreen(gstudentTexture);				
            coinsComplete = (student->coinsEaten == coins);

           
            SDL_RenderPresent( gameRenderer );	
            frameTime = SDL_GetTicks()-frameStart;
            if(delay>frameTime){
                SDL_Delay(delay - frameTime);
            }
        }

        // When running the game as server
        while (gameServer){
            int leave = 0;
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                
                if( e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                {
                    leave = 1;
                    quit = true;
                }
                else if(e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_q){
                	int buttonid;
			        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
				        SDL_Log("error displaying message box");
				        return 1;
			        }
			        if (buttonid == -1 || buttonid == 0) {
				        cout<<"no selection"<<endl;
			        } 
                    else {
				        cout<<"selection was "<< leaveButtons[buttonid].text<<endl;
                        leave = 1;
			        }
                }
                handleEvent(&e);
            }


            leave_to_buffer(leave);
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len);
            int flag = 0;

            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            flag = buffer_to_leave();

            if(leave == 1){
                gameServer = false;
                goBackToMenu();
                close(sockfd);
                break;
            }
            if(flag == 1){
                int buttonid;
                if (SDL_ShowMessageBox(&messageboxLeave, &buttonid) < 0) {
                    SDL_Log("error displaying message box");
                    return 1;
                }
                gameServer = false;
                
                goBackToMenu();
                close(sockfd);
                break;    
            }

            SDL_SetRenderDrawColor( gameRenderer,  0x00, 0x00, 0x00, 0xFF  );
            SDL_RenderClear( gameRenderer );

            // Different wining/losing conditions
            if (coinsComplete ){
                gameServer = false;
                gameClient = false;
            }
            else if (manLives<=0){
                gameServer = false;
                gameClient = false;
            }

            // Moving the man
            updateScreen(gstudentTexture);
            coinsComplete = (student->coinsEaten == coins);
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len);

            // Sending the maze data
            maze_to_buffer();
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len);

            // getting the position of dog and rendering it
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&cliaddr, &c_len);
            tuple<int, int, int, int, int, int, int> enemyPos = buffer_to_pos(); 
            dog_alive = get<5>(enemyPos);
            frender(gGrassTexture, enemyPos);
            if (dog_alive){
                int x =student->checkCollision(enemyPos);

                if (x==1){
                    manLives--;
                    student->row = student->nxtRow = student->col = student->nxtCol = 1;
                    student->x = student->cellWidth;
                    student->y = student->cellHeight; 
                    Mix_PlayChannel(-1, effect2, 0);
                    SDL_Delay(2000);
                }
            }
            buffer[0] = '0'+ dog_alive;
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );

         // Sending our position to the client
            pos_to_buffer({student->x, student->y, student->angle, student->row, student->col, manLives, coinsComplete});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&cliaddr, c_len );

            if (coinsComplete){
                SDL_RenderCopy(gameRenderer, gstudentWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
            }
            else if (manLives<=0){
                SDL_RenderCopy(gameRenderer, gdogWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
            }
            else{
                SDL_RenderPresent( gameRenderer );
                frameTime = SDL_GetTicks()-frameStart;
                if(delay>frameTime){
                    SDL_Delay(delay - frameTime);
                }
            }
            if(gameServer == false){
                goBackToMenu();
                close(sockfd);
            }
        }
        
        // When running the game as client
        while (gameClient){
            int leave = 0;
            frameStart = SDL_GetTicks();
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                {
                    quit = true;
                    leave = 1;
                }
                else if(e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_q){
                    //cout<<"yes"<<endl;
                	int buttonid;
			        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
				        SDL_Log("error displaying message box");
				        return 1;
			        }
			        if (buttonid == -1 || buttonid == 0) {
				        cout<<"no selection"<<endl;
			        } 
                    else {
				        cout<<"selection was "<< buttons[buttonid].text<<endl;
                        // goBackToMenu();
                        leave = 1;
			        }
                }
                handleEvent(&e);  
            }
            int flag = 0;
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            flag = buffer_to_leave();

            leave_to_buffer(leave);
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );

            if(leave == 1){
                gameClient = false;
                goBackToMenu();
                close(sockfd);
                break;
            }
            if(flag == 1){
                gameClient = false;
                int buttonid;
                if (SDL_ShowMessageBox(&messageboxLeave, &buttonid) < 0) {
                    SDL_Log("error displaying message box");
                    return 1;
                }       
                goBackToMenu();
                close(sockfd);
                break;    
            }

            SDL_SetRenderDrawColor( gameRenderer, 0x00, 0x00, 0x00, 0xFF );
            SDL_RenderClear( gameRenderer );

            // Different winning/losing conditons
            if (coinsComplete){
                gameServer = false;
                gameClient = false;
            }
            else if (manLives<=0){
                gameServer = false;
                gameClient = false;
            }

            recvfrom(sockfd, buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            updateScreen(DogTexture);

            // Changing the taken coins 
            recvfrom(sockfd, buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            change_maze();

            // Sending our position to the server
            pos_to_buffer({student->x, student->y, student->angle, student->row, student->col, dog_alive, 0});
            sendto(sockfd, buffer, 850, MSG_CONFIRM, (struct sockaddr *)&servaddr, s_len );

            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            dog_alive = buffer[0]-'0';   


            // getting the position of server and rendering it
            recvfrom (sockfd,  buffer, 850, MSG_WAITALL, (struct sockaddr *)&servaddr, &s_len);
            tuple<int, int, int, int, int, int, int> studentPos = buffer_to_pos();
            manLives = get<5>(studentPos);
            coinsComplete = get<6>(studentPos);
            frender(gstudentTexture, studentPos);

            if (coinsComplete ){
                SDL_RenderCopy(gameRenderer, gstudentWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
            }
            else if (manLives<=0){
                SDL_RenderCopy(gameRenderer, gdogWon, NULL, NULL);
                SDL_RenderPresent( gameRenderer );
                SDL_Delay(5000);
            }
            else{
                SDL_RenderPresent( gameRenderer );
                frameTime = SDL_GetTicks()-frameStart;
                if(delay>frameTime){
                    SDL_Delay(delay - frameTime);
                }
            }
            if(gameClient == false){
                goBackToMenu();
                close(sockfd);
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
