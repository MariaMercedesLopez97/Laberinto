#include <iostream>     // Biblioteca de entrada/salida estándar.
#include <vector>       // Biblioteca de vectores dinámicos.
#include <queue>        // Biblioteca de colas (estructura FIFO).
#include <ctime>        // Biblioteca para manejar el tiempo.
#include <cstdlib>      // Biblioteca de funciones generales, incluyendo rand() y srand().
#include <algorithm>    // Biblioteca de algoritmos, para funciones estándar como random_shuffle.
#include <chrono>       // Biblioteca para medir el tiempo.
#include <cstdio>       // Biblioteca para usar printf

using namespace std;    // Permite usar los nombres en el espacio de nombres estándar sin calificación.
using namespace chrono; // Permite usar los nombres en el espacio de nombres chrono sin calificación.

const int MURO = 0;     // Constante para representar un muro.
const int CAMINO = 1;   // Constante para representar un camino.
const int VISITADO = 2; // Constante para representar una celda visitada.

struct Punto {          // Estructura para representar un punto (coordenadas x, y) en el laberinto.
    int x, y;
};

int dx[] = {0, 1, 0, -1}; // Array para facilitar el movimiento en el eje x (derecha, abajo, izquierda, arriba).
int dy[] = {1, 0, -1, 0}; // Array para facilitar el movimiento en el eje y (derecha, abajo, izquierda, arriba).

class Laberinto { // La clase Laberinto permite crear, visualizar y resolver laberintos generados aleatoriamente,
                  // proporcionando una representación visual y funcional de un laberinto en una matriz bidimensional.
public:
    Laberinto(int ancho, int altura); // Constructor para inicializar el laberinto.
    void generar();                   // Método para generar el laberinto.
    void mostrar();                   // Método para mostrar el laberinto.
    bool resolver();                  // Método para resolver el laberinto.

private:
    int ancho, altura;                // Ancho y alto del laberinto.
    vector<vector<int>> laberinto;    // Matriz que representa el laberinto.
    void tallar(int x, int y);        // Método para tallar caminos en el laberinto.
    bool estaDentro(int x, int y);    // Método para comprobar si una coordenada está dentro de los límites del laberinto.
};

// Constructor que inicializa el laberinto con muros y establece la semilla para números aleatorios.
Laberinto::Laberinto(int ancho, int altura) : ancho(ancho), altura(altura) {
    laberinto.resize(altura, vector<int>(ancho, MURO)); // Inicializa todas las celdas como muros.
    srand(time(0)); // Establece la semilla para el generador de números aleatorios.
}

// Método para generar el laberinto.
void Laberinto::generar() {
    tallar(1, 1); // Comienza a tallar el laberinto desde la celda (1, 1).
    laberinto[0][1] = CAMINO; // Establece la entrada del laberinto.
    laberinto[altura - 1][ancho - 2] = CAMINO; // Establece la salida del laberinto.
}

// Método recursivo para tallar caminos en el laberinto.
void Laberinto::tallar(int x, int y) {
    laberinto[y][x] = CAMINO; // Marca la celda actual como camino.
    vector<int> dirs = {0, 1, 2, 3}; // Direcciones posibles para moverse.
    random_shuffle(dirs.begin(), dirs.end()); // Mezcla las direcciones aleatoriamente.

    for (int i = 0; i < 4; ++i) { // Intenta moverse en cada dirección.
        int nx = x + dx[dirs[i]] * 2; // Calcula la nueva coordenada x.
        int ny = y + dy[dirs[i]] * 2; // Calcula la nueva coordenada y.
        if (estaDentro(nx, ny) && laberinto[ny][nx] == MURO) { // Si la nueva coordenada está dentro y es un muro...
            laberinto[ny - dy[dirs[i]]][nx - dx[dirs[i]]] = CAMINO; // Tallar el camino intermedio.
            tallar(nx, ny); // Llamada recursiva para tallar desde la nueva celda.
        }
    }
}

