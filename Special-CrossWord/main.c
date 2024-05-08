#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

pthread_mutex_t mutex;
enum {
    horizontal = 10,
    vertical = 10
};

// Estructura para mantener las respuestas y sus posiciones
typedef struct {
    char palabra[20];
    char pregunta[150];
    int inicio_fila;
    int inicio_col;
    int fin_fila;
    int fin_col;
    bool usada;
} Respuesta;

Respuesta respuestasTablero1[] = {
        {"COMPILADOR", "\n ¿Qué término se refiere a un conjunto finito de instrucciones bien definidas para resolver un problema?", 0, 5, 9, 5, false},
        {"ALGORITMO", "\n ¿Qué estructura de control se utiliza para realizar una acción específica si se cumple una condición dada?", 0, 8, 8, 8, false},
        {"FUNCION", "\n ¿Qué estructura de programación permite reutilizar código al encapsularlo para realizar una tarea específica?", 1, 0, 1, 0 + 6, false},
        {"CICLO", "\n ¿Qué estructura de control se utiliza para repetir un bloque de código hasta que se cumpla una condición?", 1, 3, 1 + 4, 3, false},
        {"CLASE", "\n ¿Qué constructo en la programación orientada a objetos agrupa datos y funciones que operan sobre esos datos?", 6, 3, 6, 3 + 4, false},
        {"METODO", "\n ¿Qué se llama a las funciones dentro de una clase en la programación orientada a objetos?", 8, 0, 8, 0 + 5, false},
        {"PUNTERO", "\n ¿Qué tipo especial de variable almacena la dirección de memoria de otra variable?", 9, 0, 9, 0 + 6, false},
        {"WHILE", "\n ¿Qué tipo de bucle se ejecuta mientras una condición sea verdadera?", 2, 7, 2 + 4, 7, false}
};

Respuesta respuestasTablero2[] = {
        {"HERENCIA", "\n ¿Qué tipo de relación entre clases implica que una clase herede atributos y métodos de otra?", 9, 0, 9, 7, false},
        {"COLA", "\n ¿Qué estructura de datos se caracteriza por su comportamiento tipo \"primero en entrar, primero en salir", 6, 7, 8, 7, false},
        {"ESTATICO", "\n ¿Qué término se refiere a una variable que comparte su valor entre todas las instancias de una clase?", 7, 0, 7, 8, false},
        {"BREAK", "\n ¿Qué tipo de instrucción permite salir abruptamente de un bucle?", 4, 5, 4, 9, false},
        {"IF-ELSE", "\n ¿Qué estructura de control permite ejecutar un bloque de código si una condición es verdadera y otro bloque si es falsa?", 0, 3, 0, 9, false},
        {"GLOBAL", "\n ¿Qué tipo de variable está definida fuera de cualquier función y puede ser accedida desde cualquier parte del programa?", 3, 3, 9, 3, false},
        {"DO-WHILE", "\n ¿Qué tipo de bucle se ejecuta al menos una vez y luego comprueba la condición?", 0, 0, 9, 0, false},
        {"SINTAXIS", "\n ¿Qué tipo de error ocurre durante la compilación de un programa?", 0, 8, 7, 8, false}
};

Respuesta respuestasTablero3[] = {
        {"INDEFINIDO", "\n ¿Qué tipo de error ocurre cuando una variable se utiliza antes de ser inicializada?", 2, 0, 2, 9, false},
        {"GETTER", "\n ¿Qué método de una clase se utiliza para acceder a los atributos privados de esa clase?", 1, 3, 5, 3, false},
        {"CONJUNTO", "\n ¿Qué estructura de datos se caracteriza por almacenar elementos de manera ordenada y sin elementos repetidos?", 1, 9, 8, 9, false},
        {"ARBOL", "\n ¿Qué estructura de datos organiza elementos de manera jerárquica?", 6, 2, 6, 6, false},
        {"LOCAL", "\n ¿Qué tipo de variable está definida dentro de una función y solo es accesible dentro de ella?", 3, 2, 7, 2, false},
        {"CONSTANTE", "\n ¿Qué tipo de variable guarda un valor que no cambia durante la ejecución del programa?", 0, 1, 8, 1, false},
        {"PILA", "\n ¿Qué estructura de datos se caracteriza por su comportamiento tipo \"último en entrar, primero en salir\"?", 1, 5, 4, 5, false}
};

