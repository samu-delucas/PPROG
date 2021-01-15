/**
 * @file    body.c
 * @author  Bhavuk Sikka
 * 
 * @brief   This module handles the drawing of the bees
 * */

#include "body.h"
#define BUF 25

pthread_mutex_t beemutex;

struct body
{
    short tall, fat, angry, hair;
};

status create_body(body **b)
{
    if (!b)
        return ERROR;
    if (!(*b = (body *)malloc(sizeof(body))))
        return ERROR;
    (*b)->fat=0;
    (*b)->angry=0;
    (*b)->tall=0;
    (*b)->hair=0;
    return OK;
}

status set_body(body *b, const short tall, const short fat, const short angry, const short hair)
{
    if (!b || tall < 0 || fat < 0 || angry < 0 || hair < 0)
        return ERROR;
    if (angry > 2)
        b->angry = 2;
    else
        b->angry = angry;
    if (fat > 2)
        b->fat = 2;
    else
        b->fat = fat;
    if (hair > 20)
        b->hair = 20;
    else
        b->hair = hair;
    if (tall > 2)
        b->tall = 2;
    else
        b->tall = tall;
    return OK;
}

short get_body_tall(body *b)
{
    if (!b)
        return -1;
    return b->tall;
}

short get_body_fat(body *b)
{
    if (!b)
        return -1;
    return b->fat;
}

short get_body_angry(body *b)
{
    if (!b)
        return -1;
    return b->angry;
}

short get_body_hair(body *b)
{
    if (!b)
        return -1;
    return b->hair;
}

void free_body(body **b)
{
    if (!b)
        return;
    if (*b)
    {
        free(*b);
        *b = NULL;
    }
}

int res(int a, int b)
{
    if (a == b)
        return b;
    if(a==-1 || b==-1){
        return -1;
    }
    switch (a)
    {
    case 0:
        return b;
        break;
    case 1:
        switch (b)
        {
        case 2:
        case 5:
        case 10:
            return 10;
        case 3:
        case 6:
        case 8:
            return 8;
        case 4:
        case 7:
        case 9:
        case 11:
            return 11;
        default:
            return b;
        }
    case 2:
        switch (b)
        {
        case 1:
        case 5:
        case 10:
            return 10;
        case 3:
        case 8:
        case 9:
        case 11:
            return 11;
        case 4:
        case 6:
        case 7:
            return 7;
        default:
            return b;
        }
    case 3:
        switch (b)
        {
        case 1:
        case 6:
        case 8:
            return 8;
        case 2:
        case 7:
        case 10:
        case 11:
            return 11;
        case 4:
        case 5:
        case 9:
            return 9;
        default:
            return b;
        }
    case 4:
        switch (b)
        {
        case 1:
        case 8:
        case 10:
        case 11:
            return 11;
        case 2:
        case 6:
        case 7:
            return 7;
        case 3:
        case 5:
        case 9:
            return 9;
        default:
            return b;
        }
    case 5:
        switch (b)
        {
        case 1:
        case 2:
        case 10:
            return 10;
        case 3:
        case 4:
        case 9:
            return 9;
        case 6:
        case 7:
        case 8:
        case 11:
            return 11;
        default:
            return b;
        }
    case 6:
        switch (b)
        {
        case 1:
        case 3:
        case 8:
            return 8;
        case 2:
        case 4:
        case 7:
            return 7;
        case 5:
        case 9:
        case 10:
        case 11:
            return 11;
        default:
            return b;
        }
    case 7:
        switch (b)
        {
        case 1:
        case 3:
        case 5:
        case 8:
        case 9:
        case 10:
        case 11:
            return 11;
        default:
            return 7;
        }
    case 8:
        switch (b)
        {
        case 2:
        case 4:
        case 5:
        case 7:
        case 9:
        case 10:
        case 11:
            return 11;
        default:
            return 8;
        }
    case 9:
        switch (b)
        {
        case 1:
        case 2:
        case 6:
        case 7:
        case 8:
        case 10:
        case 11:
            return 11;
        default:
            return 9;
        }
    case 10:
        switch (b)
        {
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 11:
            return 11;
        default:
            return 10;
        }
    default:
        return 11;
    }
    return 11;
}

