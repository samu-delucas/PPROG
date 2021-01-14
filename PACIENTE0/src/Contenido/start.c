/*
    Este fichero sortea al paciente 0, acertijos, pistas y los prepara para ser usados.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <start.h>
#include <acertijosPistas.h>

/***********************************************/
/* Función que sortea al paciente 0, abre el   */
/* fichero start.txt donde se van a guardar los*/
/* nunmeros de acertijos, pistas y minijuegos  */
/* sorteados con el numero de inocente que     */
/* descarta cada 1.                            */
/* Para esto llama a startAcertijos            */
/* y startPistas de acertijosPistas.c          */
/***********************************************/
Status startGame()
{
    FILE *f;
    int x1[3], x2[3];
    //En el array inocentes guardamos el numero de los sospechosos descartados
    //para evitar que dos acertijos o pistas descarten al mismo sospechoso
    int paciente0, nInocentes = 0;
    int *inocentes = NULL;

    //Fichero donde guardamos los datos aleatorios de la partida
    f = fopen("./aux/start.txt", "w");
    if (f == NULL)
        return ERROR;
    srand(time(NULL));

    //Sorteamos el paciente 0
    paciente0 = aleat_num(0, 9);
    fprintf(f, "%d\n", paciente0);

    //Inicializanos la lista de inocentes descartados
    inocentes = (int *)malloc(9 * sizeof(int));
    if (inocentes == NULL)
    {
        fclose(f);
        return ERROR;
    }

    //Llamamos a la función que prepara los acertijos para ser usados
    if (startAcertijos(f, inocentes, &nInocentes, paciente0) == ERROR)
    {
        fclose(f);
        free(inocentes);
        return ERROR;
    }

    //ESTA PARTE SEGURAMENTE CAMBIARA CUANDO ESTEN ACABADOS LOS MINIJUEGOS
    //Sorteamos los minijuegos y los guardamos en el array x1
    //Sorteamos los inocentes que se descubren en cada minijuego y los guardamos en x2
    x1[0] = aleat_num(0, 3);
    x2[0] = posibleInocente(inocentes, &nInocentes, paciente0);
    fprintf(f, "%d\t%d\n", x1[0], x2[0]);
    x1[1] = x1[0];
    while (x1[1] == x1[0])
    {
        x1[1] = aleat_num(0, 3);
    }
    x2[1] = posibleInocente(inocentes, &nInocentes, paciente0);
    fprintf(f, "%d\t%d\n", x1[1], x2[1]);
    x1[2] = x1[0];
    while (x1[2] == x1[0] || x1[2] == x1[1])
    {
        x1[2] = aleat_num(0, 3);
    }
    x2[2] = posibleInocente(inocentes, &nInocentes, paciente0);
    fprintf(f, "%d\t%d\n", x1[2], x2[2]);

    //Llamamos a la función que prepara las pistas para ser usadas
    if (startPistas(f, inocentes, &nInocentes, paciente0) == ERROR)
    {
        fclose(f);
        free(inocentes);
        return ERROR;
    }

    //Liberamos memoria
    fclose(f);
    free(inocentes);
    return OK;
}