
// gcc jogo.c -o jogo -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

/*
tabela de erros:
    1 erro de criação de textura
    2 erro de criação de surface
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define JANELA_W (700)
#define JANELA_H (700)

#define FPS (30)	//quadros por segundo
#define SetBonus (15) //a cada quantos segundos vem o bonus
#define VIDAS (5)
#define Velocidade_Inicial (13)

SDL_Window* janela = NULL;
SDL_Renderer* render = NULL;
SDL_Surface* surface = NULL;

Mix_Music* musica = NULL;
Mix_Chunk* bonus_som = NULL;
Mix_Chunk* menu_som = NULL;
Mix_Chunk* jogo_som = NULL;

TTF_Font * font = NULL;
SDL_Color branco = { 255, 255, 255 };
SDL_Surface * surface_texto = NULL;
SDL_Texture * texture_texto = NULL;

SDL_Surface* background_menu = NULL;
SDL_Texture* textura_fundo_menu = NULL;
SDL_Texture* caixa = NULL;
SDL_Surface* surface_menu = NULL;

SDL_Surface* background_jogo = NULL;
SDL_Texture* textura_fundo_jogo = NULL;
SDL_Texture* nave = NULL;
SDL_Surface* surface_nave = NULL;

SDL_Texture* asteroide = NULL;
SDL_Surface* surface_asteroide = NULL;

SDL_Texture* asteroide2 = NULL;
SDL_Surface* surface_asteroide2 = NULL;

SDL_Texture* planeta = NULL;
SDL_Surface* surface_planeta = NULL;

SDL_Texture* golden_record = NULL;
SDL_Surface* surface_Golden_Record = NULL;

SDL_Texture* coracao_1 = NULL;
SDL_Texture* coracao_2 = NULL;
SDL_Texture* coracao_3 = NULL;
SDL_Texture* coracao_4 = NULL;
SDL_Texture* coracao_5 = NULL;
SDL_Surface* surface_coracao = NULL;

SDL_Surface* background_creditos = NULL;
SDL_Texture* textura_fundo_creditos = NULL;

SDL_Surface* background_como_jogar = NULL;
SDL_Texture* textura_fundo_como_jogar = NULL;

SDL_Surface* background_ranking = NULL;
SDL_Texture* textura_fundo_ranking = NULL;


typedef struct 
{
    int id;
    int pts;
} JogadorType;

typedef struct 
{
	JogadorType J1, J2, J3, J4, J5, J6, J7, J8, J9, J10;
    int id_final;
} RankingType;

RankingType podio;

void menu(void);

void Erro(int codigo){
    switch(codigo){
        case 1:
            printf("Erro de criação de textura\n");
            SDL_DestroyRenderer(render);
            SDL_DestroyWindow(janela);
            SDL_Quit();
        break;
        case 2:
            printf("Erro de criação de surface\n");
            SDL_DestroyRenderer(render);
            SDL_DestroyWindow(janela);
            SDL_Quit();
        break;
    }
    return;
}

bool impacto_detection(SDL_Rect obj1, SDL_Rect obj2){
	int x1, x2, y1, y2, r1, r2;

	x1 = obj1.x + obj1.w/2;
	y1 = obj1.y + obj1.h/2;

	x2 = obj2.x + obj2.w/2;
	y2 = obj2.y + obj2.h/2;

	r1 = obj1.w/2;
	r2 = obj2.w/2;

	double distancia = sqrt( ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)) );

	if(distancia <= (r1+r2))
		return true;
	else
		return false;
}

void ordenar_podio(int pot, int idtf){
	if(pot >= podio.J1.pts){
		podio.J10 = podio.J9;
		podio.J9 = podio.J8;
		podio.J8 = podio.J7;
		podio.J7 = podio.J6;
		podio.J6 = podio.J5;
		podio.J5 = podio.J4;
		podio.J4 = podio.J3;
		podio.J3 = podio.J2;
		podio.J2 = podio.J1;
		podio.J1.id = idtf;
		podio.J1.pts = pot;
	}
	else if(pot >= podio.J2.pts){
	   	podio.J10 = podio.J9;
		podio.J9 = podio.J8;
		podio.J8 = podio.J7;
		podio.J7 = podio.J6;
		podio.J6 = podio.J5;
	   	podio.J5 = podio.J4;
		podio.J4 = podio.J3;
		podio.J3 = podio.J2;
		podio.J2.id = idtf;
		podio.J2.pts = pot;
	}
	else if(pot >= podio.J3.pts){
		podio.J10 = podio.J9;
		podio.J9 = podio.J8;
		podio.J8 = podio.J7;
		podio.J7 = podio.J6;
		podio.J6 = podio.J5;
	   	podio.J5 = podio.J4;
		podio.J4 = podio.J3;
		podio.J3.id = idtf;
		podio.J3.pts = pot;
	}
	else if(pot >= podio.J4.pts){
		podio.J10 = podio.J9;
		podio.J9 = podio.J8;
		podio.J8 = podio.J7;
		podio.J7 = podio.J6;
		podio.J6 = podio.J5;
		podio.J5 = podio.J4;
		podio.J4.id = idtf;
		podio.J4.pts = pot;
	}
	else if(pot >= podio.J5.pts){
		podio.J10 = podio.J9;
		podio.J9 = podio.J8;
		podio.J8 = podio.J7;
		podio.J7 = podio.J6;
		podio.J6 = podio.J5;
		podio.J5.id = idtf;
		podio.J5.pts = pot;
	}
	else if(pot >= podio.J6.pts){
		podio.J10 = podio.J9;
		podio.J9 = podio.J8;
		podio.J8 = podio.J7;
		podio.J7 = podio.J6;
		podio.J6.id = idtf;
		podio.J6.pts = pot;
	}
	else if(pot >= podio.J7.pts){
		podio.J10 = podio.J9;
		podio.J9 = podio.J8;
		podio.J8 = podio.J7;
		podio.J7.id = idtf;
		podio.J7.pts = pot;
	}
	else if(pot >= podio.J8.pts){
		podio.J10 = podio.J9;
		podio.J9 = podio.J8;
		podio.J8.id = idtf;
		podio.J8.pts = pot;
	}
	else if(pot >= podio.J9.pts){
		podio.J10 = podio.J9;
		podio.J9.id = idtf;
		podio.J9.pts = pot;
	}
	else if(pot >= podio.J10.pts){
		podio.J10.id = idtf;
		podio.J10.pts = pot;		
	}
}
void jogo(void){
	int identificacao = podio.id_final += 1;
	// tamanho e posição da nave
    SDL_Rect dest, rGolden, rPlaneta, rAsteroide, rAsteroide2, rC1, rC2, rC3, rC4, rC5;

    char buffer[40];
    SDL_Surface* Superficie_jogo = NULL;

    SDL_Texture* Pontos_textura = NULL;
    SDL_Rect rPontos;
    rPontos.h = 0;
    rPontos.w = 0;
    rPontos.x = 540;
    rPontos.y = 430;
	
    SDL_Texture* Pontos_textura_2 = NULL;
    SDL_Rect rPontos2;
    rPontos2.h = 0;
    rPontos2.w = 0;
    rPontos2.x = 540;
    rPontos2.y = 455;

    SDL_Texture* Id_textura = NULL;
    SDL_Rect rId;
    rId.h = 0;
    rId.w = 0;
    rId.x = 540;
    rId.y = 480;

    SDL_Texture* FimDeJogo_textura = NULL;
    SDL_Rect rFDJ;
    rFDJ.h = 0;
    rFDJ.w = 0;

    sprintf(buffer, "pontos:");
    Superficie_jogo = TTF_RenderText_Solid(font, buffer, branco);
    Pontos_textura = SDL_CreateTextureFromSurface(render, Superficie_jogo);
    SDL_QueryTexture(Pontos_textura, NULL, NULL, &rPontos.w, &rPontos.h);
        
    srand(time(NULL));

    dest.w = 75;//define o tamanho da nave
    dest.h = 75;
    dest.x = 262;//inicializa a posição da nave no centro da area do jogo
    dest.y = 610;

    rGolden.w = 75;//define o tamanho de cada objeto que aparecerá na tela
    rGolden.h = 75;
    rAsteroide.w = 75;
    rAsteroide.h = 75;
    rPlaneta.w = 175;
    rPlaneta.h = 175;
    rAsteroide2.w = 75;
    rAsteroide2.h = 75;

    rC1.w = 75;
    rC1.h = 75;
    rC2.w = 75;
    rC2.h = 75;
    rC3.w = 75;
    rC3.h = 75;
    rC4.w = 75;
    rC4.h = 75;
    rC5.w = 75;
    rC5.h = 75;

    rC1.x = 575;
    rC2.x = 575;
    rC3.x = 575;
    rC4.x = 575;
    rC5.x = 575;

    rC1.y = 25;
    rC2.y = 100;
    rC3.y = 175;
    rC4.y = 250;
    rC5.y = 325;

    rGolden.x = rand() % 450; //inicializa a posição horizonal do bonus aleatoriamente
    rGolden.y = -80;

    int velocidade = 0, pontos = 0, vidas = VIDAS, SPEED = Velocidade_Inicial;
    bool left = false, right = false, bonus;

    // definido para 1 quando o botão de fechar a janela é pressionado
    int encerrar = 0;

    Uint32 tempo_bonus = SDL_GetTicks() + SetBonus*1000;
    Uint32 tempo_ant, tempo;

    while(!encerrar){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {   
            switch (event.type)
            {
            case SDL_QUIT:
                return;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = true;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = true;
                    break;
                case SDL_SCANCODE_ESCAPE:
                    encerrar = 1;
                    break;         
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = false;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = false;
                    break;
                }
                break;
            }
        }

        if(vidas < 0)   encerrar = 1;

        //movimentação nave
        velocidade = 0;
        if(left && !right)
        	velocidade = -Velocidade_Inicial;
        if(!left && right)
        	velocidade = Velocidade_Inicial;
        dest.x += velocidade;

        //colisão nave
        if(dest.x < 0)	dest.x = 0;
        if(dest.x > 525 -dest.w)	dest.x = 525 -dest.w;

        //velocidade de cada objeto
        rAsteroide.y += SPEED+12;
        rAsteroide2.y += SPEED+10;
        rPlaneta.y += SPEED+8;

        if(rAsteroide.y > 800){
            pontos += 10;
            rAsteroide.x = rand() % 450;
            rAsteroide.y = -rAsteroide.h;
        }
        if(rAsteroide2.y > 800){
            pontos += 10;
            rAsteroide2.x = rand() % 450;
            rAsteroide2.y = -rAsteroide2.h-100;
        }
        if(rPlaneta.y > 850){
            pontos += 20;
            rPlaneta.x = rand() % 350;
            rPlaneta.y = -rPlaneta.h-150;
        }

        //impactos com elementos
        if(impacto_detection( rAsteroide, dest)){
        	Mix_PlayChannel( -1, jogo_som, 0 );
            --vidas;
            pontos -= 20;
            rAsteroide.y = -rAsteroide.h;
            rAsteroide.x = rand() % 450;
        }
        if(impacto_detection( rAsteroide2, dest)){
        	Mix_PlayChannel( -1, jogo_som, 0 );
            --vidas;
            pontos -= 20;
            rAsteroide2.y = -rAsteroide2.h-100;
            rAsteroide2.x = rand() % 450;
        }
        if(impacto_detection( rPlaneta, dest)){
        	Mix_PlayChannel( -1, jogo_som, 0 );
            --vidas;
            pontos -= 30;
            rPlaneta.y = -rPlaneta.h-150;
            rPlaneta.x = rand() % 350;
        }

        //bonus
        bonus = SDL_TICKS_PASSED(SDL_GetTicks(), tempo_bonus);

        if(impacto_detection(rGolden, dest)){
        	Mix_PlayChannel( -1, bonus_som, 0 );
            SPEED += 5;
            pontos += 500;
            rGolden.y = 800;
        }

        if(bonus){
            rGolden.y += SPEED+10;
        }

        if(rGolden.y > 800){
            bonus = false;
            tempo_bonus = SDL_GetTicks() + SetBonus*1000;
            rGolden.x = rand() % 450;
            rGolden.y = -80;
        }

        //info de tela
        sprintf(buffer, "%d", pontos);
        Superficie_jogo = TTF_RenderText_Solid(font, buffer, branco);
        Pontos_textura_2 = SDL_CreateTextureFromSurface(render, Superficie_jogo);
        SDL_QueryTexture(Pontos_textura_2, NULL, NULL, &rPontos2.w, &rPontos2.h);
        
        sprintf(buffer, "ID: %d", identificacao);
        Superficie_jogo = TTF_RenderText_Solid(font, buffer, branco);
        Id_textura = SDL_CreateTextureFromSurface(render, Superficie_jogo);
        SDL_QueryTexture(Id_textura, NULL, NULL, &rId.w, &rId.h);
        
        //pontos
        //id

        SDL_RenderClear(render);
        SDL_RenderCopy(render, textura_fundo_jogo, NULL, NULL);
        SDL_RenderCopy(render, asteroide, NULL, &rAsteroide);
        SDL_RenderCopy(render, asteroide2, NULL, &rAsteroide2);
        SDL_RenderCopy(render, planeta, NULL, &rPlaneta);
        SDL_RenderCopy(render, nave, NULL, &dest);
        SDL_RenderCopy(render, golden_record, NULL, &rGolden);
        if(vidas >= 1)	SDL_RenderCopy(render, coracao_1, NULL, &rC1);
        if(vidas >= 2)	SDL_RenderCopy(render, coracao_2, NULL, &rC2);
        if(vidas >= 3)	SDL_RenderCopy(render, coracao_3, NULL, &rC3);
        if(vidas >= 4)	SDL_RenderCopy(render, coracao_4, NULL, &rC4);
        if(vidas >= 5)	SDL_RenderCopy(render, coracao_5, NULL, &rC5);
        SDL_RenderCopy(render, Pontos_textura, NULL, &rPontos);
        SDL_RenderCopy(render, Pontos_textura_2, NULL, &rPontos2);
        SDL_RenderCopy(render, Id_textura, NULL, &rId);
        SDL_RenderPresent(render);

        SDL_Delay(1000/FPS);
    }
    
    encerrar = 0;

    while(!encerrar){ //tela de fim de jogo
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {   
            switch (event.type)
            {
            case SDL_QUIT:
                return;
                break;
            case SDL_KEYDOWN:
               switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    encerrar = 1;
                    break;         
                }
                break;
            }
        }

        //info de tela
        sprintf(buffer, "%d", pontos);
        Superficie_jogo = TTF_RenderText_Solid(font, buffer, branco);
        Pontos_textura_2 = SDL_CreateTextureFromSurface(render, Superficie_jogo);
        SDL_QueryTexture(Pontos_textura_2, NULL, NULL, &rPontos2.w, &rPontos2.h);
        
        sprintf(buffer, "ID: %d", identificacao);
        Superficie_jogo = TTF_RenderText_Solid(font, buffer, branco);
        Id_textura = SDL_CreateTextureFromSurface(render, Superficie_jogo);
        SDL_QueryTexture(Id_textura, NULL, NULL, &rId.w, &rId.h);
        
        sprintf(buffer, "FIM DE JOGO: Esc para sair!");
        Superficie_jogo = TTF_RenderText_Solid(font, buffer, branco);
        FimDeJogo_textura = SDL_CreateTextureFromSurface(render, Superficie_jogo);
        SDL_QueryTexture(FimDeJogo_textura, NULL, NULL, &rFDJ.w, &rFDJ.h);
        rFDJ.x = (700 - rFDJ.w)/2;   
        rFDJ.y = (700 - rFDJ.h)/2;   
        
        SDL_RenderClear(render);
        SDL_RenderCopy(render, textura_fundo_jogo, NULL, NULL);
        SDL_RenderCopy(render, Pontos_textura, NULL, &rPontos);
        SDL_RenderCopy(render, Pontos_textura_2, NULL, &rPontos2);
        SDL_RenderCopy(render, Id_textura, NULL, &rId);
        SDL_RenderCopy(render, FimDeJogo_textura, NULL, &rFDJ);
        SDL_RenderPresent(render);

        SDL_Delay(1000/FPS);
    }

    ordenar_podio(pontos, identificacao);

    SDL_FreeSurface(Superficie_jogo);
    return;
}

void ranking(void){
        char buffer[40];
        SDL_Surface* Superficie = NULL;

        SDL_Texture* Titulo_textura = NULL;
        SDL_Rect rTitulo;
        rTitulo.h = 0;
        rTitulo.w = 0;
        rTitulo.x = 180;
        rTitulo.y = 175;

        SDL_Texture* t1 = NULL;
        SDL_Rect r1;
        r1.h = 0;
        r1.w = 0;
        r1.x = 187;
        r1.y = 200;

        SDL_Texture* t2 = NULL;
        SDL_Rect r2;
        r2.h = 0;
        r2.w = 0;
        r2.x = 180;
        r2.y = 225;

        SDL_Texture* t3 = NULL;
        SDL_Rect r3;
        r3.h = 0;
        r3.w = 0;
        r3.x = 180;
        r3.y = 250;

        SDL_Texture* t4 = NULL;
        SDL_Rect r4;
        r4.h = 0;
        r4.w = 0;
        r4.x = 180;
        r4.y = 275;

        SDL_Texture* t5 = NULL;
        SDL_Rect r5;
        r5.h = 0;
        r5.w = 0;
        r5.x = 180;
        r5.y = 300;

        SDL_Texture* t6 = NULL;
        SDL_Rect r6;
        r6.h = 0;
        r6.w = 0;
        r6.x = 180;
        r6.y = 325;

        SDL_Texture* t7 = NULL;
        SDL_Rect r7;
        r7.h = 0;
        r7.w = 0;
        r7.x = 180;
        r7.y = 350;
    	        
        SDL_Texture* t8 = NULL;
        SDL_Rect r8;
        r8.h = 0;
        r8.w = 0;
        r8.x = 180;
        r8.y = 375;

    	SDL_Texture* t9 = NULL;
        SDL_Rect r9;
        r9.h = 0;
        r9.w = 0;
        r9.x = 180;
        r9.y = 400;

        SDL_Texture* t10 = NULL;
        SDL_Rect r10;
        r10.h = 0;
        r10.w = 0;
        r10.x = 187;
        r10.y = 425;

    	while(1){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {   
            switch (event.type)
            {
            case SDL_KEYDOWN:
                SDL_FreeSurface(Superficie);
                return;
                break;
            }
        }
        sprintf(buffer, "pos  Id  Pontos");
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        Titulo_textura = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(Titulo_textura, NULL, NULL, &rTitulo.w, &rTitulo.h);
        
        sprintf(buffer, "1    %d  %d", podio.J1.id, podio.J1.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t1 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t1, NULL, NULL, &r1.w, &r1.h);
        
        sprintf(buffer, "2    %d  %d", podio.J2.id, podio.J2.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t2 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t2, NULL, NULL, &r2.w, &r2.h);
        
        sprintf(buffer, "3    %d  %d", podio.J3.id, podio.J3.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t3 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t3, NULL, NULL, &r3.w, &r3.h);

        sprintf(buffer, "4    %d  %d", podio.J4.id, podio.J4.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t4 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t4, NULL, NULL, &r4.w, &r4.h);

        sprintf(buffer, "5    %d  %d", podio.J5.id, podio.J5.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t5 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t5, NULL, NULL, &r5.w, &r5.h);
        
        sprintf(buffer, "6    %d  %d", podio.J6.id, podio.J6.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t6 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t6, NULL, NULL, &r6.w, &r6.h);
        
        sprintf(buffer, "7    %d  %d", podio.J7.id, podio.J7.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t7 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t7, NULL, NULL, &r7.w, &r7.h);
        
        sprintf(buffer, "8    %d  %d", podio.J8.id, podio.J8.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t8 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t8, NULL, NULL, &r8.w, &r8.h);
        
        sprintf(buffer, "9    %d  %d", podio.J9.id, podio.J9.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t9 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t9, NULL, NULL, &r9.w, &r9.h);
        
        sprintf(buffer, "10   %d  %d", podio.J10.id, podio.J10.pts);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        t10 = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(t10, NULL, NULL, &r10.w, &r10.h);
        
        SDL_RenderClear(render);
        SDL_RenderCopy(render, textura_fundo_ranking, NULL, NULL);
        SDL_RenderCopy(render, Titulo_textura, NULL, &rTitulo);
        SDL_RenderCopy(render, t1, NULL, &r1);
        SDL_RenderCopy(render, t2, NULL, &r2);
        SDL_RenderCopy(render, t3, NULL, &r3);
        SDL_RenderCopy(render, t4, NULL, &r4);
        SDL_RenderCopy(render, t5, NULL, &r5);
        SDL_RenderCopy(render, t6, NULL, &r6);
        SDL_RenderCopy(render, t7, NULL, &r7);
        SDL_RenderCopy(render, t8, NULL, &r8);
        SDL_RenderCopy(render, t9, NULL, &r9);
        SDL_RenderCopy(render, t10, NULL, &r10);
        SDL_RenderPresent(render);
        SDL_Delay(1000/FPS);
    	}
}

void controles(void){
    while(1){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {   
            switch (event.type)
            {
            case SDL_KEYDOWN:
                return;
                break;
            }
        }
        SDL_RenderClear(render);
        SDL_RenderCopy(render, textura_fundo_como_jogar, NULL, NULL);
        SDL_RenderPresent(render);
        SDL_Delay(1000/FPS);
    }
}

void creditos(void){
    while(1){
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {   
            switch (event.type)
            {
            case SDL_KEYDOWN:
                return;
                break;
            }
        }
        SDL_RenderClear(render);
        SDL_RenderCopy(render, textura_fundo_creditos, NULL, NULL);
        SDL_RenderPresent(render);
        SDL_Delay(1000/FPS);
    }
}

void menu(void){
    int seletor = 0;
    SDL_Rect dest_caixa;

    // obter e dimensionar as dimensões da caixa de seleção
    SDL_QueryTexture(caixa, NULL, NULL, &dest_caixa.w, &dest_caixa.h);
    dest_caixa.w = 301;
    
    dest_caixa.x = 349;

    while(1){
            SDL_Event event;
        while (SDL_PollEvent(&event)){   
            switch (event.type){
            case SDL_QUIT:
                SDL_DestroyTexture(caixa);
                SDL_DestroyRenderer(render);
                SDL_DestroyWindow(janela);
                SDL_Quit();
                return;
                break;
            case SDL_KEYDOWN:
            	Mix_PlayChannel( -1, menu_som, 0 );
                switch (event.key.keysym.scancode){
                case SDL_SCANCODE_DOWN:
                    seletor++;
                    if(seletor > 3) seletor = 0;
                    break;
                case SDL_SCANCODE_UP:
                    seletor--;
                    if(seletor<0) seletor = 3;
                    break;
                case SDL_SCANCODE_ESCAPE:
                    SDL_DestroyTexture(caixa);
                    SDL_DestroyRenderer(render);
                    SDL_DestroyWindow(janela);
                    SDL_Quit();
                    return;
                    break;
                case SDL_SCANCODE_RETURN:
                    switch(seletor){
                        case 0://iniciar jogo
                            SDL_RenderClear(render);
                            jogo();
                            break;
                        case 1://ranking
                            ranking();
                            break;
                        case 2://como jogar
                            controles();
                            break;
                        case 3://creditos
                            creditos();
                            break;
                    }
                    break;
                }
                break;
            }
        }

        switch(seletor){
        case 0://iniciar jogo
        dest_caixa.y = 125;
            break;
        case 1://ranking
        dest_caixa.y = 250;          
            break;
        case 2://como jogar
        dest_caixa.y = 375;    
            break;
        case 3://creditos
        dest_caixa.y = 500;    
            break;
        }
        SDL_RenderClear(render);
        SDL_RenderCopy(render, textura_fundo_menu, NULL, NULL);
        SDL_RenderCopy(render, caixa, NULL, &dest_caixa);
        SDL_RenderPresent(render);

        SDL_Delay(1000/FPS);

    }
}

int main(void){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){    // tentativa de inicializar gráficos e sistema temporizador
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    janela = SDL_CreateWindow("Voyager 1977",SDL_WINDOWPOS_CENTERED,0,JANELA_W,JANELA_H,0);
    if (!janela){
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    render = SDL_CreateRenderer(janela, -1, 0);    // criar um renderizador, que configura o hardware gráfico
    if (!render){
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(janela);
        SDL_Quit();
        return 1;
    }
    
    TTF_Init();

    font = TTF_OpenFont("fonte.ttf", 25);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    musica = Mix_LoadMUS("Blue Space v0_96.wav");
    bonus_som = Mix_LoadWAV("Picked Coin Echo 2.wav");
    menu_som = Mix_LoadWAV("Menu Selection Click.wav");
    jogo_som = Mix_LoadWAV("8bit_bomb_explosion.wav");

    Mix_VolumeChunk(jogo_som, 48);
    Mix_VolumeChunk(bonus_som, 64);
    Mix_PlayMusic( musica, -1 );
    Mix_VolumeMusic(MIX_MAX_VOLUME/4);

    surface_menu = IMG_Load("caixa.png");//diretirio seletor
    if (!surface_menu){
        Erro(2);
        return 1;
    }

    background_creditos = IMG_Load("creditos.png");
    if (!background_creditos){
        Erro(2);
        return 1;
    }

    background_menu = IMG_Load("menu 2.png");
    if (!background_menu){
        Erro(2);
        return 1;
    }

    background_como_jogar = IMG_Load("como jogar.png");
    if (!background_como_jogar){
        Erro(2);
        return 1;
    }

    caixa = SDL_CreateTextureFromSurface(render, surface_menu);
    SDL_FreeSurface(surface_menu);
    if (!caixa){
        Erro(1);
        return 1;
    }

    textura_fundo_menu = SDL_CreateTextureFromSurface(render, background_menu);
    SDL_FreeSurface(background_menu);
    if (!textura_fundo_menu){
        Erro(1);
        return 1;
    }

    textura_fundo_creditos = SDL_CreateTextureFromSurface(render, background_creditos);
    SDL_FreeSurface(background_creditos);
    if (!textura_fundo_creditos){
        Erro(1);
        return 1;
    }

    textura_fundo_como_jogar = SDL_CreateTextureFromSurface(render, background_como_jogar);
    SDL_FreeSurface(background_como_jogar);
    if (!textura_fundo_como_jogar){
        Erro(1);
        return 1;
    }


    background_ranking = IMG_Load("ranking.png");
    if (!background_ranking){
        Erro(2);
        return 1;
    }

    textura_fundo_ranking = SDL_CreateTextureFromSurface(render, background_ranking);
    SDL_FreeSurface(background_ranking);
    if (!textura_fundo_ranking){
        Erro(1);
        return 1;
    }

    background_jogo = IMG_Load("fundo jogo.png");
    if (!background_jogo){
        Erro(2);
        return 1;
    }

    textura_fundo_jogo = SDL_CreateTextureFromSurface(render, background_jogo);
    SDL_FreeSurface(background_jogo);
    if (!textura_fundo_jogo){
        Erro(1);
        return 1;
    }

    surface_nave = IMG_Load("voyager.png");
    if (!surface_nave){
        Erro(2);
        return 1;
    }

    nave = SDL_CreateTextureFromSurface(render, surface_nave);
    SDL_FreeSurface(surface_nave);
    if (!nave){
        Erro(1);
        return 1;
    }

    surface_asteroide = IMG_Load("asteroide.png");
    if (!surface_asteroide){
        Erro(2);
        return 1;
    }

    asteroide = SDL_CreateTextureFromSurface(render, surface_asteroide);
    SDL_FreeSurface(surface_asteroide);
    if (!asteroide){
        Erro(1);
        return 1;
    }

    surface_asteroide2 = IMG_Load("asteroide 2.png");
    if (!surface_asteroide2){
        Erro(2);
        return 1;
    }

    asteroide2 = SDL_CreateTextureFromSurface(render, surface_asteroide2);
    SDL_FreeSurface(surface_asteroide2);
    if (!asteroide2){
        Erro(1);
        return 1;
    }

    surface_planeta = IMG_Load("planeta.png");
    if (!surface_planeta){
        Erro(2);
        return 1;
    }

    planeta = SDL_CreateTextureFromSurface(render, surface_planeta);
    SDL_FreeSurface(surface_planeta);
    if (!planeta){
        Erro(1);
        return 1;
    }

    surface_Golden_Record = IMG_Load("golden record.png");
    if (!surface_Golden_Record){
        Erro(2);
        return 1;
    }

    golden_record = SDL_CreateTextureFromSurface(render, surface_Golden_Record);
    SDL_FreeSurface(surface_Golden_Record);
    if (!golden_record){
        Erro(1);
        return 1;
    }

    surface_coracao = IMG_Load("coracao.png");
    if (!surface_coracao){
        Erro(2);
        return 1;
    }

    coracao_1 = SDL_CreateTextureFromSurface(render, surface_coracao);
    SDL_FreeSurface(surface_coracao);
    if (!coracao_1){
        Erro(1);
        return 1;
    }

    coracao_2 = coracao_1;
    coracao_3 = coracao_1;
    coracao_4 = coracao_1;
    coracao_5 = coracao_1;

    //recolhe os dados do arquivo e insere na variavel global podio
    FILE * pont_arq;
    pont_arq = fopen( "registro.bin", "rb");//leitura do arquivo
    if( !pont_arq )	printf("arquivo não aberto\n");
    fread( &podio, sizeof(RankingType), 1, pont_arq );
    fclose(pont_arq);
    
    menu();

    pont_arq = fopen( "registro.bin", "wb");//Escrita do arquivo
    if( !pont_arq )	printf("arquivo não salvo\n");
    fwrite( &podio, sizeof(RankingType), 1, pont_arq );
    fclose(pont_arq);

    Mix_FreeChunk( bonus_som );
    Mix_FreeChunk( menu_som );
    Mix_FreeChunk( jogo_som );
    menu_som = NULL;
    jogo_som = NULL;
    bonus_som = NULL;
    Mix_HaltMusic();
    Mix_FreeMusic( musica );
    musica = NULL;
    Mix_Quit();

    return 0;
}