static char **hairr(int hair, int *a)
{
    char ch[20];
    char **s = NULL;
    FILE *f;
    int i, j;
    sprintf(ch, "quests/hair/%d.txt", hair);
    if (!(f = fopen(ch, "r")))
    {
        return NULL;
    }
    fscanf(f, "%d", a);
    if (!(s = (char **)malloc((*a) * sizeof(char *))))
    {
        fclose(f);
        return NULL;
    }
    for (i = 0; i < (*a); i++)
    {
        if (!((s[i]) = (char *)malloc(1 + BUF)))
        {
            for (j = 0; j < i; j++)
            {
                free(s[j]);
            }
            free(s);
            fclose(f);
            return NULL;
        }
    }

    for (i = 0; i < (*a); i++)
    {
        fscanf(f, "%s", s[i]);
    }

    fclose(f);
    return s;
}

static void draw_hair(pts *p, section *sec, kolor *k, int hair, int startrow, int startcol, int endrow, int endcol)
{
    int i, ran, a, bb, j = 0, aux, ic = 0, raices = 0, rran;
    char **h;
    char ch[][4] = {"┓", "┏", "┛", "┗", "━", "┃", "┣", "┫", "┻", "┳", "╋"};
    char **s;
    short flag = 1;
    pts *p1;
    if (!(s = (char **)calloc(get_section_row(sec), sizeof(char *))))
    {
        return;
    }
    for (i = 0; i < get_section_row(sec); i++)
    {
        if (!(s[i] = (char *)calloc(get_section_col(sec), sizeof(char))))
        {
            for (j = 0; j < i; j++)
            {
                free(s[j]);
            }
            free(s);
            return;
        }
    }
    if (!(h = hairr(hair, &raices)))
    {
        free_point(&p1);
        for (j = 0; j < get_section_row(sec); j++)
        {
            free(s[j]);
        }
        free(s);
        return;
    }
    if (create_point(&p1) == ERROR)
    {
        free_point(&p1);
        for (j = 0; j < get_section_row(sec); j++)
        {
            free(s[j]);
        }
        free(s);
        for (j = 0; j < raices; j++)
        {
            free(h[j]);
        }
        free(h);
        return;
    }
    for (i = 0; i < raices; i++)
    {
        j = 0;
        flag = 1;
        ran = 2;
        aux = (endcol - startcol - 1) * (raices - i) / (raices + 1) + startcol;
        a = startrow - get_point_row(p);
        bb = aux - get_point_col(p) + 1;
        while (flag && h[i][j])
        {
            rran = (int)(h[i][j] - '0');
            switch (rran)
            {
            case 0:
                switch (ran)
                {
                case 1:
                    ic = 2;
                    bb--;
                    break;
                case 0:
                    ic = 6;
                    a++;
                    break;
                case 3:
                    ic = 1;
                    bb++;
                    break;
                default:
                    break;
                }
                ran = 0;
                break;
            case 1:
                switch (ran)
                {
                case 0:
                    ic = 3;
                    a++;
                    break;
                case 1:
                    ic = 5;
                    bb--;
                    break;
                case 2:
                    ic = 1;
                    a--;
                    break;
                default:
                    break;
                }
                ran = 1;
                break;
            case 2:
                switch (ran)
                {
                case 1:
                    ic = 4;
                    bb--;
                    break;
                case 2:
                    ic = 6;
                    a--;
                    break;
                case 3:
                    ic = 3;
                    bb++;
                    break;
                default:
                    break;
                }
                ran = 2;
                break;
            default:
                switch (ran)
                {
                case 0:
                    ic = 4;
                    a++;
                    break;
                case 3:
                    ic = 5;
                    bb++;
                    break;
                case 2:
                    ic = 2;
                    a--;
                    break;
                default:
                    break;
                }
                ran = 3;
                break;
            }
            if (a < 0)
            {
                a++;
                flag = 0;
                ran = 2;
                ic = 10;
            }
            else if (bb < 0)
            {
                bb++;
                flag = 0;
                ran = 1;
                ic = 7;
            }
            else if (a > get_section_row(sec))
            {
                a--;
                flag = 0;
                ran = 0;
                ic = 9;
            }
            else if (bb > get_section_col(sec))
            {
                bb--;
                flag = 0;
                ran = 3;
                ic = 8;
            }
            else
                s[a][bb] = res((int)s[a][bb], ic);
            j++;
        }
        switch (ran)
        {
        case 0:
            a++;
            ic = 9;
            break;
        case 1:
            bb--;
            ic = 7;
            break;
        case 2:
            a--;
            ic = 10;
            break;
        default:
            bb++;
            ic = 8;
            break;
        }
        if (a < 0)
            a = 0;
        else if (bb < 0)
            bb = 0;
        else if (a >= get_section_row(sec))
            a = get_section_row(sec) - 1;
        else if (bb >= get_section_col(sec))
            bb = get_section_col(sec) - 1;
        else
            s[a][bb] = res((int)s[a][bb], ic);
    }

    for (i = 0; i < get_section_row(sec); i++)
    {
        for (j = 0; j < get_section_col(sec); j++)
        {
            if (s[i][j] != 0)
            {
                set_point(p1, i + get_point_row(p), j + get_point_col(p));
                draw_line(p1, p1, ch[s[i][j] - 1], sec, k, 0);
            }
        }
    }
    fflush(stdout);
    free_point(&p1);
    for (j = 0; j < get_section_row(sec); j++)
    {
        free(s[j]);
    }
    free(s);
    for (j = 0; j < raices; j++)
    {
        free(h[j]);
    }
    free(h);
}

