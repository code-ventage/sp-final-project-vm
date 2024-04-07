# sp-final-project-vm

## Descripción

Este proyecto consiste en un servidor concurrente TCP que escucha en el puerto 6666. Cada conexión es atendida por un nuevo proceso hijo. El cliente envía en la solicitud las tres letras correspondientes a la moneda que quiere saber el cambio y recibe el valor del cambio como respuesta, la cual se muestra en pantalla. Además, el hijo por una tubería le pasa al padre la IP del cliente, la cual se imprime por pantalla. El programa ignora el CTRL/C y evita la creación de procesos zombies (maneja la señal SIGCHLD).

## Monedas Soportadas

| Nombre | Sigla | TC Población |
| ------ | ----- | ------------ |
| Dólar Australiano | AUD | 78.27000 |
| Nuevo Peso Mexicano | MXN | 7.23382 |
| Libra Esterlina | GBP | 151.66200 |
| Corona Noruega | NOK | 11.08806 |
| Corona Sueca | SEK | 11.24343 |
| Dólar Americano | USD | 120.00000 |
| Dólar Canadiense | CAD | 88.64593 |
| Yen Japonés(*) | JPY | 1.26021 |
| Corona Danesa | DKK | 17.37796 |
| Franco Suizo | CHF | 133.11148 |
| Euro | EUR | 129.61800 |

## Uso

Para ejecutar el servidor, compile y ejecute el archivo `smain.c`:

```bash
gcc smain.c -o server
./server
```

Para ejecutar el cliente, compile y ejecute el archivo `cmain.c`:

```bash
gcc cmain.c -o client
./client
```

El cliente enviará las siglas de las monedas al servidor y recibirá el valor del cambio como respuesta. Este valor se mostrará en la pantalla del cliente. Además, el servidor imprimirá la IP del cliente.

## Código

El código del proyecto se encuentra en los archivos `smain.c` (servidor) y `cmain.c` (cliente).

## Integrante
- [Victor Manuel Palmero Valdes](https://github.com/palmerovicdev)