Respuesta TableroModificado[] = {
        {"INTEGER", "\n ¿Cuál es la longitud máxima de un número entero de 32 bits sin signo en un lenguaje de programación típico?", 2, 0, 2, 6, false},
        {"SETTER", "\n ¿Cuál es la función utilizada para asignar un valor entero a una variable en muchos lenguajes de programación?", 1, 3, 5, 3, false},
        {"CONJUNTO", "\n ¿Qué estructura de datos se caracteriza por almacenar elementos de manera ordenada y sin elementos repetidos?", 1, 9, 8, 9, false},
        {"ARBOL", "\n ¿Qué estructura de datos organiza elementos de manera jerárquica?", 6, 2, 6, 6, false},
        {"LOCAL", "\n ¿Qué tipo de variable está definida dentro de una función y solo es accesible dentro de ella?", 3, 2, 7, 2, false},
        {"CONSTANTE", "\n ¿Qué tipo de variable guarda un valor que no cambia durante la ejecución del programa?", 0, 1, 8, 1, false},
        {"PILA", "\n ¿Qué estructura de datos se caracteriza por su comportamiento tipo \"último en entrar, primero en salir\"?", 1, 5, 4, 5, false}
};

Respuesta TableroSegundoModificado[] = {
        {"INTEGER", "\n ¿Cuál es la longitud máxima de un número entero de 32 bits sin signo en un lenguaje de programación típico?", 2, 0, 2, 6, false},
        {"SETTER", "\n ¿Cuál es la función utilizada para asignar un valor entero a una variable en muchos lenguajes de programación?", 1, 3, 5, 3, false},
        {"CONJUNTO", "\n ¿Qué estructura de datos se caracteriza por almacenar elementos de manera ordenada y sin elementos repetidos?", 1, 9, 8, 9, false},
        {"ARREGLO", "\n ¿Qué estructura de datos organiza elementos de manera jerárquica?", 6, 2, 6, 8, false},
        {"LOCAL", "\n ¿Qué tipo de variable está definida dentro de una función y solo es accesible dentro de ella?", 3, 2, 7, 2, false},
        {"FUNCION", "\n ¿Qué estructura de programación permite reutilizar código al encapsularlo para realizar una tarea específica?x", 0, 1, 6, 1, false},
        {"PILA", "\n ¿Qué estructura de datos se caracteriza por su comportamiento tipo \"último en entrar, primero en salir\"?", 1, 5, 4, 5, false}
};

Respuesta *respuestas;
int numeroRespuestas;
int cambiar_tablero = 0;
char tablero[horizontal][vertical];

// Función para inicializar el tablero
void inicializarTablero(char tablero[horizontal][vertical]) {
    for (int i = 0; i < horizontal; i++) {
        for (int j = 0; j < vertical; j++) {
            tablero[i][j] = '*';
        }
    }
}

// Función para determinar si una posición del tablero debe mostrar '#'
bool mostrarOculto(int fila, int col) {
    for (int i = 0; i < numeroRespuestas; i++) {
        if (!respuestas[i].usada) {
            if (fila >= respuestas[i].inicio_fila && fila <= respuestas[i].fin_fila & col >= respuestas[i].inicio_col && col <= respuestas[i].fin_col) {
                return true;
            }
        }
    }
    return false;
}

// Muestra oculto los demás ubicaciones
void imprimirTablero(char tablero[horizontal][vertical]) {
    for (int i = 0; i < horizontal; i++) {
        printf("%d\t", i);
        for (int j = 0; j < vertical; j++) {
            if (mostrarOculto(i, j)) {
                printf("#\t");
            } else {
                printf("%c\t", tablero[i][j]);
            }
        }
        printf("\n");
    }
}


// Mostrar perguntas para el crucigrama
void mostrarPreguntas() {
    for (int i = 0; i < numeroRespuestas; i++) {
        if (!respuestas[i].usada) {
            printf("Pregunta: %s\n", respuestas[i].pregunta);
        }
    }
}

//Hilo para el saludo del programa
void *saludar(void *arg) {
    printf("\n\t\t!!Bienvenido al Crucigrama Especial!!\n");
    printf("\nDurante el juego se darán el todas las preguntas, para que puedas adivinar\n");
    printf("Considera que hay 1 minuto para resolver todo el tablero, apresurate...\n\n");
    pthread_exit(NULL);
}


// Colocar palabras en el tablero con las coordenadas definidas
void colocarPalabra(char tablero[horizontal][vertical], int index) {
    pthread_mutex_lock(&mutex);
    Respuesta resp = respuestas[index];
    int fila = resp.inicio_fila;
    int col = resp.inicio_col;
    int longitud_palabra =  strlen(resp.palabra);
    int contador = 0;

    while (fila <= resp.fin_fila && col <= resp.fin_col && contador < longitud_palabra) {
        tablero[fila][col] = resp.palabra[contador];
        if (col == resp.fin_col) {
            fila++;
        } else {
            col++;
        }
        contador++;
    }
    respuestas[index].usada = true;
}