static void draw_body(section *sec, pts *p1, pts *p2, int startrow, int startcol, int endrow, int endcol)
{
    int i;
    if (endrow > 6 * get_section_row(sec) / 7)
        endrow--;
    set_point(p1, startrow, startcol + 1);
    set_point(p2, startrow, endcol - 1);
    draw_line(p1, p2, ("▄"), sec, NULL, 0);
    set_point(p1, startrow, startcol + 2);
    set_point(p2, startrow, endcol - 2);
    draw_line(p2, p1, ("▀"), sec, NULL, 0);

    set_point(p1, startrow + 1, startcol);
    set_point(p2, endrow - 1, startcol);
    draw_line(p2, p1, ("▐"), sec, NULL, 0);

    set_point(p1, startrow + 1, endcol);
    set_point(p2, endrow - 1, endcol);
    draw_line(p2, p1, ("▌"), sec, NULL, 0);

    set_point(p1, endrow, startcol + 1);
    set_point(p2, endrow, endcol - 1);
    draw_line(p1, p2, ("▀"), sec, NULL, 0);
    set_point(p1, endrow, startcol + 2);
    set_point(p2, endrow, endcol - 2);
    draw_line(p1, p2, ("▄"), sec, NULL, 0);

    for (i = (endrow - startrow) * 7 / 18; i < endrow - startrow; i += 2)
    {
        set_point(p1, i + startrow, startcol + 1);
        set_point(p2, i + startrow, endcol - 1);
        draw_line(p1, p2, ("█"), sec, NULL, 0);
        set_point(p1, i + startrow, startcol + 2);
        set_point(p2, i + startrow, endcol - 2);
        draw_line(p1, p2, ("▄"), sec, NULL, 0);
        set_point(p1, i + 1 + startrow, startcol + 1);
        set_point(p2, i + 1 + startrow, endcol - 1);
        draw_line(p1, p2, (" "), sec, NULL, 0);
        set_point(p1, i + 1 + startrow, startcol + 2);
        set_point(p2, i + 1 + startrow, endcol - 2);
        draw_line(p1, p2, ("▀"), sec, NULL, 0);
    }

    set_point(p1, i + startrow + 1 - 2 * (i > endrow - startrow), startcol + 5 * (endcol - startcol) / 11);
    draw_line(p1, p1, "▜██▛", sec, NULL, 0);
    set_point(p1, get_point_row(p1) + 1, get_point_col(p1) + 1);
    draw_line(p1, p1, "▜▛", sec, NULL, 0);
}

