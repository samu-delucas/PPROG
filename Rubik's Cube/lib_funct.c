#include "lib_funct.h"
#include "cube.h"
#include "print_c.h"
#include "solver.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>

#ifndef FONT_TTF
#define FONT_TTF "./fonts/FreeMonoOblique.ttf"
#endif

#define MAX_CAD 500

#define NUM_CAD 72

char **separarCadenas(char *cad)
{

    char *texto = (char *)calloc(81,1);
    char *texto2 = (char *)calloc(81, 1);
    char *texto3 = (char *)calloc(81, 1);
    char **textos = (char **)calloc(3, sizeof(char *));
    int i, j, len;
    len = strlen(cad);
    if (len <= NUM_CAD)
    {
        for (i = 0; i < len; i++)
        {
            texto[i] = cad[i];
        }
    }
    else if (len <= 2*NUM_CAD)
    {
        for (i = 0; i < NUM_CAD; i++)
        {
            texto[i] = cad[i];
        }
        texto[i]='\0';
        for (j = 0; i < len; i++, j++)
        {
            texto2[j] = cad[i];
        }
        texto2[j] = '\0';
        textos[1] = texto2;
    }
    else
    {
        for (i = 0; i < NUM_CAD; i++)
        {
            texto[i] = cad[i];
        }
        texto[i]='\0';
        for (j = 0; i < 2*NUM_CAD; i++, j++)
        {
            texto2[j] = cad[i];
        }
        texto2[j]='\0';
        for (j = 0; i < len; i++, j++)
        {
            texto3[j] = cad[i];
        }
        texto3[j] = '\0';
        textos[1] = texto2;
        textos[2] = texto3;
    }
    textos[0] = texto;

    textos[1] = texto2;
    textos[2] = texto3;

    return textos;
}

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect)
{
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    int ancho, alto;

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);

    ancho = surface->w;
    alto = surface->h;
    rect->x = x;
    rect->y = y;

    rect->w = ancho;
    rect->h = alto;

    SDL_FreeSurface(surface);
}

void SDL_DisplayText(SDL_Renderer *renderer, char *text, TTF_Font *font)
{
    SDL_Rect rect1, rect2, rect3;
    SDL_Texture *texture1, *texture2, *texture3;
    int len_txt;
    char **cadenas;
    len_txt = strlen(text);
    /*if (len_txt <= NUM_CAD)
    {
        get_text_and_rect(renderer, 0, 0, text, font, &texture1, &rect1);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderPresent(renderer);
    }
    else if (len_txt <= NUM_CAD)
    {
        cadenas = separarCadenas(text);
        get_text_and_rect(renderer, 0, 0, cadenas[0], font, &texture1, &rect1);
        get_text_and_rect(renderer, 0, rect1.y + rect1.h, cadenas[1], font, &texture2, &rect2);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);

        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture2);
        free(cadenas[0]);
        free(cadenas[1]);
        free(cadenas);
    }
    else*/
    {
        cadenas = separarCadenas(text);
        get_text_and_rect(renderer, 0, 0, cadenas[0], font, &texture1, &rect1);
        if(len_txt > NUM_CAD)
            get_text_and_rect(renderer, 0, rect1.y + rect1.h, cadenas[1], font, &texture2, &rect2);
        if(len_txt > 2 * NUM_CAD)
            get_text_and_rect(renderer, 0, rect2.y + rect2.h, cadenas[2], font, &texture3, &rect3);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        if(len_txt > NUM_CAD)
            SDL_RenderCopy(renderer, texture2, NULL, &rect2);
        if(len_txt > 2 * NUM_CAD)
            SDL_RenderCopy(renderer, texture3, NULL, &rect3);

        free(cadenas[0]);
        free(cadenas[1]);
        free(cadenas[2]);
        free(cadenas);

        SDL_RenderPresent(renderer);
        if(len_txt > NUM_CAD)
            SDL_DestroyTexture(texture2);
        if(len_txt > 2 * NUM_CAD)
            SDL_DestroyTexture(texture3);
    }

    SDL_DestroyTexture(texture1);
    return;
}

void Render(double **s, int option)
{

    sCube *sc = NULL;
    int i = 0;

    if ((sc = sCube_init()) == NULL)
        return;

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_QUADS);

    /* "paint" all pieces */
    for (i = 0; i < NPC; i++)
        paint_sPiece(&(sc->pc[i]), option, s);
    free_sCube(sc);

    glEnd();
}

void Render_wr(int *w, int *h, double **stickers, SDL_Window *_window, SDL_GLContext _ctx, int option)
{
    SDL_GL_MakeCurrent(_window, _ctx);
    SDL_GetWindowSize(_window, w, h);
    glViewport(0, 0, *w, *h);
    Render(stickers, option);
    SDL_GL_SwapWindow(_window);
}

