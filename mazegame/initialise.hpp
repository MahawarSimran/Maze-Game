#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>


using namespace std;

const int BOSS_SCORE= 250;
const int COIN_SCORE= 5;

bool gameRunning = false;
bool gameServer = false;
bool gameClient  = false;
const int SCREEN_WIDTH = 696;  
const int SCREEN_HEIGHT = 540*1.5;
const int cellHeight =24;
const int cellWidth = 24;

enum KeyPress_start
{
    KEY_MENU,
    KEY_1P,
    KEY_2P,
    KEY_rules,
    KEY_CREDITS,
    KEY_TOTAL
};

int dog_alive=1;
int manLives =5;
int coinsComplete = 0;
int scared = 0;
int score=0;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window *gameWindow = NULL;

//The window renderer
SDL_Renderer *gameRenderer = NULL;

//The images that correspond to a keypress
SDL_Texture *gameKeyPressTextures[KEY_TOTAL];

//Current displayed image
SDL_Texture *gameCurrentTexture = NULL;

SDL_Texture* gGrassTexture = NULL;
SDL_Texture* gstudentTexture = NULL;
SDL_Texture* gmazeTexture = NULL;
SDL_Texture* DogTexture = NULL;
SDL_Texture* coinTexture = NULL;
SDL_Texture* gQuitTexture = NULL;
SDL_Texture* gstudentWon = NULL;
SDL_Texture* gdogWon = NULL;

man* student = NULL;
Mix_Chunk* effect1;
Mix_Chunk* effect2;
Mix_Chunk* effect3;




KeyPress_start check_position(int x, int y)
{
    int x_start = 198;
    int x_width = 498-198; // width of button is
    int y_width = 345-297;
    int y_single = 297;
    int y_double = 366;
    int y_rules = 537;
    int y_credits = 449;

    if (x >= x_start && x <= (x_start + x_width))
    {
        if (y >= y_single && y <= (y_single + y_width))
        {
            // cout << KEY_1P;
            return KEY_1P;
        }
        else if (y >= y_double && y <= (y_double + y_width))
        {
            // cout << KEY_2P;
            return KEY_2P;
        }
        else if (y >= y_rules && y <= (y_rules + y_width))
        {
            // cout << KEY_rules;
            return KEY_rules;
        }
        else if (y >= y_credits && y <= (y_credits + y_width))
        {
            // cout << KEY_CREDITS;
            return KEY_CREDITS;
        }
        else
        {
            // cout << KEY_MENU;
            return KEY_MENU;
        }
    }
    // cout << KEY_MENU;
    return KEY_MENU;
}

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gameWindow = SDL_CreateWindow("MAZE GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gameWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create renderer for window
            gameRenderer = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gameRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load Start menu surface
    gameKeyPressTextures[KEY_MENU] = loadTexture("Resources/start_menu.png");
    if (gameKeyPressTextures[KEY_MENU] == NULL)
    {
        printf("Failed to load default image!\n");
        success = false;
    }

    //Load 1 Player surface
    gameKeyPressTextures[KEY_1P] = loadTexture("Resources/1_player.png");
    if (gameKeyPressTextures[KEY_1P] == NULL)
    {
        printf("Failed to load 1 Player image!\n");
        success = false;
    }

    //Load 2 Player surface
    gameKeyPressTextures[KEY_2P] = loadTexture("Resources/2_player.png");
    if (gameKeyPressTextures[KEY_2P] == NULL)
    {
        printf("Failed to load 2 Player image!\n");
        success = false;
    }

    gameKeyPressTextures[KEY_rules] = loadTexture("Resources/rules.png");
    if (gameKeyPressTextures[KEY_rules] == NULL)
    {
        printf("Failed to load rules image!\n");
        success = false;
    }

    //Load Credits surface
    gameKeyPressTextures[KEY_CREDITS] = loadTexture("Resources/credits.png");
    if (gameKeyPressTextures[KEY_CREDITS] == NULL)
    {
        printf("Failed to load credits image!\n");
        success = false;
    }

    gstudentTexture = loadTexture("Resources/man.png");
    if (gstudentTexture == NULL)
    {
        printf("Failed to load student image!\n");
        success = false;
    }
    gmazeTexture = loadTexture("Resources/map.png");
    if (gstudentTexture == NULL)
    {
        printf("Failed to load map image!\n");
        success = false;
    }

    DogTexture = loadTexture("Resources/dog.png");
    if (DogTexture == NULL)
    {
        printf("Failed to load dog image!\n");
        success = false;
    }



    gQuitTexture = loadTexture("Resources/quit_button.png");
    if (gQuitTexture == NULL)
    {
        printf("Failed to load Quit Button!\n");
        success = false;
    }

    gstudentWon = loadTexture("Resources/won.png");
    if (gstudentWon == NULL)
    {
        printf("Failed to load student Won image!\n");
        success = false;
    }

    gdogWon = loadTexture("Resources/lost.png");
    if (gdogWon == NULL)
    {
        printf("Failed to load student lost image!\n");
        success = false;
    }

	gGrassTexture = loadTexture("Resources/grass.png");
    if (gGrassTexture == NULL)
    {
        printf("Failed to load grass image!\n");
        success = false;
    }

    coinTexture = loadTexture("Resources/coin.png");
    if (coinTexture == NULL)
    {
        printf("Failed to load coin image!\n");
        success = false;
    }

    return success;
}