static void draw_face(section *sec, kolor *k, pts *p, short angry, short tall, short fat)
{
    int i, j, row, col;
    pts *p1 = NULL, *p2 = NULL, *p3 = NULL;
    char ch[16];
    if (create_point(&p1) == ERROR || create_point(&p2) == ERROR || create_point(&p3) == ERROR)
    {
        free_point(&p1);
        free_point(&p2);
        free_point(&p3);
        return;
    }
    row = get_section_row(sec);
    col = get_section_col(sec);
    if (angry == 2)
    {
        i = 1;
        do
        {
            set_point(p1, get_point_row(p) + i, get_point_col(p) + i + 1);
            set_point(p2, get_point_row(p) + i, get_point_col(p) + i + 1 + col / 4);
            draw_line(p1, p2, "▚", sec, k, 0);
            i++;
        } while (i < row - 2 + (tall > 0) && i < col / 6);
        i = 1;
        do
        {
            set_point(p1, get_point_row(p) + i, get_point_col(p) - 1 - i + col - 1);
            set_point(p2, get_point_row(p) + i, get_point_col(p) - 2 - i + col - col / 4);
            draw_line(p1, p2, "▞", sec, k, 0);
            i++;
        } while (i < row - 2 + (tall > 0) && i < col / 6);
        if(row>5){
            if (col % 2)
                strcpy(ch, "┣┳━┳┫");
            else
                strcpy(ch, "┣┳┳┫");
            set_point(p1, get_point_row(p) + i + 1, get_point_col(p) + col / 2 - 2);
            draw_line(p1, p1, ch, sec, NULL, 0);
            if (col % 2)
                strcpy(ch, "┗   ┛");
            else
                strcpy(ch, "┗  ┛");
            set_point(p1, get_point_row(p1) + 1, get_point_col(p1));
            draw_line(p1, p1, ch, sec, NULL, 0);
        }else{
            strcpy(ch, "┏━┓");
            set_point(p1, get_point_row(p)+row-1, get_point_col(p) + col / 2-1);
            draw_line(p1, p1, ch, sec, NULL, 0);
        }
    }
    else if (angry == 1)
    {
        i = 1;
        set_point(p1, get_point_row(p), get_point_col(p) + 1 + (col > 13));
        set_point(p2, get_point_row(p), get_point_col(p) + 1 + 3 * col / 11);
        do
        {
            set_point(p1, get_point_row(p1) + 1, get_point_col(p1));
            set_point(p2, get_point_row(p2) + 1, get_point_col(p2) + 1 * (2 * (i < (row - 2) / 2) - 1));
            draw_line(p1, p2, "▚", sec, k, 0);
            i++;
        } while (i < (row - 2) / 2 && i < col / 7);
        do
        {
            set_point(p1, get_point_row(p1) + 1, get_point_col(p1));
            set_point(p2, get_point_row(p2) + 1, get_point_col(p2) - 1);
            set_point(p3, get_point_row(p2), get_point_col(p2) + 1);
            draw_line(p1, p2, "▚", sec, k, 0);
            draw_line(p3, p3, "▘", sec, k, 0);
            i++;
        } while (i < row - 2 && i < col / (6 - tall + fat));
        set_point(p2, get_point_row(p) + 1, get_point_col(p) + 1 + (col > 13));
        draw_line(p1, p2, "▙", sec, k, 0);

        i = 1;
        set_point(p1, get_point_row(p), get_point_col(p) - 2 + col - (col > 13));
        set_point(p2, get_point_row(p), get_point_col(p) - 2 + col - 3 * col / 11);
        do
        {
            set_point(p1, get_point_row(p1) + 1, get_point_col(p1));
            set_point(p2, get_point_row(p2) + 1, get_point_col(p2) - 1 * (2 * (i < (row - 2) / 2) - 1));
            draw_line(p1, p2, "▞", sec, k, 0);
            i++;
        } while (i < (row - 2) / 2 && i < col / 7);
        j = i;
        do
        {
            set_point(p1, get_point_row(p1) + 1, get_point_col(p1));
            set_point(p2, get_point_row(p2) + 1, get_point_col(p2) + 1);
            set_point(p3, get_point_row(p2), get_point_col(p2) - 1);
            draw_line(p1, p2, "▞", sec, k, 0);
            draw_line(p3, p3, "▝", sec, k, 0);
            i++;
        } while (i < row - 2 && i < col / (6 - tall + fat));
        set_point(p2, get_point_row(p) + 1, get_point_col(p) - (col > 13) - 1 + col - 1);
        draw_line(p1, p2, "▟", sec, k, 0);

        if(row>5){
            if (col % 2)
                strcpy(ch, "┣━━━┫");
            else
                strcpy(ch, "┣━━┫");
            set_point(p1, get_point_row(p) + j + 1 + 2 * (j <= row / 2), get_point_col(p) + col / 2 - 2);
            draw_line(p1, p1, ch, sec, NULL, 0);
            if (col % 2)
                strcpy(ch, "┗   ┛");
            else
                strcpy(ch, "┗  ┛");
            set_point(p1, get_point_row(p1) + 1, get_point_col(p1));
            draw_line(p1, p1, ch, sec, NULL, 0);
        }else{
            strcpy(ch, "━━━");
            set_point(p1, get_point_row(p) + row-1, get_point_col(p) + col / 2 - 1);
            draw_line(p1, p1, ch, sec, NULL, 0);
        }
    }
    else
    {
        i = 1;
        set_point(p1, get_point_row(p), get_point_col(p) + 1 + (col) / (6 - tall + fat) + (col > 13));
        set_point(p2, get_point_row(p), get_point_col(p) + 1 + 3 * col / 11);
        do
        {
            set_point(p1, get_point_row(p1) + 1, get_point_col(p1) - 1);
            set_point(p2, get_point_row(p2) + 1, get_point_col(p2));
            set_point(p3, get_point_row(p1), get_point_col(p1) - 1);
            draw_line(p1, p2, "▚", sec, k, 0);
            draw_line(p3, p3, "▗", sec, k, 0);
            i++;
        } while (i < row - 2 && i < col / (6 - tall / 2 + fat));
        i = 1;
        set_point(p1, get_point_row(p), get_point_col(p) - 2 + col - (col > 13) - (col) / (6 - tall + fat));
        set_point(p2, get_point_row(p), get_point_col(p) - 2 + col - 3 * col / 11);
        do
        {
            set_point(p1, get_point_row(p1) + 1, get_point_col(p1) + 1);
            set_point(p2, get_point_row(p2) + 1, get_point_col(p2));
            set_point(p3, get_point_row(p1), get_point_col(p1) + 1);
            draw_line(p1, p2, "▞", sec, k, 0);
            draw_line(p3, p3, "▖", sec, k, 0);
            i++;
        } while (i < row - 2 && i < col / (6 - tall / 2 + fat));
        j = i;
        if (col % 2)
            strcpy(ch, "┣━━━┫");
        else
            strcpy(ch, "┣━━┫");
        set_point(p1, get_point_row(p) + j + (j < (col / 6 - tall / 2 + fat)) + 2 * (j <= row / 2), get_point_col(p) + col / 2 - 2);
        draw_line(p1, p1, ch, sec, NULL, 0);
        if (col % 2)
            strcpy(ch, "┗   ┛");
        else
            strcpy(ch, "┗  ┛");
        set_point(p1, get_point_row(p1) + 1, get_point_col(p1));
        draw_line(p1, p1, ch, sec, NULL, 0);
    }
    free_point(&p1);
    free_point(&p2);
    free_point(&p3);
}

