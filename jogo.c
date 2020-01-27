
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
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>

#define JANELA_W (700)
#define JANELA_H (700)

#define FPS (30)

SDL_Window* janela = NULL;
SDL_Renderer* render = NULL;
SDL_Surface* surface = NULL;

SDL_Surface* background_menu = NULL;
SDL_Texture* textura_fundo_menu = NULL;
SDL_Texture* caixa = NULL;
SDL_Surface* surface_menu = NULL;

SDL_Surface* background_jogo = NULL;
SDL_Texture* textura_fundo_jogo = NULL;

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
        SDL_RenderCopy(render, textura_fundo_jogo, NULL, NULL);
        SDL_RenderPresent(render);
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

    menu();
    return 0;
}