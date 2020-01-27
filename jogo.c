
// gcc jogoteste.c -o jogo -lSDL2 -lSDL2_image

/*
tabela de erros:
    1 erro de criação de textura
    2 erro de criação de surface

tela inicial
    nome do jogo
    menu
        jogar
            chamar função que inicia o jogo
        como jogar
            abrir imagem com os controles do jogo
        ranking
            ler arquivo de ranking das partidas
        creditos
            nome dos participantes
fim menu

afazeres
    criar documento para armazenar e ler o ranking
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define JANELA_W (700)
#define JANELA_H (700)

#define FPS (30)	//quadros por segundo
#define SetBonus (10) //a cada quantos segundos vem o bonus
#define VIDAS (5)
#define Velocidade_Inicial (13)

SDL_Window* janela = NULL;
SDL_Renderer* render = NULL;
SDL_Surface* surface = NULL;

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


SDL_Surface* background_creditos = NULL;
SDL_Texture* textura_fundo_creditos = NULL;

SDL_Surface* background_como_jogar = NULL;
SDL_Texture* textura_fundo_como_jogar = NULL;

SDL_Surface* background_ranking = NULL;
SDL_Texture* textura_fundo_ranking = NULL;

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
	// tamanho e posição da nave
    SDL_Rect dest;
    SDL_Rect rGolden;
    SDL_Rect rPlaneta;
    SDL_Rect rAsteroide;

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
                encerrar = 1;
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
        if(tempo > tempo_ant)   printf("%d segundos   %d pontos   %d Vidas\n", tempo, pontos, vidas);

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
            rPlaneta.y = -rPlaneta.h;
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
        SDL_RenderClear(render);
        SDL_RenderCopy(render, textura_fundo_jogo, NULL, NULL);
        SDL_RenderCopy(render, asteroide, NULL, &rAsteroide);
        SDL_RenderCopy(render, planeta, NULL, &rPlaneta);
        SDL_RenderCopy(render, nave, NULL, &dest);
        SDL_RenderCopy(render, golden_record, NULL, &rGolden);
        SDL_RenderPresent(render);

        SDL_Delay(1000/FPS);
    }
}

void ranking(void){
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
        SDL_RenderCopy(render, textura_fundo_ranking, NULL, NULL);
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

    janela = SDL_CreateWindow("RURALINO",SDL_WINDOWPOS_CENTERED,0,JANELA_W,JANELA_H,0);
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


    menu();
    return 0;
}