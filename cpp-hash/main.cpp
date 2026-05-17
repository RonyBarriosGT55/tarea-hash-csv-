#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;


const string NOMBRE_COMPLETO = "RONY BARRIOS";
const string CARNE = "9941 99 127";

const string ARCHIVO_CSV = "estudiantes.csv";

struct Estudiante {
    int student_id;
    string full_name;
    string career;
    int semester;
    float gpa;
    int skill_score;
};

struct Nodo {
    Estudiante estudiante;
    Nodo* siguiente;

    Nodo(Estudiante e) {
        estudiante = e;
        siguiente = nullptr;
    }
};

string limpiarTexto(string texto) {
    while (!texto.empty() && (texto[0] == ' ' || texto[0] == '\t' || texto[0] == '\r' || texto[0] == '\n')) {
        texto.erase(0, 1);
    }

    while (!texto.empty() && (texto[texto.length() - 1] == ' ' || texto[texto.length() - 1] == '\t' || texto[texto.length() - 1] == '\r' || texto[texto.length() - 1] == '\n')) {
        texto.erase(texto.length() - 1, 1);
    }

    if (!texto.empty() && texto[0] == '\xEF') {
        // Limpieza preventiva de BOM UTF-8 si aparece al inicio del CSV.
        if (texto.length() >= 3) {
            texto.erase(0, 3);
        }
    }

    return texto;
}

bool convertirLineaCSV(const string& linea, Estudiante& estudiante) {
    if (linea.empty()) {
        return false;
    }

    string campos[6];
    int indiceCampo = 0;

    for (int i = 0; i < (int)linea.length(); i++) {
        char caracter = linea[i];

        if (caracter == ',') {
            indiceCampo++;
            if (indiceCampo >= 6) {
                return false;
            }
        } else {
            campos[indiceCampo] += caracter;
        }
    }

    if (indiceCampo != 5) {
        return false;
    }

    for (int i = 0; i < 6; i++) {
        campos[i] = limpiarTexto(campos[i]);
    }

    try {
        estudiante.student_id = stoi(campos[0]);
        estudiante.full_name = campos[1];
        estudiante.career = campos[2];
        estudiante.semester = stoi(campos[3]);
        estudiante.gpa = stof(campos[4]);
        estudiante.skill_score = stoi(campos[5]);
    } catch (...) {
        return false;
    }

    return true;
}

void mostrarEstudiante(const Estudiante& e) {
    cout << "ID: " << e.student_id
         << " | Nombre: " << e.full_name
         << " | Carrera: " << e.career
         << " | Semestre: " << e.semester
         << " | GPA: " << fixed << setprecision(2) << e.gpa
         << " | Skill Score: " << e.skill_score << endl;
}

class HashTable {
private:
    static const int SIZE = 17;
    Nodo* tabla[SIZE];
    int totalEstudiantes;
    int totalColisiones;

    int funcionHash(int clave) {
        if (clave < 0) {
            clave = clave * -1;
        }
        return clave % SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < SIZE; i++) {
            tabla[i] = nullptr;
        }
        totalEstudiantes = 0;
        totalColisiones = 0;
    }

    ~HashTable() {
        liberarMemoria();
    }

    bool insertar(const Estudiante& estudiante) {
        if (buscar(estudiante.student_id) != nullptr) {
            return false;
        }

        int indice = funcionHash(estudiante.student_id);
        Nodo* nuevo = new Nodo(estudiante);

        if (tabla[indice] == nullptr) {
            tabla[indice] = nuevo;
        } else {
            totalColisiones++;
            Nodo* actual = tabla[indice];
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }

        totalEstudiantes++;
        return true;
    }

    Estudiante* buscar(int student_id) {
        int indice = funcionHash(student_id);
        Nodo* actual = tabla[indice];

        while (actual != nullptr) {
            if (actual->estudiante.student_id == student_id) {
                return &(actual->estudiante);
            }
            actual = actual->siguiente;
        }

        return nullptr;
    }

    bool eliminar(int student_id) {
        int indice = funcionHash(student_id);
        Nodo* actual = tabla[indice];
        Nodo* anterior = nullptr;

        while (actual != nullptr) {
            if (actual->estudiante.student_id == student_id) {
                if (anterior == nullptr) {
                    tabla[indice] = actual->siguiente;
                } else {
                    anterior->siguiente = actual->siguiente;
                }

                delete actual;
                totalEstudiantes--;
                return true;
            }

            anterior = actual;
            actual = actual->siguiente;
        }

        return false;
    }

    void mostrarTabla() {
        cout << "\n========== CONTENIDO DE LA TABLA HASH POR BUCKET ==========" << endl;
        for (int i = 0; i < SIZE; i++) {
            cout << "Bucket [" << setw(2) << i << "]: ";

            Nodo* actual = tabla[i];
            if (actual == nullptr) {
                cout << "vacio";
            }

            while (actual != nullptr) {
                cout << "(" << actual->estudiante.student_id << " - " << actual->estudiante.full_name << ")";
                if (actual->siguiente != nullptr) {
                    cout << " -> ";
                }
                actual = actual->siguiente;
            }
            cout << endl;
        }
    }

    void mostrarEstadisticas() {
        cout << "\n========== ESTADISTICAS DE LA TABLA HASH ==========" << endl;
        cout << "Tamanio de la tabla: " << SIZE << endl;
        cout << "Total de estudiantes cargados: " << totalEstudiantes << endl;
        cout << "Total de colisiones generadas: " << totalColisiones << endl;
        cout << "Factor de carga: " << fixed << setprecision(2) << obtenerFactorCarga() << endl;
    }

    float obtenerFactorCarga() {
        return (float)totalEstudiantes / SIZE;
    }

    int obtenerTotalEstudiantes() {
        return totalEstudiantes;
    }

    int obtenerTotalColisiones() {
        return totalColisiones;
    }

    int obtenerSize() {
        return SIZE;
    }

    void liberarMemoria() {
        for (int i = 0; i < SIZE; i++) {
            Nodo* actual = tabla[i];

            while (actual != nullptr) {
                Nodo* temp = actual;
                actual = actual->siguiente;
                delete temp;
            }

            tabla[i] = nullptr;
        }

        totalEstudiantes = 0;
        totalColisiones = 0;
    }
};