static void draw_wings(section *sec, int startcol, int startrow, int endcol, int endrow, pts *p1, pts *p2, pts *paux, kolor *k)
{
    int i, begin, end;

    begin = (endrow - startrow) * 7 / 18 + startrow;
    end = (endrow - startrow) * 13 / 18 + startrow;

    set_point(p1, begin, startcol - 1);
    set_point(p2, end, startcol - 1);

    for (i = 0; i < (get_section_col(sec) - (endcol - startcol)) / 2; i++)
    {
        draw_line(p1, p2, "█", sec, k, 0);
        set_point(p1, get_point_row(p1) + 1, get_point_col(p1) - 1);
        set_point(p2, get_point_row(p2) + (i == 0) - ((i > 2) * i / 3), get_point_col(p2) - 1);
        if ((get_point_row(p2) - get_point_row(p1)) < 1)
            i = endcol;
    }

    set_point(p1, begin, endcol + 1);
    set_point(p2, end, endcol + 1);
    for (i = 0; i < (get_section_col(sec) - (endcol - startcol)) / 2; i++)
    {
        draw_line(p1, p2, "█", sec, k, 0);
        set_point(p1, get_point_row(p1) + 1, get_point_col(p1) + 1);
        set_point(p2, get_point_row(p2) + (i == 0) - ((i > 2) * i / 3), get_point_col(p2) + 1);
        if ((get_point_row(p2) - get_point_row(p1)) < 1)
            i = endcol;
    }
}