// Método para comprobar si una coordenada está dentro de los límites del laberinto.
bool Laberinto::estaDentro(int x, int y) {
    return x > 0 && y > 0 && x < ancho - 1 && y < altura - 1; // Devuelve true si la coordenada está dentro de los límites.
}

// Método para mostrar el laberinto en la consola.
void Laberinto::mostrar() {
    for (int y = 0; y < altura; ++y) { // Itera sobre cada fila.
        for (int x = 0; x < ancho; ++x) { // Itera sobre cada columna.
            if (y == 0 && x == 1) printf("E"); // Muestra la entrada.
            else if (y == altura - 1 && x == ancho - 2) printf("S"); // Muestra la salida.
            else if (laberinto[y][x] == MURO) printf("#"); // Muestra un muro.
            else if (laberinto[y][x] == CAMINO) printf(" "); // Muestra un camino.
            else if (laberinto[y][x] == VISITADO) printf("\033[31m*\033[0m"); // Muestra una celda visitada en rojo.
        }
        printf("\n"); // Nueva línea al final de cada fila.
    }
}

// Método para resolver el laberinto usando BFS (Búsqueda en Anchura).
bool Laberinto::resolver() {
    queue<Punto> q; // Cola para BFS.
    q.push({1, 0}); // Empieza desde la entrada.
    laberinto[1][0] = VISITADO; // Marca la entrada como visitada.

    while (!q.empty()) { // Mientras haya celdas en la cola...
        Punto p = q.front(); // Toma la celda frontal.
        q.pop(); // Elimina la celda frontal de la cola.

        for (int i = 0; i < 4; ++i) { // Intenta moverse en cada dirección.
            int nx = p.x + dx[i]; // Nueva coordenada x.
            int ny = p.y + dy[i]; // Nueva coordenada y.

            if (nx == ancho - 2 && ny == altura - 1) { // Si se ha llegado a la salida...
                laberinto[ny][nx] = VISITADO; // Marca la salida como visitada.
                return true; // Laberinto resuelto.
            }

            if (estaDentro(nx, ny) && laberinto[ny][nx] == CAMINO) { // Si la nueva celda está dentro y es un camino...
                laberinto[ny][nx] = VISITADO; // Marca la celda como visitada.
                q.push({nx, ny}); // Añade la celda a la cola.
            }
        }
    }
    return false; // No se encontró solución.
}

int main() {
    int ancho, altura;
    cout << "Ingrese el ancho del laberinto (impar): "; // Solicita el ancho del laberinto al usuario.
    cin >> ancho;
    cout << "Ingrese la altura del laberinto (impar): "; // Solicita la altura del laberinto al usuario.
    cin >> altura;

    if (ancho % 2 == 0) ancho++; // Asegura que el ancho sea impar.
    if (altura % 2 == 0) altura++; // Asegura que la altura sea impar.

    auto inicio = high_resolution_clock::now(); // Marca el inicio del tiempo de generación.
    Laberinto laberinto(ancho, altura); // Crea un laberinto con el tamaño especificado.
    laberinto.generar(); // Genera el laberinto.
    auto fin = high_resolution_clock::now(); // Marca el final del tiempo de generación.
    auto duracion = duration_cast<milliseconds>(fin - inicio).count(); // Calcula la duración en milisegundos.
    cout << "Tiempo de generación: " << duracion << "ms" << endl; // Muestra el tiempo de generación.

    laberinto.mostrar(); // Muestra el laberinto generado.

    inicio = high_resolution_clock::now(); // Marca el inicio del tiempo de resolución.
    if (laberinto.resolver()) { // Intenta resolver el laberinto.
        fin = high_resolution_clock::now(); // Marca el final del tiempo de resolución.
        duracion = duration_cast<milliseconds>(fin - inicio).count(); // Calcula la duración en milisegundos.
        cout << "Laberinto resuelto en " << duracion << "ms:" << endl; // Muestra el tiempo de resolución.
        laberinto.mostrar(); // Muestra el laberinto resuelto.
    } else {
        cout << "No se pudo encontrar una solución." << endl; }// Muestra mensaje si no se encuentra solución.
    

    return 0;
}
