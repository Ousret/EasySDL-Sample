/*
	ESDL sample game (use EasySDL 0.6.x)
	Panda need to find his way out of this !
	Made by TAHRI Ahmed R. @Ousret
	https://www.github.com/Ousret/
	
	Feel free to improve ! 
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <ESDL.h> // EasySDL C-API (Extended SDL)

/* Give ID to sprite directions */
#define DIR_UP 4
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

#define N_C 30 
#define M_C 40 

int Random (int _iMin, int _iMax);
void generatetrap();

int cellules[N_C][M_C];

int main() {
	
	int current_level = 1, nb_life = 5, posX = 50, posY = 50, update = 1, animate = 1;
	char t_current_level[50], t_nb_life[50];
	
	srand(time (NULL)); //In order to have value base on sys time. (Not needed by EasySDL, just for trap generator)
	/* Init SDL Window with 800x600px named ESDL Sample with text support + audio support */
	SDL_initWindow(800, 600, 0, "ESDL Sample", "M_ICON.png", 1, "global.ttf", 20, 1); //800x600 +tff_support +audio_support
	
	/* Will retrive or create new savegame */
	d_save *savegame = SDL_initProfil("sample.sav");
	
	/* Try to retrive lastest level value and life value */
	char *level = SDL_readParam(savegame, "level");
	char *life = SDL_readParam(savegame, "life");
	if (level && life) {
		/* Assign current level to loaded one */
		current_level = atoi(level);
		nb_life = atoi(life); //Same with life
		
		free(level); //Cleanup memory for level
		free(life);  //Same for life
	}
	
	/* Start a new context named ingame */
	t_context *ingame = SDL_newContext("ESDL Sample", 0, 0, 800, 600);
	
	sprintf(t_current_level, "Level %i", current_level);
	sprintf(t_nb_life, "| Life %i", nb_life);
	
	/* Load primary picture, background image. */
	SDL_newImage(ingame, NULL, "APP_BG_SAMPLE.png", 0, 0);
	
	/* Load Player 1 sprite into ingame context */
	SDL_newSprite(ingame, "panda_sp.png", colorGreenLight, 36, 33, posX, posY, DIR_DOWN, 1, 0);
	/* Place formatted text into ingame context */
	SDL_newText(ingame, NULL, t_current_level, colorGreenLight, 20, 575);
	SDL_newText(ingame, NULL, t_nb_life, colorGreenLight, 90, 575);
	
	/* Preload into RAM our sounds */
	SDL_loadSound("explosion.wav");
	SDL_loadSound("Female/unstoppable.wav"); 
	
	/* Fill matrice with trap */
	generatetrap();
	
	while (1) {
		
		/* Check keyboard btn status and act ! */
		if (SDL_isKeyPressed(SDLK_UP)) {
			if (posY > 5) {
				posY-=5;
				update = 1;
			}
			/* Assign new screen pos for P1 sprite */
			SDL_editSprite(ingame, 0, posX, posY, DIR_UP, animate, 0);
			animate++;
		}
		
		if (SDL_isKeyPressed(SDLK_DOWN)) {
			if (posY < 540) {
				posY+=5;
				update = 1;
			}
			SDL_editSprite(ingame, 0, posX, posY, DIR_DOWN, animate, 0);
			animate++;
		}
		
		if (SDL_isKeyPressed(SDLK_LEFT)) {
			if (posX > 5) {
				posX-=5;
				update = 1;
			}
			SDL_editSprite(ingame, 0, posX, posY, DIR_LEFT, animate, 0);
			animate++;
		}
		
		if (SDL_isKeyPressed(SDLK_RIGHT)) {
			if (posX < 780) {
				posX+=5;
				update = 1;
			}
			SDL_editSprite(ingame, 0, posX, posY, DIR_RIGHT, animate, 0);
			animate++;
		}
		
		/* Check if player win */
		if (posX > 590 && posY > 440) {
			
			/* Play sound from RAM */
			SDL_playSound("Female/unstoppable.wav");
			current_level++;
			sprintf(t_current_level, "Level %i", current_level);
			SDL_editText(ingame, 0, t_current_level, colorGreenLight, 20, 575);
			if (SDL_nbImage(ingame) > 1) SDL_delImage(ingame, 1);
			posX = 50;
			posY = 50;
			SDL_editSprite(ingame, 0, posX, posY, DIR_RIGHT, animate, 0);
			generatetrap();
			SDL_generate(ingame);
			
		}
		
		/* Check if player is under trap */
		if (cellules[600/posY][800/posX] == 1) {
		
			SDL_editSprite(ingame, 0, posX, posY, DIR_UP, animate, 1);
			if (SDL_nbImage(ingame) < 2) {
				SDL_newImage(ingame, NULL, "impact.png", posX, posY);
			}else{
				SDL_editImage(ingame, 1, posX, posY);
			}
			
			SDL_generate(ingame);
			SDL_playSound("explosion.wav");
			//generatetrap();
			nb_life--;
			posX = 50;
			posY = 50;
			
			if (nb_life == 0) {
				current_level = 1;
				nb_life = 5;
				sprintf(t_current_level, "Level %i", current_level);
				SDL_editText(ingame, 0, t_current_level, colorGreenLight, 20, 575);
				sprintf(t_nb_life, "| Life %i", nb_life);
				SDL_editText(ingame, 1, t_nb_life, colorGreenLight, 90, 575);
				SDL_delImage(ingame, 1);
			}else{
				sprintf(t_nb_life, "| Life %i", nb_life);
				SDL_editText(ingame, 1, t_nb_life, colorGreenLight, 90, 575);
			}
			
		}
		
		/* If user request exit, we need to quit while() */
		if (SDL_requestExit()) break;
		
		/* If player is moving */
		if (update) {
			SDL_generate(ingame);
			update = 0;
		}else{
			SDL_Delay(50);
		}
		
		/* Change sprite */
		if (animate >= 4) animate = 1;
	
	}
	
	/* Create string with life and level */
	sprintf(t_nb_life, "%i", nb_life);
	sprintf(t_current_level, "%i", current_level);
	
	/* Write to RAM new params, values */
	SDL_writeParam(savegame, "level", t_current_level);
	SDL_writeParam(savegame, "life", t_nb_life);
	
	SDL_saveProfil(savegame); //Save to file
	SDL_freeProfil(savegame); //Free savegame from RAM
	
	/* Unload all sounds from RAM */
	SDL_unloadallSound();
	/* Cleanup ingame context */
	SDL_freeContext(ingame);
	
	return 0;

}

int Random (int _iMin, int _iMax)
{
	return (rand() % (_iMax - _iMin) + _iMin);
}

void generatetrap() {
	
	int i = 0, j = 0;
	
	memset(cellules, 0, sizeof(cellules));
	
	/* Generate trap in the map */
	for (i = 0; i < N_C; i++) {
		for (j = 0; j < M_C; j ++) {
			if (Random(0, 80) > 70) cellules[i][j] = 1;j+=5;
		}
	}
	
}