void Rot_layer(float _angle, double **s, char c, int option)
{

    sCube *sc = NULL;
    int i = 0;

    if ((sc = sCube_init()) == NULL)
        return;

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.0, 1.0, 1.0, 0, 0, 0, 0.5, 0, 0);

    glBegin(GL_QUADS);

    /* "paint" all pieces */
    for (i = 0; i < NPC; i++)
    {
        if (is_in_face(i, c) == 0)
        {
            paint_sPiece(&(sc->pc[i]), option, s);
        }
    }
    glEnd();
    glPushMatrix();

    /*set up the angles and axis of rotation*/
    if (c == 'L' || c == 'M' || c == 'r')
        glRotatef(_angle, 0.0f, 0.0f, 1.0f);
    else if (c == 'R' || c == 'l' || c == 'm')
        glRotatef(-_angle, 0.0f, 0.0f, 1.0f);
    else if (c == 'E' || c == 'D' || c == 'u')
        glRotatef(_angle, 1.0f, 0.0f, 0.0f);
    else if (c == 'U' || c == 'e' || c == 'd')
        glRotatef(-_angle, 1.0f, 0.0f, 0.0f);
    else if (c == 'F' || c == 'S' || c == 'b')
        glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
    else if (c == 'B' || c == 'f' || c == 's')
        glRotatef(_angle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
    for (i = 0; i < NPC; i++)
    {
        if (is_in_face(i, c) == 1)
        {
            paint_sPiece(&(sc->pc[i]), option, s);
        }
    }
    glPopMatrix();
    glEnd();

    free_sCube(sc);
}

void Rot_cube(float _angle, double **s, char c, int option)
{

    sCube *sc = NULL;
    int i = 0;

    if ((sc = sCube_init()) == NULL)
        return;

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.0, 1.0, 1.0, 0, 0, 0, 0.5, 0, 0);

    glPushMatrix();
    if (c == 'x')
    {
        glRotatef(_angle, 0.0f, 0.0f, 1.0f);
    }
    else if (c == 'X')
    {
        glRotatef(-_angle, 0.0f, 0.0f, 1.0f);
    }
    else if (c == 'y')
    {
        glRotatef(_angle, 1.0f, 0.0f, 0.0f);
    }
    else if (c == 'Y')
    {
        glRotatef(-_angle, 1.0f, 0.0f, 0.0f);
    }
    else if (c == 'z')
    {
        glRotatef(_angle, 0.0f, 1.0f, 0.0f);
    }
    else if (c == 'Z')
    {
        glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
    }

    glBegin(GL_QUADS);

    /* "paint" all pieces */
    for (i = 0; i < NPC; i++)
    {
        paint_sPiece(&(sc->pc[i]), option, s);
    }

    glEnd();
    glPopMatrix();

    free_sCube(sc);
}

void Rot(float _angle, double **s, char c, int option)
{
    if (option == 2)
    {
        if (c == 'R' || c == 'L' || c == 'U' || c == 'D' || c == 'F' || c == 'B' || c == 'r' || c == 'l' || c == 'u' || c == 'd' || c == 'f' || c == 'b')
        {
            Rot_layer(_angle, s, c, option);
        }
    }
    else if (c == 'R' || c == 'L' || c == 'M' || c == 'E' || c == 'U' || c == 'D' || c == 'F' || c == 'B' || c == 'S' || c == 'r' || c == 'l' || c == 'm' || c == 'e' || c == 'u' || c == 'd' || c == 'f' || c == 'b' || c == 's')
    {
        Rot_layer(_angle, s, c, option);
    }

    if (c == 'x' || c == 'X' || c == 'y' || c == 'Y' || c == 'z' || c == 'Z')
    {
        Rot_cube(_angle, s, c, option);
    }
}

void Rot_full_move(int *w, int *h, double **stickers, char move, SDL_Window *_window, SDL_GLContext _ctx, int option)
{
    float _angle = 0.0f;

    glPushMatrix();

    SDL_GL_MakeCurrent(_window, _ctx);
    SDL_GetWindowSize(_window, w, h);
    glViewport(0, 0, *w, *h);

    while (_angle <= 90.0f)
    {

        Rot(_angle, stickers, move, option);
        SDL_GL_SwapWindow(_window);
        SDL_Delay(10);
        _angle += 4.0f;
    }
    glPopMatrix();
}

void SlowMoveRot(Cube3 *c, int *w, int *h, double **stickers, char *moves, SDL_Window *_window, SDL_GLContext _ctx, int option)
{
    int i = 0, len = strlen(moves);

    for (i = 0; i < len; i++)
    {
        /*display rotation*/
        Rot_full_move(w, h, stickers, moves[i], _window, _ctx, option);
        /*make move in the cube*/
        c_make(c, moves[i]);
        /*update rgb matrix*/
        cube_to_SDL(c, stickers);
    }
    /*render changes in the cube*/
    Render_wr(w, h, stickers, _window, _ctx, option);
}

void SDL_DisplayTextWRAPPER(SDL_Window **_window2, char *solution, SDL_Renderer **_renderer, TTF_Font *_font)
{
    if ((*_renderer) != NULL)
        SDL_DestroyRenderer(*_renderer);
    if ((*_window2) != NULL)
        SDL_DestroyWindow(*_window2);

    (*_window2) = SDL_CreateWindow("Rubik Cube PPROG", 410, 220, 900, 80, SDL_WINDOW_BORDERLESS);

    if (!(*_window2))
    {
        printf("SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    _font = TTF_OpenFont(FONT_TTF, 20);

    (*_renderer) = SDL_CreateRenderer(*_window2, -1, SDL_RENDERER_ACCELERATED);

    SDL_DisplayText(*_renderer, solution, _font);

    TTF_CloseFont(_font);
}