void mostrarEncabezado() {
    cout << "==========================================================" << endl;
    cout << " TAREA HASH CSV - IMPLEMENTACION C++ MANUAL" << endl;
    cout << "==========================================================" << endl;
    cout << "Estudiante: " << NOMBRE_COMPLETO << endl;
    cout << "Carne: " << CARNE << endl;
    cout << "Estructura: Tabla Hash manual con Separate Chaining" << endl;
    cout << "Archivo CSV esperado: " << ARCHIVO_CSV << endl;
    cout << "==========================================================" << endl;
}

void pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

int leerEntero(string mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;

        if (!cin.fail()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return valor;
        }

        cout << "Entrada invalida. Intente de nuevo." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void cargarDesdeCSV(HashTable& tabla) {
    ifstream archivo(ARCHIVO_CSV.c_str());

    if (!archivo.is_open()) {
        cout << "\nNo se pudo abrir el archivo: " << ARCHIVO_CSV << endl;
        cout << "Verifique que el CSV este en la misma carpeta que el ejecutable." << endl;
        return;
    }

    string linea;
    int lineaActual = 0;
    int insertados = 0;
    int duplicados = 0;
    int invalidos = 0;

    while (getline(archivo, linea)) {
        lineaActual++;

        if (lineaActual == 1) {
            continue;
        }

        Estudiante estudiante;
        bool valido = convertirLineaCSV(linea, estudiante);

        if (!valido) {
            invalidos++;
            continue;
        }

        bool insertado = tabla.insertar(estudiante);
        if (insertado) {
            insertados++;
        } else {
            duplicados++;
        }
    }

    archivo.close();

    cout << "\n========== RESULTADO DE CARGA CSV ==========" << endl;
    cout << "Archivo procesado: " << ARCHIVO_CSV << endl;
    cout << "Registros insertados: " << insertados << endl;
    cout << "Registros duplicados no insertados: " << duplicados << endl;
    cout << "Lineas invalidas ignoradas: " << invalidos << endl;
    tabla.mostrarEstadisticas();
}

void buscarEstudiante(HashTable& tabla) {
    int id = leerEntero("Ingrese el student_id a buscar: ");
    Estudiante* encontrado = tabla.buscar(id);

    cout << "\n========== RESULTADO DE BUSQUEDA ==========" << endl;
    if (encontrado != nullptr) {
        cout << "Estudiante encontrado:" << endl;
        mostrarEstudiante(*encontrado);
    } else {
        cout << "No se encontro estudiante con ID " << id << endl;
    }
}

void eliminarEstudiante(HashTable& tabla) {
    int id = leerEntero("Ingrese el student_id a eliminar: ");
    bool eliminado = tabla.eliminar(id);

    cout << "\n========== RESULTADO DE ELIMINACION ==========" << endl;
    if (eliminado) {
        cout << "Estudiante con ID " << id << " eliminado correctamente." << endl;
    } else {
        cout << "No se encontro estudiante con ID " << id << ". No se elimino ningun registro." << endl;
    }
}

void mostrarMenu() {
    cout << "\n==================== MENU C++ ====================" << endl;
    cout << "1. Cargar estudiantes desde CSV" << endl;
    cout << "2. Mostrar contenido de la tabla hash por bucket" << endl;
    cout << "3. Buscar estudiante por student_id" << endl;
    cout << "4. Eliminar estudiante por student_id" << endl;
    cout << "5. Mostrar estadisticas de la tabla hash" << endl;
    cout << "0. Salir" << endl;
    cout << "==================================================" << endl;
}

int main() {
    HashTable tabla;
    int opcion;

    mostrarEncabezado();

    do {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1:
                cargarDesdeCSV(tabla);
                pausar();
                break;
            case 2:
                tabla.mostrarTabla();
                pausar();
                break;
            case 3:
                buscarEstudiante(tabla);
                pausar();
                break;
            case 4:
                eliminarEstudiante(tabla);
                pausar();
                break;
            case 5:
                tabla.mostrarEstadisticas();
                pausar();
                break;
            case 0:
                cout << "\nSaliendo del programa C++..." << endl;
                break;
            default:
                cout << "\nOpcion invalida. Intente nuevamente." << endl;
                pausar();
                break;
        }

    } while (opcion != 0);

    return 0;
}
