
// gcc jogo.c -o jogo -lSDL2 -lSDL2_image -lSDL2_ttf

/*
tabela de erros:
    1 erro de criação de textura
    2 erro de criação de surface
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define JANELA_W (700)
#define JANELA_H (700)

#define FPS (30)	//quadros por segundo
#define SetBonus (15) //a cada quantos segundos vem o bonus
#define VIDAS (5)
#define Velocidade_Inicial (13)

SDL_Window* janela = NULL;
SDL_Renderer* render = NULL;
SDL_Surface* surface = NULL;

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
    int id1, id2, id3;
    int pts1, pts2, pts3;
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

void jogo(void){
	int identificacao = podio.id_final += 1;
	// tamanho e posição da nave
    SDL_Rect dest, rGolden, rPlaneta, rAsteroide, rC1, rC2, rC3, rC4, rC5;

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
    bool left = false, right = false, bonus, impacto = false;

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

        //relogio
        tempo_ant = tempo;
        tempo = SDL_GetTicks()/1000;
        if(tempo > tempo_ant)   printf("%d pts  %d Id\n", pontos, identificacao);

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
        rPlaneta.y += SPEED+10;

        if(rAsteroide.y > 800){
            pontos += 10;
            rAsteroide.x = rand() % 450;
            rAsteroide.y = -rAsteroide.h;
        }
        if(rPlaneta.y > 850){
            pontos += 20;
            rPlaneta.x = rand() % 350;
            rPlaneta.y = -rPlaneta.h-100;
        }

        //impacto com elementos
        if(SDL_HasIntersection(&dest, &rAsteroide)){
            --vidas;
            pontos -= 20;
            rAsteroide.y = -rAsteroide.h;
            rAsteroide.x = rand() % 450;
        }
        if(SDL_HasIntersection(&dest, &rPlaneta)){
            --vidas;
            pontos -= 30;
            rPlaneta.y = -rPlaneta.h;
            rPlaneta.x = rand() % 350;
        }

        //bonus
        bonus = SDL_TICKS_PASSED(SDL_GetTicks(), tempo_bonus);

        if(SDL_HasIntersection(&dest, &rGolden)){
            SPEED += 5;
            pontos += 500;
            rGolden.y = 800;
            printf("bonus\n");
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
    if(pontos >= podio.pts1){
		podio.pts3 = podio.pts2;
		podio.id3 = podio.id2;
		podio.pts2 = podio.pts1;
		podio.id2 = podio.id1;
		podio.pts1 = pontos;
		podio.id1 = identificacao;
	}
	else if(pontos >= podio.pts2 && pontos < podio.pts1){
	   		podio.pts3 = podio.pts2;
	    	podio.id3 = podio.id2;
	    	podio.pts2 = pontos;
	    	podio.id2 = identificacao;
	}
	else if(pontos >= podio.pts3 && pontos < podio.pts2 && pontos < podio.pts1){
	   		podio.pts3 = pontos;
	    	podio.id3 = identificacao;
	}
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

        SDL_Texture* primeiro_textura = NULL;
        SDL_Rect rPrimeiro;
        rPrimeiro.h = 0;
        rPrimeiro.w = 0;
        rPrimeiro.x = 180;
        rPrimeiro.y = 200;

        SDL_Texture* segundo_textura = NULL;
        SDL_Rect rSegundo;
        rSegundo.h = 0;
        rSegundo.w = 0;
        rSegundo.x = 180;
        rSegundo.y = 225;

        SDL_Texture* terceiro_textura = NULL;
        SDL_Rect rTerceiro;
        rTerceiro.h = 0;
        rTerceiro.w = 0;
        rTerceiro.x = 180;
        rTerceiro.y = 250;

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
        
        sprintf(buffer, "1    %d  %d", podio.id1, podio.pts1);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        primeiro_textura = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(primeiro_textura, NULL, NULL, &rPrimeiro.w, &rPrimeiro.h);
        
        sprintf(buffer, "2    %d  %d", podio.id2, podio.pts2);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        segundo_textura = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(segundo_textura, NULL, NULL, &rSegundo.w, &rSegundo.h);
        
        sprintf(buffer, "3    %d  %d", podio.id3, podio.pts3);
        Superficie = TTF_RenderText_Solid(font, buffer, branco);
        terceiro_textura = SDL_CreateTextureFromSurface(render, Superficie);
        SDL_QueryTexture(terceiro_textura, NULL, NULL, &rTerceiro.w, &rTerceiro.h);
        
        SDL_RenderClear(render);
        SDL_RenderCopy(render, textura_fundo_ranking, NULL, NULL);
        SDL_RenderCopy(render, Titulo_textura, NULL, &rTitulo);
        SDL_RenderCopy(render, primeiro_textura, NULL, &rPrimeiro);
        SDL_RenderCopy(render, segundo_textura, NULL, &rSegundo);
        SDL_RenderCopy(render, terceiro_textura, NULL, &rTerceiro);
        SDL_RenderPresent(render);
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
    }
}

void menu(void){
    int seletor = 0;
    SDL_Rect dest_caixa;

    // obter e dimensionar as dimensões do jogador
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

    //recolhe os dados do arquivo e insere na variavel global Podio
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


    return 0;
}