status draw_bee(section *sec, char *origname, short lang)
{
    short endcol, endrow, flag = 1;
    int row, col, startrow, startcol;
    char cname[NAMELEN];
    char fname[50];
    body *b;
    /*cloth *c;*/
    pts *p = NULL;
    pts *p1 = NULL, *p2 = NULL;
    kolor *k = NULL;
    section *face = NULL;
    FILE *f;
    if (!sec)
        return ERROR;
    
    strcpy(fname, "quests/");
    if (lang == 0)
    {
        strcat(fname, "en");
    }
    else
    {
        strcat(fname, "es");
    }
    strcat(fname, "char.txt");
    if (!(f = fopen(fname, "r")))
    {
        return ERROR;
    }
    
    if (create_body(&b) == ERROR)
    {
        fclose(f);
        return ERROR;
    }
    while (flag && fscanf(f, "\n%[^\n] %hd %hd %hd %hd", cname, &(b->tall), &(b->fat), &(b->angry), &(b->hair)) != EOF)
    {
        if (strcmp(cname, origname) == 0)
        {
            flag = 0;
        }
    }
    fclose(f);
    if (!(p = get_section_point(sec)))
    {
        free_body(&b);
        return ERROR;
    }
    if (create_point(&p1) == ERROR)
    {
        free_point(&p);
        free_body(&b);
        return ERROR;
    }
    if (create_point(&p2) == ERROR)
    {
        free_point(&p);
        free_point(&p1);
        free_body(&b);
        return ERROR;
    }
    if (create_kolor(&k) == ERROR)
    {
        free_point(&p);
        free_point(&p1);
        free_point(&p2);
        free_body(&b);
        return ERROR;
    }
    if (create_section(&face) == ERROR)
    {
        free_point(&p);
        free_point(&p1);
        free_point(&p2);
        free_kolor(&k);
        free_body(&b);
        return ERROR;
    }
    set_kolor(k, GREY_BRIGHT, DEFAULT_BG);
    row = get_section_row(sec);
    col = get_section_col(sec);
    startrow = (get_point_row(p)) + 3 * row / (9 + b->tall * 4);
    startcol = (get_point_col(p)) + 2 * col / (9 + b->fat * 3);
    endrow = (get_point_row(p)) + row - row / (5 + b->tall * 4);
    endcol = (get_point_col(p) - 1) + col - 2 * col / (9 + b->fat * 3);
    draw_hair(p, sec, k, b->hair, startrow, startcol, endrow, endcol);
    draw_body(sec, p1, p2, startrow, startcol, endrow, endcol);
    set_point(p1, startrow + 1, startcol + 1);
    set_kolor(k, YELLOW_DARK, DEFAULT_BG);
    set_section(face, p1, (endrow - startrow) * 7 / 18 - 1, endcol - startcol - 1, k);
    switch (b->angry)
    {
    case 2:
        set_kolor(k, RED_DARK, DEFAULT_BG);
        break;
    case 1:
        set_kolor(k, CYAN_DARK, DEFAULT_BG);
        break;
    default:
        set_kolor(k, 39, DEFAULT_BG);
        break;
    }
    draw_face(face, k, p1, b->angry, b->tall, b->fat);
    set_kolor(k, GREY_BRIGHT, DEFAULT_BG);
    draw_wings(sec, startcol, startrow, endcol, endrow, p1, p2, p, k);
    free_point(&p);
    free_point(&p1);
    free_point(&p2);
    free_kolor(&k);
    free_section(&face);
    free_body(&b);
    return OK;
}

void *display_bee(void *voidb){
    beestruct *b = (beestruct *)voidb;
    pts *p;
    kolor *k;
    int row,col,i=0,j;
    if(!(p=get_section_point(b->sec))) return NULL;
    if(!(k=get_section_kolor(b->sec))){
        free_point(&p);
        return NULL;
    }
    row=get_point_row(p);
    col=get_point_col(p);
    while(b->aux!=-1){
        if(b->aux==1){
            pthread_mutex_lock(&beemutex);
            clear_section(b->sec);
            set_point(p,row-((i++)%2)+1,col);
            set_section(b->sec,p,get_section_row(b->sec),get_section_col(b->sec),k);
            draw_bee(b->sec,b->name,b->lang);
            j=0;
            while(b->aux==1 && j<10000){
                usleep(5);
                j++;
            }
            pthread_mutex_unlock(&beemutex);
        }
    }
    return NULL;
}