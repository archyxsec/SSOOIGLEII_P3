# `ssooigle(2)` Práctica 3

![Build Status](https://img.shields.io/badge/Maintained%3F-yes-green.svg) 
![Creator](https://img.shields.io/badge/Creator-BlaZzes10%20and%20SergioSanchezIglesias-blue)
[![Partners](https://img.shields.io/badge/Partners-2-red)](https://github.com/BlaZzes10/python/graphs/contributors)
![Version](https://img.shields.io/badge/version-1.0-brightgreen)

## **Esquema**

![imagen](https://user-images.githubusercontent.com/52278758/118182133-ecdce580-b438-11eb-807a-be906a0c69f8.png)

## **Idea**

El proyecto consta de un programa principal `Buscador.cpp` que se encarga de crear el sistema de pago y unos determinados clientes (en `definitions.h` por defecto son 50), ambos como procesos. Este buscador antes de crear los clientes inicia tres hilos. Uno de ellos se encarga de manejar las peticiones de los procesos clientes que se harán mediante memoria compartida y semáforos. Otro de estos hilos se encarga de extraer de la cola de peticiones, siguiendo una cierta condición mediante variables de condición, y crea Manejadores de clientes `Client_Management.cpp` que se encargara de dividir los ficheros por hilos y estos a la vez, si un fichero se asigna a varios hilos se dividen las líneas. Cuando este Manejador de clientes termina de encontrar las coincidencias se las envía a través de una tubería a los clientes y notifica al *hilo* encargado de la terminación de los manejadores de clientes mediante un semáforo.

Es necesario recalcar que el esquema de la práctica esta dividido por 50 procesos clientes generados aleatoriamente, un proceso Sistema de Pago, un proceso Buscador y los respectivos procesos Client_Management que solo puede haber 4 en activo. Se ha decidido comunicar *procesos* mediante segmentos de memoria compartida y semáforos. A su vez, el sistema de Pago lanzará tres hilos encargados de la gestión de los clientes como se ha explicado anteriormente. Los procesos Client_Management también lanzan una serie de hilos encargados de realizar la busqueda, este número de hilos estan definidos en `definitions.h`. Para la sincronización de procesos se ha utilizado funciones típicas del estandar de C, sin embargo, hemos usado más recursos de C++11 tal y como se puede apreciar en la siguiente imagen:

![imagen](https://user-images.githubusercontent.com/52278758/118179213-45aa7f00-b435-11eb-8583-bf27ac5b4586.png)

Estos recursos corresponden a semáforos encargados de sincronizar los hilos, variables de condición, semáforos binarios encargados de proteger la sección crítica de la cola de coincidencias del Client_Management, y muchos más recursos que invitamos a observar.

También se ha controlado si uno de los hilos del Client_Management genera una excepción mediante promises y Futures.

En general, hemos combinado todos los conocimientos, y más... adquiridos durante este curso, combinando conocimientos de C y C++ para el desarrollo de esta práctica explotando al máximo todo el potencial de nuestro computador al paralelizar hilos y procesos, así como sería en un *entorno real*.

El día de la entrega cambiaré el acceso al repositorio para que sea público y este al acceso de todos!.

## **Requeriments:** 
`g++` with __c++11__ standard

## **Prepare**
```bash
git clone https://github.com/BlaZzes10/SSOOIGLEII_P3.git
cd SSOOIGLEII_P3 && make
```

## **Run**
commun test:
```bash
make solution
```

## Credits
`Developed by Tomás Jesús García López(BlaZzes10) and Sergio Sánchez Iglesias(SergioSanchezIglesias)`