void close()
{
    //Free loaded image
    for (int i = 0; i < KEY_TOTAL; i++)
    {
        SDL_DestroyTexture(gameKeyPressTextures[i]);
        gameKeyPressTextures[i] = NULL;
    }
    SDL_DestroyTexture(gameCurrentTexture);
    gameCurrentTexture = NULL;

    SDL_DestroyTexture( gGrassTexture );
	gGrassTexture = NULL;
	SDL_DestroyTexture( gstudentTexture );
	gstudentTexture = NULL;
	SDL_DestroyTexture( gGrassTexture );
	gGrassTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(gameWindow);
    gameWindow = NULL;
    gameRenderer = NULL;

    Mix_FreeChunk(effect1);
    Mix_FreeChunk(effect2);
    Mix_FreeChunk(effect3);
    Mix_CloseAudio();

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *loadTexture(std::string path)
{
    //The final texture
    SDL_Texture *newTexture = NULL;

    //Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gameRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void createNewGame(int type){
    student = new man(type);
    dog_alive=1;
    manLives =3;
    coinsComplete = 0;
    score =0;
}

void goBackToMenu(){
    gameCurrentTexture = gameKeyPressTextures[KEY_MENU];
    SDL_RenderClear(gameRenderer);
    SDL_RenderCopy(gameRenderer, gameCurrentTexture, NULL, NULL);
    SDL_RenderPresent(gameRenderer);
}

void show_man(){
    int x_last = width*cellWidth;
    int y = (height+1)*cellHeight;
    for (int i=1; i<manLives; i++){
        SDL_Rect fillRect = { x_last- 3*cellWidth, y, 2*cellWidth, 2*cellHeight };
        SDL_RenderCopy(gameRenderer, gstudentTexture, NULL, &fillRect);
        x_last -= 3*cellWidth;
    }
}

void frender(SDL_Texture* texture, tuple<int,int,int,int,int, int, int> pos){
    SDL_Rect fillRect = { get<0>(pos), get<1>(pos), student->cellWidth, student->cellHeight };
    if (texture!= gstudentTexture && get<5>(pos)==0) texture= DogTexture;
    else if (texture!=gstudentTexture ) texture = DogTexture;
	if (get<2>(pos) == 180){
		SDL_RenderCopyEx(gameRenderer, texture, NULL, &fillRect, 0 , NULL,SDL_FLIP_HORIZONTAL);
	}
	else SDL_RenderCopyEx(gameRenderer, texture, NULL, &fillRect, get<2>(pos) , NULL,SDL_FLIP_NONE );
}

void handleEvent(SDL_Event* event){
    student->HandleEvent(event);
}
