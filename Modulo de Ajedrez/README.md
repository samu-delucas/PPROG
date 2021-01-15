

# INSTRUCCIONES MÓDULO DE AJEDREZ 

## INICIO DE PARTIDA

Al comienzo de la partida se debe de seleccionar bando, utilizando w para jugar con blancas y b con negras (pulsando ENTER a continuación).

## MOVIMIENTOS

El formato utilizado para introducir movmientos por teclado será el siguiente:
(P)from(x)to(=C) donde se da que:
 - P corresponde a la pieza que se omite si se mueve un peón (en mayúsculas)
 - from corresponde a la casilla desde la que se mueve la pieza (en minúsculas)
 - x indica si hay captura. Si no hay captura se omite
 - to corresponde a la casilla a la que va la pieza (en minúsculas)
 - =C se escribe si hay una coronación y en C se escribe la pieza en la que se corona (en mayúsculas)
 - En el caso de que el movimiento sea un enroque se escribirá 0-0 para el enroque en el ala de rey y 0-0-0 para el enroque en el ala de dama

### EJEMPLOS

* Peon de e2 a e4: e2e4
* Caballo de b1 a c3: Nb1c3
* Torre de h8 a h2: Rh8h2
* Alfil de h2 a g3 y captura: Bh2xg3
* Reina de h5 a h1: Qh5h1
* Enroque corto(de rey): 0-0
* Peon de h7 a h8 y corona en dama: h7h8=Q


## COMANDOS ADICIONALES

### exit: 
La partida finaliza en la situación en la que esté

### mod:

Aunque sea el turno del jugador, puede utilizar mod para que mueva la máquina pero cuidado, si juegas con blancas por ejemplo y utilizas mod, debes de utilizar mod en el turno de las negras también, y ya luego tras tu movimiento el módulo moverá solo.














