import java.io.BufferedReader;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class Main {

    private static final String NOMBRE_COMPLETO = "RONY ALEXANDER BARRIOS";
    private static final String CARNE = "9941 99 127";

    private static final String ARCHIVO_CSV = "estudiantes.csv";
    private static final Scanner scanner = new Scanner(System.in);
    private static final HashMap<Integer, Estudiante> estudiantes = new HashMap<>();

    public static void main(String[] args) {
        int opcion;

        mostrarEncabezado();

        do {
            mostrarMenu();
            opcion = leerEntero("Seleccione una opcion: ");

            switch (opcion) {
                case 1:
                    cargarDesdeCSV();
                    pausar();
                    break;
                case 2:
                    mostrarContenidoHashMap();
                    pausar();
                    break;
                case 3:
                    buscarEstudiante();
                    pausar();
                    break;
                case 4:
                    eliminarEstudiante();
                    pausar();
                    break;
                case 5:
                    mostrarEstadisticas();
                    pausar();
                    break;
                case 6:
                    mostrarComparacion();
                    pausar();
                    break;
                case 0:
                    System.out.println("\nSaliendo del programa Java...");
                    break;
                default:
                    System.out.println("\nOpcion invalida. Intente nuevamente.");
                    pausar();
                    break;
            }
        } while (opcion != 0);
    }

    private static void mostrarEncabezado() {
        System.out.println("==========================================================");
        System.out.println(" TAREA HASH CSV - IMPLEMENTACION JAVA CON HASHMAP");
        System.out.println("==========================================================");
        System.out.println("Estudiante: " + NOMBRE_COMPLETO);
        System.out.println("Carne: " + CARNE);
        System.out.println("Estructura utilizada: " + estudiantes.getClass().getName());
        System.out.println("Tipo declarado: HashMap<Integer, Estudiante>");
        System.out.println("Archivo CSV esperado: " + ARCHIVO_CSV);
        System.out.println("==========================================================");
    }

    private static void mostrarMenu() {
        System.out.println("\n==================== MENU JAVA ====================");
        System.out.println("1. Cargar estudiantes desde CSV");
        System.out.println("2. Mostrar contenido del HashMap");
        System.out.println("3. Buscar estudiante por student_id");
        System.out.println("4. Eliminar estudiante por student_id");
        System.out.println("5. Mostrar total de estudiantes cargados");
        System.out.println("6. Comparar C++ manual vs Java HashMap");
        System.out.println("0. Salir");
        System.out.println("===================================================");
    }

    private static int leerEntero(String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String entrada = scanner.nextLine();

            try {
                return Integer.parseInt(entrada.trim());
            } catch (NumberFormatException e) {
                System.out.println("Entrada invalida. Intente de nuevo.");
            }
        }
    }

    private static void pausar() {
        System.out.print("\nPresione ENTER para continuar...");
        scanner.nextLine();
    }

    private static Path obtenerRutaCSV() {
        Path ruta1 = Paths.get("estudiantes.csv");
        Path ruta2 = Paths.get("src", "estudiantes.csv");
        Path ruta3 = Paths.get("java-hashmap", "estudiantes.csv");

        if (Files.exists(ruta1)) {
            return ruta1;
        }

        if (Files.exists(ruta2)) {
            return ruta2;
        }

        if (Files.exists(ruta3)) {
            return ruta3;
        }

        return ruta1;
    }

    private static void cargarDesdeCSV() {
        int insertados = 0;
        int duplicados = 0;
        int invalidos = 0;
        int lineaActual = 0;

        Path rutaCSV = obtenerRutaCSV();

        try (BufferedReader br = Files.newBufferedReader(rutaCSV, StandardCharsets.UTF_8)) {
            String linea;

            while ((linea = br.readLine()) != null) {
                lineaActual++;

                if (lineaActual == 1) {
                    continue;
                }

                Estudiante estudiante = convertirLineaCSV(linea);

                if (estudiante == null) {
                    invalidos++;
                    continue;
                }

                if (estudiantes.containsKey(estudiante.getStudentId())) {
                    duplicados++;
                } else {
                    estudiantes.put(estudiante.getStudentId(), estudiante);
                    insertados++;
                }
            }

            System.out.println("\n========== RESULTADO DE CARGA CSV ==========");
            System.out.println("Archivo procesado: " + rutaCSV.toAbsolutePath());
            System.out.println("Registros insertados: " + insertados);
            System.out.println("Registros duplicados no insertados: " + duplicados);
            System.out.println("Lineas invalidas ignoradas: " + invalidos);
            mostrarEstadisticas();

        } catch (IOException e) {
            System.out.println("\nNo se pudo abrir el archivo: " + ARCHIVO_CSV);
            System.out.println("Java lo esta buscando aqui:");
            System.out.println(Paths.get("estudiantes.csv").toAbsolutePath());
            System.out.println(Paths.get("src", "estudiantes.csv").toAbsolutePath());
            System.out.println(Paths.get("java-hashmap", "estudiantes.csv").toAbsolutePath());
            System.out.println("\nColoque estudiantes.csv en una de esas rutas.");
        }
    }

    private static Estudiante convertirLineaCSV(String linea) {
        if (linea == null || linea.trim().isEmpty()) {
            return null;
        }

        linea = linea.replace("\uFEFF", "");
        String[] campos = linea.split(",", -1);

        if (campos.length != 6) {
            return null;
        }

        try {
            int studentId = Integer.parseInt(campos[0].trim());
            String fullName = campos[1].trim();
            String career = campos[2].trim();
            int semester = Integer.parseInt(campos[3].trim());
            double gpa = Double.parseDouble(campos[4].trim());
            int skillScore = Integer.parseInt(campos[5].trim());

            return new Estudiante(studentId, fullName, career, semester, gpa, skillScore);
        } catch (NumberFormatException e) {
            return null;
        }
    }

    private static void mostrarContenidoHashMap() {
        System.out.println("\n========== CONTENIDO DEL HASHMAP ==========");
        System.out.println("Evidencia de estructura utilizada: " + estudiantes.getClass().getName());

        if (estudiantes.isEmpty()) {
            System.out.println("El HashMap esta vacio. Primero cargue el CSV.");
            return;
        }

        for (Map.Entry<Integer, Estudiante> entrada : estudiantes.entrySet()) {
            System.out.println("Clave: " + entrada.getKey() + " -> " + entrada.getValue());
        }
    }

    private static void buscarEstudiante() {
        int id = leerEntero("Ingrese el student_id a buscar: ");
        Estudiante encontrado = estudiantes.get(id);

        System.out.println("\n========== RESULTADO DE BUSQUEDA ==========");
        if (encontrado != null) {
            System.out.println("Estudiante encontrado:");
            System.out.println(encontrado);
        } else {
            System.out.println("No se encontro estudiante con ID " + id);
        }
    }

    private static void eliminarEstudiante() {
        int id = leerEntero("Ingrese el student_id a eliminar: ");
        Estudiante eliminado = estudiantes.remove(id);

        System.out.println("\n========== RESULTADO DE ELIMINACION ==========");
        if (eliminado != null) {
            System.out.println("Estudiante eliminado correctamente:");
            System.out.println(eliminado);
        } else {
            System.out.println("No se encontro estudiante con ID " + id + ". No se elimino ningun registro.");
        }
    }

    private static void mostrarEstadisticas() {
        System.out.println("\n========== ESTADISTICAS JAVA ==========");
        System.out.println("Estructura utilizada: HashMap<Integer, Estudiante>");
        System.out.println("Clase real en memoria: " + estudiantes.getClass().getName());
        System.out.println("Total de estudiantes cargados: " + estudiantes.size());
    }

    private static void mostrarComparacion() {
        System.out.println("\n========== COMPARACION C++ MANUAL VS JAVA HASHMAP ==========");
        System.out.println("C++: se implementa manualmente la tabla hash, los buckets y las listas enlazadas.");
        System.out.println("C++: el programador controla la memoria con new/delete y calcula colisiones/factor de carga.");
        System.out.println("Java: HashMap ya incluye la tabla hash, manejo interno de colisiones y administracion de memoria.");
        System.out.println("Java: permite concentrarse mas en la logica del negocio y menos en la estructura interna.");
    }
}