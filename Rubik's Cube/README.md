https://github.com/pcuestas/PPROG_CUBE

### Autores: Ignacio Bernardez, Pablo Cuesta y Álvaro Zamanillo.
Nombre del grupo: *`is_prime()`*

(se compila haciendo 'make all' y se ejecuta el juego con 'make run')

## Comandos para instalar en Ubuntu las librerías necesarias para compilar el juego
```
sudo apt-get install libsdl2-2.0
sudo apt-get install libsdl2-dev 
sudo apt-get install libsdl2-ttf-dev 
sudo apt-get install freeglut3-dev
```

## Controles dentro de la interfaz del cubo 

**Movimientos del cubo**:
* RLFBUD: son los movimientos de caras en sentido horario (mismas letras en minúscula, para los movimientos antihorarios)
* XYZxyz: rotaciones del cubo completo (mayúscula horario, minúscula antihorario)
* MESmes: mueven las capas centrales
	
Todos estos movimientos son considerados estándar y se conocen como [Singmaster notation](https://es.wikipedia.org/wiki/Cubo_de_Rubik#Notaci%C3%B3n) –aunque nosotros utilizamos letras mayúsculas o minúsculas para movimientos horarios o antihorarios, respectivamente.

**Además se puede usar**: 
* w: selecciona (pseudo)aleatoriamente una de entre todas las mezclas en el fichero scrambles.txt y pone el cronómetro a cero
* W: utiliza el algoritmo de solver.c para generar una string que resuelve el cubo, la ejecuta de una vez e imprime el cubo (resuelto)
* A: muestra en la pantalla la string generada por el solver.h, si no cabe de una vez se puede ver la siguiente parte de la solución volviendo a pulsar 'A'.
* a: resuelve el cubo ejecutando cada movimiento con una pequeña espera antes de cada uno para que se vea en la pantalla cómo el algoritmo utilizado resuelve el cubo.
* espacio: para/reinicia el cronómetro. Si el cubo está resuelto, aparece u mensaje en la pantalla, del que se sale presionando cualquier tecla.


**Solo en la interfaz de la terminal**:
* q: sale de la interfaz
* o: limpia la terminal y vuelve a imprimir el contenido por si hubiese ocurrido algún error. Pensado para hacer `CTRL`+`+`, o `CTRL`+`-`; y luego utilizar `o` para actualizar la pantalla.

**Solo en la interfaz de la biblioteca SDL**:
*  ESC: sale de la interfaz