// Comprueba si la palabra es correcta y no ha sido usada
int comprobarRespuesta(char *entrada) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < numeroRespuestas; i++) {
        if (strcmp(entrada, respuestas[i].palabra) == 0 && !respuestas[i].usada) {
            return i;
        }
    }
    return -1;
}

//Cambio de tablero usando alarmas
void manejarAlarma(int sig) {
    cambiar_tablero++;
    if (cambiar_tablero == 1) {
        printf("\nCUIDADO: Ha cambiado el tablero y las preguntas!\n");
        respuestas = TableroModificado;
        numeroRespuestas = sizeof(TableroModificado) / sizeof(TableroModificado[0]);
    } else if (cambiar_tablero == 2) {
        printf("\nCUIDADO: Ha cambiado el tablero  y las preguntas nuevamente!\n");
        respuestas = TableroSegundoModificado;
        numeroRespuestas = sizeof(TableroSegundoModificado) / sizeof(TableroSegundoModificado[0]);
    } else {
        printf("\nJuego completado o tiempo agotado!\n");
        exit(0);
    }
    alarm(30);
    inicializarTablero(tablero);
    imprimirTablero(tablero);
    mostrarPreguntas();
}

bool verificarFinJuego() {
    for (int i = 0; i < numeroRespuestas; i++) {
        if (!respuestas[i].usada) {
            return false;
        }
    }
    return true;
}


void procesarRespuesta(char *entrada) {
    pid_t pid = fork();

    if (pid == 0) {
        int resultado = comprobarRespuesta(entrada);
        if (resultado != -1) {
            colocarPalabra(tablero, resultado);
            printf("Respuesta correcta y colocada en el tablero.\n");
        } else {
            printf("Respuesta incorrecta o ya utilizada.\n");
        }
        exit(0);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("El proceso de verificación ha completado con estado %d.\n", WEXITSTATUS(status));
        }
    } else {
        printf("Fallo al crear proceso hijo.\n");
    }
}

int main() {
    char entrada[50];
    int resultado;
    int opcion;
    pthread_t saludo_Usuario;

    signal(SIGALRM, manejarAlarma);
    pthread_create(&saludo_Usuario, NULL, saludar, NULL);
    pthread_join(saludo_Usuario, NULL);

    printf("Seleccione un tablero:\n1. Programación I\n2. Programacion II\n3. Programacion III\nIngrese el número del tablero: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            respuestas = respuestasTablero1;
            numeroRespuestas = sizeof(respuestasTablero1) / sizeof(respuestasTablero1[0]);
            break;
        case 2:
            respuestas = respuestasTablero2;
            numeroRespuestas = sizeof(respuestasTablero2) / sizeof(respuestasTablero2[0]);
            break;
        case 3:
            respuestas = respuestasTablero3;
            numeroRespuestas = sizeof(respuestasTablero3) / sizeof(respuestasTablero3[0]);
            break;
        default:
            printf("Ingresa una opción que se encuentre en las disponibles...\n");
            return 0;
    }
        alarm(30);
        inicializarTablero(tablero);
        imprimirTablero(tablero);
        mostrarPreguntas();

        while (true) {
            printf("\n\t\tOJO DEBE DE ESTAR TODO EN MAYUSCULAS SIN ACENTOS, CONSIDERA TAMBIEN SEPARACIONES USANDO '-', DISFRUTA :) Y APRESURATE");
            printf("\n\nIngrese una respuesta basada en las preguntas mostradas (o 'salir' para finalizar): ");
            scanf("%s", entrada);

            if (strcmp(entrada, "salir") == 0) {
                break;
            }
            resultado = comprobarRespuesta(entrada);
            if (resultado != -1) {
                procesarRespuesta(entrada);
                colocarPalabra(tablero, resultado);
                imprimirTablero(tablero);
                mostrarPreguntas();
            } else {
                printf("Respuesta incorrecta o ya utilizada. \nIntenta de nuevo.\n");
            }
            if (verificarFinJuego()) {
                printf("\n¡Felicidades, has completado el juego!\n");
                break;
            }
        }
        if (cambiar_tablero > 0) {
        inicializarTablero(tablero);
        imprimirTablero(tablero);
        mostrarPreguntas();
        cambiar_tablero = 0;
            }
        return 0;
}