# Tarea-Programada-usando-MPI
## El sistema que se va a simular:
Un satélite "estudia"zonas que una vez fueron boscosas y que han sufrido erosión por el ser humano o la naturalezaen diferentes
países del mundo, paradeterminarsi hay posibilidad de reforestación a un costo no muy alto. La ideaen última instancia, es 
identificar la máxima cantidad de terreno boscosoerosionado en el mundo que puede ser reforestado a un costo razonable. Este 
satélite, cuando trabaja para identificar el estado de un bosqueespecífico, lo hace hectárea1por hectárea,y envía el estado de 
cada una de estas hectáreasa una delas computadoras de un gran centro de investigación, llamado Arenal-ECCI.Los datos que se 
reciben desde el satélite, vienen ya preprocesados y codificados de la siguiente manera:(un código por cada hectárea)
 ### 0 indica que es casi imposible de reforestar, lo cual implicaría una gran inversiónpara lograrlo.
 ### 1 indica que es posible a un alto costo
 ### 2 es posible con un costo razonable (determinado por la agencia ambiental del país en el que se lleva a cabo el estudio)
 Estos datos, una vez recibidos desde el satélite, se  almacenan en un disco y serán puestos en memoria por uno de los procesos
 que ud debe crear para esta tarea, y que se describe más adelante.La división en cuadrantes no es arbitraria, ya que cada uno
 representa cierta área geográfica que se pude ver como una unidad para su tratamiento en la posible reforestación. 
 Para el estudio por el satélite, un bosque "se divide"de forma muy regular en hectáreas, las cuales se agrupan
 (también de manera muy regular) en cuadrantes.  Lo ideal es que se pudiera reforestar todo el bosque, sin embargo la decisión se
 hace cuadrante por cuadrante, de la siguiente manera:Si no más del 20% del área del cuadrante implica una gran inversiónpara
 reforestar y al menos un 50%  es posible de reforestar a un costo razonable, entonces se decide que el cuadrante 
 será reforestado en su totalidad.Si lo anterior no se cumple, no se reforesta el cuadrante.Si al menos el 50% del total de 
 hectáreas del bosque serán reforestadas, se obtendrá un beneficio de manera que el costo se reduce. Si todo el bosque está apto 
 para reforestación, el costo será de 0 para ese país, pues será asumido por cierta agencia internacional que vela por el pobre 
 planeta.
 
 ## Su tarea
 1. Lo primero es crear lo datos, como si tuviéramos el satélite y el instituto de investigación Arenal-ECCI, es decir, cada vez 
 que corre su programacreando procesos,este programadebe crear los datos que se obtendríancon elsatélite para cierto bosque.
 La idea es que uno solo de susprocesosgenere al azar la matriz que representa el bosque y que se tendrá en la memoria 
 de ese proceso. La generación de las entradas de esta matriz (0, 1 ó 2) se hará con ciertas "distribuciones de probabilidad  ocultas" que se
 ofrecerán para pruebas.
 
 Para definir la dimensión de esta matriz cuadradaque representa el bosque se procede así: Si el usuario creó "p" procesos, se debe obtener
 el máximo divisor de este número. Sea x este máximo divisor. 
 ** x: ** indicará el número de cuadrantesque necesita el bosque por "lado" de la matriz. Cada uno de estos cuadrantes tendrá representadas 
 p*p hectáreas.
 
 En total habrá x*x cuadrantes en la matriz, cada uno con p*p hectáreas. Es decir,la matriz tendrá x*p filas  y x*p columnas para un total de
 x^2*p^2 entradas.
 
 2. Una vez que se cuenta con los datos del satélite en memoria, es decir, la matriz generada por un procesose encuentraalmacenada en memoria por
 filas, sus procesos deben
 ### (1ero)trabajar de manera eficiente,
 ### (2do) aprovechando al máximo las facilidades que ofrece MPI y 
 ### (3ero) paralelizando de manera correcta (equilibrio)
 
 para resolver lo que se desea averiguar del bosque en estudio:
 
 a. Desplegar de manera clara la matriz que se generó (cuando su dimensión tenga sentido) 
 b. Desplegar información general del bosque y resumen del resultado del análisis: 
  - su área total
  - Numero de cuadrantes
  - Numero de hectáreas por cuadrante 
  - Total de área del bosque(en número de hectáreas )
  - % delárea a reforestar.
  - si todo el bosque está apto para reforestación (y el costo sería de 0) ó
  - si al menos el 50% de sus hectáreas serán reforestadas  (se gana cierto beneficio en el costo)ó 
  - si no se obtiene beneficio enelcosto
  
  c. Por cada cuadrante, enumerados en orden de izqa derecha y de arriba hacia abajo 0, 1, 2, 3, 4,...:
  - % del área del cuadrante
   -que implica unagran inversiónpara  reforestar - con alto costopara reforestar
   -con costo razonable
  - Si se debe reforestar o no. Justificado muy claramente con los cálculos obtenidos anteriores.
