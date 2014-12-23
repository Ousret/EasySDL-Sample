/*
	ESDL sample game
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
#include <ESDL.h> 

#define DIR_UP 4
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3

#define N_C 30 
#define M_C 40 

int cellules[N_C][M_C];

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

int main() {
	
	int current_level = 1, nb_life = 5, posX = 50, posY = 50, update = 1, animate = 1;
	char t_current_level[50], t_nb_life[50];
	
	t_window *ingame = SDL_newWindow("ESDL Sample", 0, 0, 800, 600);
	
	srand(time (NULL));
	
	memset(t_current_level, 0, sizeof(t_current_level));
	memset(t_nb_life, 0, sizeof(t_nb_life));
	
	sprintf(t_current_level, "Level %i", current_level);
	sprintf(t_nb_life, "| Life %i", nb_life);
	
	SDL_init(800, 600, 0, "ESDL Sample", "M_ICON.png", 1, "global.ttf", 20, 1); //800x600 +tff_support +audio_support
	
	/* Begin to load what is needed for ingame window */
	SDL_newTexture(ingame, NULL, "APP_BG_SAMPLE.png", 0, 0, 800, 600);
	SDL_newSprite(ingame, "panda_sp.png", colorGreenLight, 145, 144, 36, 33, posX, posY, DIR_DOWN, 1, 0);
	SDL_newText(ingame, NULL, t_current_level, colorGreenLight, 20, 575);
	SDL_newText(ingame, NULL, t_nb_life, colorGreenLight, 90, 575);
	
	SDL_preloadwav("explosion.wav");
	SDL_preloadwav("Female/unstoppable.wav"); 

	generatetrap();
	
	while (1) {
		
		SDL_UpdateEvents(&in); //Manual update event mapping, SDL_generate do update mapping too..
		
		if (SDL_isKeyPressed(SDLK_UP)) {
			if (posY > 5) {
				posY-=5;
				update = 1;
			}
			SDL_modSprite(ingame, 0, posX, posY, DIR_UP, animate, 0);
			animate++;
		}
		
		if (SDL_isKeyPressed(SDLK_DOWN)) {
			if (posY < 540) {
				posY+=5;
				update = 1;
			}
			SDL_modSprite(ingame, 0, posX, posY, DIR_DOWN, animate, 0);
			animate++;
		}
		
		if (SDL_isKeyPressed(SDLK_LEFT)) {
			if (posX > 5) {
				posX-=5;
				update = 1;
			}
			SDL_modSprite(ingame, 0, posX, posY, DIR_LEFT, animate, 0);
			animate++;
		}
		
		if (SDL_isKeyPressed(SDLK_RIGHT)) {
			if (posX < 780) {
				posX+=5;
				update = 1;
			}
			SDL_modSprite(ingame, 0, posX, posY, DIR_RIGHT, animate, 0);
			animate++;
		}
		
		/* Check if player win */
		if (posX > 590 && posY > 440) {
			
			SDL_playwav("Female/unstoppable.wav", 1, NULL);
			current_level++;
			sprintf(t_current_level, "Level %i", current_level);
			SDL_modText(ingame, 0, t_current_level, colorGreenLight, 20, 575);
			SDL_delTexture(ingame, 1);
			posX = 50;
			posY = 50;
			generatetrap();
			
		}
		
		/* Check if player is under trap */
		if (cellules[600/posY][800/posX] == 1) {
		
			SDL_modSprite(ingame, 0, posX, posY, DIR_UP, animate, 1);
			if (SDL_nbTexture(ingame) < 2) {
				SDL_newTexture(ingame, NULL, "impact.png", posX, posY, 120, 120);
			}else{
				SDL_modTexture(ingame, 1, "impact.png", posX, posY, 120, 120);
			}
			
			SDL_generate(ingame);
			SDL_playwav("explosion.wav", 1, NULL);
			//generatetrap();
			nb_life--;
			posX = 50;
			posY = 50;
			
			if (nb_life == 0) {
				current_level = 1;
				nb_life = 5;
				sprintf(t_current_level, "Level %i", current_level);
				SDL_modText(ingame, 0, t_current_level, colorGreenLight, 20, 575);
				sprintf(t_nb_life, "| Life %i", nb_life);
				SDL_modText(ingame, 1, t_nb_life, colorGreenLight, 90, 575);
				SDL_delTexture(ingame, 1);
			}else{
				sprintf(t_nb_life, "| Life %i", nb_life);
				SDL_modText(ingame, 1, t_nb_life, colorGreenLight, 90, 575);
			}
			
		}
		
		if (in.quit == 1) break;
		
		if (update) {
			SDL_generate(ingame);
			update = 0;
		}
		
		SDL_Delay(50);
		
		if (animate >= 4) animate = 1;
	
	}
	
	SDL_freeWindow(ingame);
	return 0;

}
