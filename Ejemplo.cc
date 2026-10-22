#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "PuntoGPS.h"
#include "ListaPuntosGPS.h"

using namespace std;

const double EPSILON = 1e-6;

int pruebasPasadas = 0;
int pruebasTotales = 0;

bool sonIguales(double a, double b) {
    return abs(a - b) < EPSILON;
}

// Función auxiliar para registrar e imprimir errores sin repetir código
void registrarResultado(bool correcto, const string& mensaje, const string& txtObtenido, const string& txtEsperado) {
    pruebasTotales++;
    if (correcto) {
        pruebasPasadas++;
        cout << "[OK] " << mensaje << endl;
    } else {
        cerr << "[ERROR EN PRUEBA] " << mensaje << endl;
        cerr << "   -> Obtenido: " << txtObtenido << " | Esperado: " << txtEsperado << endl;
    }
}

// ------------------------------------------------------------------
// Sobrecargas concretas de 'probar' (sin uso de plantillas C++)
// ------------------------------------------------------------------

// 1. Enteros
void probar(int obtenido, int esperado, const string& mensaje) {
    registrarResultado(obtenido == esperado, mensaje, to_string(obtenido), to_string(esperado));
}

// 2. Marcas de tiempo (unsigned long)
void probar(unsigned long obtenido, unsigned long esperado, const string& mensaje) {
    registrarResultado(obtenido == esperado, mensaje, to_string(obtenido), to_string(esperado));
}

// 3. Cadenas de texto
void probar(const string& obtenido, const string& esperado, const string& mensaje) {
    registrarResultado(obtenido == esperado, mensaje, obtenido, esperado);
}

// 4. Booleanos
void probar(bool obtenido, bool esperado, const string& mensaje) {
    string txtObt = obtenido ? "true" : "false";
    string txtEsp = esperado ? "true" : "false";
    registrarResultado(obtenido == esperado, mensaje, txtObt, txtEsp);
}

// 5. Flotantes (double) con tolerancia EPSILON
void probar(double obtenido, double esperado, const string& mensaje) {
    registrarResultado(sonIguales(obtenido, esperado), mensaje, to_string(obtenido), to_string(esperado));
}

// 6. Punteros a NodoLista
void probar(NodoLista* obtenido, NodoLista* esperado, const string& mensaje) {
    string txtObt = (obtenido == nullptr) ? "nullptr" : "puntero_valido";
    string txtEsp = (esperado == nullptr) ? "nullptr" : "puntero_valido";
    registrarResultado(obtenido == esperado, mensaje, txtObt, txtEsp);
}

// 7. Objetos PuntoGPS
void probar(const PuntoGPS& obtenido, const PuntoGPS& esperado, const string& mensaje) {
    pruebasTotales++;
    if (obtenido == esperado) {
        pruebasPasadas++;
        cout << "[OK] " << mensaje << endl;
    } else {
        cerr << "[ERROR EN PRUEBA] " << mensaje << endl;
        cerr << "   -> Obtenido:\n" << obtenido.aCadena() << endl;
        cerr << "   -> Esperado:\n" << esperado.aCadena() << endl;
    }
}

// 8. Objetos IteradorLista
void probar(const IteradorLista& obtenido, const IteradorLista& esperado, const string& mensaje) {
    string txtObt = (obtenido == IteradorLista()) ? "end/nullptr" : "iterador_valido";
    string txtEsp = (esperado == IteradorLista()) ? "end/nullptr" : "iterador_valido";
    registrarResultado(obtenido == esperado, mensaje, txtObt, txtEsp);
}

int main() {
    cout << "==========================================================" << endl;
    cout << "  Batería de Pruebas para Práctica 2: ListaPuntosGPS" << endl;
    cout << "==========================================================" << endl << endl;

    // ------------------------------------------------------------------
    // SESIÓN 1: Auxiliares (NodoLista, IteradorLista) y Estructura Básica de ListaPuntosGPS
    // ------------------------------------------------------------------
    cout << "--- SESIÓN 1: Clases Auxiliares y Estructura Básica de ListaPuntosGPS ---" << endl;
    
    PuntoGPS p1(38.3854, -0.5127, 120.5, 1000, "GARMIN-01");
    PuntoGPS p2(38.3860, -0.5120, 125.0, 1050, "GARMIN-01");
    PuntoGPS p3(38.3900, -0.5100, 130.0, 1200, "DEV-B");
    PuntoGPS p4(38.4000, -0.5000, 140.0, 1500, "GARMIN-01");

    // 1.1 Pruebas de PuntoGPS
    probar(p1.getTimestamp(), 1000UL, "PuntoGPS inicializado correctamente con timestamp 1000");

    // 1.2 Pruebas de NodoLista aisladas
    NodoLista nDef;
    probar(nDef.getSiguiente(), (NodoLista*)nullptr, "NodoLista por defecto inicializa siguiente a nullptr");
    
    NodoLista* n1 = new NodoLista(nullptr, p1);
    probar(n1->getPuntoGPS(), p1, "NodoLista sobrecargado almacena correctamente el PuntoGPS");
    probar(n1->getSiguiente(), (NodoLista*)nullptr, "NodoLista sobrecargado inicializa siguiente a nullptr");

    NodoLista* n2 = new NodoLista(n1, p2);
    probar(n2->getSiguiente(), n1, "NodoLista sobrecargado enlaza correctamente con el nodo siguiente");

    n1->setPuntoGPS(p3);
    probar(n1->getPuntoGPS(), p3, "NodoLista::setPuntoGPS modifica el punto correctamente");
    
    delete n2;
    probar(n1->getPuntoGPS(), p3, "Destructor de NodoLista no borra el nodo siguiente (memoria aislada)");
    delete n1;

    // 1.3 Pruebas de IteradorLista
    IteradorLista itDef;
    probar(itDef, IteradorLista(), "IteradorLista por defecto es equivalente a end() / nullptr");

    // 1.4 Estructura básica de ListaPuntosGPS (esVacia, longitud, begin, end, getPuntoGPS)
    ListaPuntosGPS lista;
    probar(lista.esVacia(), true, "Una lista recién creada está vacía");
    probar(lista.longitud(), 0, "Una lista recién creada tiene longitud 0");
    probar(lista.begin(), lista.end(), "En una lista vacía, begin() == end()");
    probar(lista.getPuntoGPS(lista.begin()), PuntoGPS(), "getPuntoGPS sobre iterador nulo (lista vacía) devuelve PuntoGPS por defecto");

    // Robustez: getPuntoGPS con iterador nulo explícito
    IteradorLista itNull;
    probar(lista.getPuntoGPS(itNull), PuntoGPS(), "getPuntoGPS con iterador nulo devuelve PuntoGPS por defecto");

    // 1.5 Pruebas de inserción rudimentaria y verificación progresiva de longitud
    // Estas pruebas dan por válida la inserción en cualquier posición de la lista, incluso
    // si no se respeta el orden cronológico
    cout << "--- Subpruebas Sesión 1: Inserción rudimentaria y longitud ---" << endl;
    ListaPuntosGPS listaRudimentaria;
    probar(listaRudimentaria.esVacia(), true, "Lista rudimentaria recién creada está vacía");
    probar(listaRudimentaria.longitud(), 0, "Longitud inicial de la lista es 0");

    probar(listaRudimentaria.insertar(p1), true, "Inserción rudimentaria del primer elemento");
    probar(listaRudimentaria.esVacia(), false, "esVacia() devuelve false tras insertar un elemento");
    probar(listaRudimentaria.longitud(), 1, "longitud() devuelve 1 tras la primera inserción");

    probar(listaRudimentaria.insertar(p2), true, "Inserción rudimentaria del segundo elemento");
    probar(listaRudimentaria.longitud(), 2, "longitud() devuelve 2 tras la segunda inserción");

    probar(listaRudimentaria.insertar(p3), true, "Inserción rudimentaria del tercer elemento");
    probar(listaRudimentaria.longitud(), 3, "longitud() devuelve 3 tras la tercera inserción");

    // ------------------------------------------------------------------
    // SESIÓN 2: Inserción Ordenada Completa, aCadena y Métodos de Búsqueda y Consulta
    // ------------------------------------------------------------------
    cout << endl << "--- SESIÓN 2: Inserción Ordenada, aCadena y Métodos de Búsqueda ---" << endl;

    // 2.1 Inserción manteniendo orden por timestamp
    probar(lista.insertar(p3), true, "Inserta correctamente el primer punto (p3, ts: 1200)");
    probar(lista.esVacia(), false, "La lista ya no está vacía tras la primera inserción");
    probar(lista.longitud(), 1, "La longitud de la lista es 1");

    // Robustez: getPuntoGPS con iterador nulo en lista NO vacía
    probar(lista.getPuntoGPS(itNull), PuntoGPS(), "getPuntoGPS con iterador nulo en lista con elementos devuelve PuntoGPS por defecto");

    probar(lista.insertar(p1), true, "Inserta un punto con timestamp menor al principio (p1, ts: 1000)");
    probar(lista.longitud(), 2, "La longitud de la lista es 2");

    probar(lista.insertar(p4), true, "Inserta un punto con timestamp mayor al final (p4, ts: 1500)");
    probar(lista.longitud(), 3, "La longitud de la lista es 3");

    probar(lista.insertar(p2), true, "Inserta un punto en posición intermedia (p2, ts: 1050)");
    probar(lista.longitud(), 4, "La longitud de la lista es 4");

    // Restricción de clave única (timestamps duplicados)
    PuntoGPS pDuplicado(39.0000, -0.4000, 200.0, 1050, "OTRO-DEV");
    probar(lista.insertar(pDuplicado), false, "insertar devuelve false al intentar insertar un timestamp duplicado (1050)");
    probar(lista.longitud(), 4, "La longitud de la lista se mantiene intacta tras el intento duplicado");

    // Recorrido secuencial con IteradorLista
    IteradorLista itRec = lista.begin();
    probar(lista.getPuntoGPS(itRec).getTimestamp(), 1000UL, "El primer nodo es p1 (ts: 1000)");
    itRec.step();
    probar(lista.getPuntoGPS(itRec).getTimestamp(), 1050UL, "El segundo nodo es p2 (ts: 1050)");
    itRec.step();
    probar(lista.getPuntoGPS(itRec).getTimestamp(), 1200UL, "El tercer nodo es p3 (ts: 1200)");
    itRec.step();
    probar(lista.getPuntoGPS(itRec).getTimestamp(), 1500UL, "El cuarto nodo es p4 (ts: 1500)");
    itRec.step();
    probar(itRec, lista.end(), "it.step() desde el último elemento alcanza lista.end()");

    // 2.2 Método aCadena()
    string cadenaEsperada = p1.aCadena() + "\n" + p2.aCadena() + "\n" + p3.aCadena() + "\n" + p4.aCadena();
    probar(lista.aCadena(), cadenaEsperada, "aCadena() genera la representación multilínea ordenada sin salto final");

    // 2.3 Métodos de búsqueda y consulta (existeTimestamp, buscarPorTimestamp, contarMismaUbicacion)
    probar(lista.existeTimestamp(1050), true, "existeTimestamp devuelve true para un timestamp existente (1050)");
    probar(lista.existeTimestamp(9999), false, "existeTimestamp devuelve false para un timestamp inexistente (9999)");

    IteradorLista itBusqueda = lista.buscarPorTimestamp(1200);
    probar(itBusqueda != lista.end() && lista.getPuntoGPS(itBusqueda).getDispositivo() == "DEV-B", true, 
           "buscarPorTimestamp devuelve el iterador correcto al punto buscado");

    probar(lista.buscarPorTimestamp(9999), lista.end(), "buscarPorTimestamp devuelve end() si no existe el timestamp");

    // Probar contarMismaUbicacion con punto coincidente (misma posición que p1 pero ts 2000)
    PuntoGPS p5(38.3854 + 1e-7, -0.5127 - 1e-7, 120.5, 2000, "GARMIN-02");
    lista.insertar(p5);

    probar(lista.contarMismaUbicacion(p1), 2, "contarMismaUbicacion cuenta 2 puntos en la misma ubicación (p1 y p5 con tolerancia 1e-6)");
    probar(lista.contarMismaUbicacion(p2), 1, "contarMismaUbicacion cuenta 1 punto para la ubicación de p2");
    PuntoGPS pUbicacionLejana(40.0000, -3.0000, 600.0, 3000, "DEV-X");
    probar(lista.contarMismaUbicacion(pUbicacionLejana), 0, "contarMismaUbicacion devuelve 0 para una ubicación sin visitas en la lista");

    // ------------------------------------------------------------------
    // SESIÓN 3: Métodos de Eliminación y Gestión de Memoria / Copia Profunda
    // ------------------------------------------------------------------
    cout << endl << "--- SESIÓN 3: Métodos de Eliminación y Copia Profunda ---" << endl;

    // 3.1 Pruebas de borrarPrimero() y borrarUltimo()
    // La lista contiene: 1000 (p1), 1050 (p2), 1200 (p3), 1500 (p4), 2000 (p5)
    probar(lista.borrarPrimero(), true, "borrarPrimero elimina el primer elemento (p1, ts: 1000)");
    probar(lista.longitud(), 4, "Longitud es 4 tras borrar el primero");
    probar(lista.getPuntoGPS(lista.begin()).getTimestamp(), 1050UL, "El nuevo primer elemento es p2 (ts: 1050)");

    probar(lista.borrarUltimo(), true, "borrarUltimo elimina el último elemento (p5, ts: 2000)");
    probar(lista.longitud(), 3, "Longitud es 3 tras borrar el último");

    // 3.2 Pruebas exhaustivas de borrarPorTimestamp()
    cout << "--- Subpruebas: borrarPorTimestamp ---" << endl;
    {
        // Caso A: Lista con un único elemento
        ListaPuntosGPS lUnica;
        lUnica.insertar(p1); // ts 1000
        probar(lUnica.borrarPorTimestamp(1000), true, "borrarPorTimestamp en lista de un único elemento devuelve true");
        probar(lUnica.esVacia(), true, "Lista queda vacía tras borrar su único elemento por timestamp");
        probar(lUnica.borrarPorTimestamp(1000), false, "borrarPorTimestamp devuelve false en lista recién vaciada");

        // Caso B: Cabeza, nodo intermedio y cola en lista de varios elementos
        ListaPuntosGPS lTres;
        lTres.insertar(p1); // 1000
        lTres.insertar(p2); // 1050
        lTres.insertar(p3); // 1200

        // Borrar al principio (cabeza)
        probar(lTres.borrarPorTimestamp(1000), true, "borrarPorTimestamp borra correctamente el primer nodo (cabeza)");
        probar(lTres.getPuntoGPS(lTres.begin()).getTimestamp(), 1050UL, "El nuevo primer nodo tras borrarPorTimestamp es ts 1050");

        // Restablecer y borrar en punto intermedio
        lTres.insertar(p1); // 1000, 1050, 1200
        probar(lTres.borrarPorTimestamp(1050), true, "borrarPorTimestamp borra correctamente un nodo intermedio");
        probar(lTres.longitud(), 2, "La longitud es 2 tras borrar un nodo intermedio");
        probar(lTres.existeTimestamp(1050), false, "El timestamp 1050 ya no existe en la lista");

        // Borrar al final (cola)
        probar(lTres.borrarPorTimestamp(1200), true, "borrarPorTimestamp borra correctamente el último nodo (cola)");
        probar(lTres.longitud(), 1, "La longitud es 1 tras borrar el último nodo por timestamp");
        probar(lTres.getPuntoGPS(lTres.begin()).getTimestamp(), 1000UL, "Solo queda el nodo con ts 1000");
    }

    // 3.3 Pruebas exhaustivas de borrar(IteradorLista&)
    cout << "--- Subpruebas: borrar(IteradorLista&) ---" << endl;
    {
        // Robustez: iterador nulo o end()
        ListaPuntosGPS lTres;
        lTres.insertar(p1);
        lTres.insertar(p2);
        lTres.insertar(p3);

        IteradorLista itNull;
        probar(lTres.borrar(itNull), false, "borrar(iterador) devuelve false cuando el iterador apunta a nullptr (nulo)");
        probar(lTres.longitud(), 3, "La longitud de la lista no cambia tras intentar borrar un iterador nulo");

        IteradorLista itEnd = lTres.end();
        probar(lTres.borrar(itEnd), false, "borrar(iterador) devuelve false cuando recibe end()");

        // Caso A: Lista con un único elemento
        ListaPuntosGPS lUnica;
        lUnica.insertar(p1);
        IteradorLista it = lUnica.begin();
        probar(lUnica.borrar(it), true, "borrar(iterador) en lista de un único elemento devuelve true");
        probar(lUnica.esVacia(), true, "Lista queda vacía tras borrar su único elemento con iterador");
        probar(it, lUnica.end(), "El iterador pasa a ser nullptr (end) tras borrar el único elemento");

        // Caso B: Cabeza, intermedio y cola en lista de varios elementos
        IteradorLista itIni = lTres.begin();
        probar(lTres.borrar(itIni), true, "borrar(iterador) borra el primer nodo (cabeza)");
        probar(lTres.getPuntoGPS(lTres.begin()).getTimestamp(), 1050UL, "El nuevo primer nodo tras borrar por iterador es ts 1050");
        probar(itIni, lTres.end(), "El iterador inicial pasa a ser end() tras ser borrado");

        // Reconstruir lista y borrar intermedio
        lTres.insertar(p1); // 1000, 1050, 1200
        IteradorLista itMid = lTres.begin();
        itMid.step(); // apunta a p2 (1050)
        probar(lTres.borrar(itMid), true, "borrar(iterador) borra un nodo intermedio");
        probar(lTres.existeTimestamp(1050), false, "El nodo intermedio borrado ya no existe");
        probar(itMid, lTres.end(), "El iterador intermedio pasa a ser end() tras borrado");

        // Borrar al final (cola con iterador)
        IteradorLista itFin = lTres.begin();
        itFin.step(); // apunta a p3 (1200)
        probar(lTres.borrar(itFin), true, "borrar(iterador) borra el último nodo (cola)");
        probar(lTres.longitud(), 1, "La longitud es 1 tras borrar la cola");
        probar(itFin, lTres.end(), "El iterador final pasa a ser end() tras borrado");
    }

    // 3.4 Copia Profunda (Constructor de copia, operator= y destructor)
    cout << "--- Subpruebas: Copia Profunda y Asignación ---" << endl;
    {
        ListaPuntosGPS lBase;
        lBase.insertar(p1);
        lBase.insertar(p2);
        lBase.insertar(p4);

        ListaPuntosGPS listaCopia(lBase);
        probar(listaCopia.longitud(), lBase.longitud(), "El constructor de copia crea una lista de la misma longitud");
        probar(listaCopia.aCadena(), lBase.aCadena(), "La lista copiada tiene idéntico contenido en aCadena()");

        listaCopia.borrarPrimero();
        probar(listaCopia.longitud() != lBase.longitud(), true, "La modificación de la lista copiada no afecta a la original (copia profunda)");

        ListaPuntosGPS listaAsignada;
        listaAsignada = lBase;
        probar(listaAsignada.longitud(), lBase.longitud(), "El operador de asignación realiza una copia correcta de la lista");
        listaAsignada = listaAsignada;
        probar(listaAsignada.longitud(), lBase.longitud(), "La auto-asignación (obj = obj) mantiene la integridad de la lista");
    }

    // ------------------------------------------------------------------
    // SESIÓN 4: Método Asignar, Filtros y Métodos Globales de Trayectoria
    // ------------------------------------------------------------------
    cout << endl << "--- SESIÓN 4: Método Asignar, Filtros y Métodos de Trayectoria ---" << endl;

    // 4.1 Pruebas exhaustivas de asignar(IteradorLista, const PuntoGPS&)
    cout << "--- Subpruebas: asignar ---" << endl;
    {
        ListaPuntosGPS lAsig;
        lAsig.insertar(p1); // 1000
        lAsig.insertar(p2); // 1050
        lAsig.insertar(p4); // 1500

        // Robustez: iterador nulo o end()
        IteradorLista itNull;
        PuntoGPS pPrueba(38.0, -0.5, 100.0, 1100, "PRUEBA");
        probar(lAsig.asignar(itNull, pPrueba), false, "asignar devuelve false cuando recibe un iterador nulo");

        IteradorLista itEnd = lAsig.end();
        probar(lAsig.asignar(itEnd, pPrueba), false, "asignar devuelve false cuando recibe el iterador end()");

        // Caso A: Sin nodo anterior (nodo Cabeza en lista de varios elementos)
        IteradorLista itHead = lAsig.begin(); // apunta a 1000 (sin nodo anterior)
        PuntoGPS pHeadValido(38.0, -0.5, 100.0, 500, "HEAD-OK");
        probar(lAsig.asignar(itHead, pHeadValido), true, "asignar en cabeza modifica correctamente con ts menor al siguiente (500 < 1050)");
        probar(lAsig.getPuntoGPS(itHead).getTimestamp(), 500UL, "El timestamp en cabeza se actualizó correctamente a 500");

        PuntoGPS pHeadInvalido(38.0, -0.5, 100.0, 1200, "HEAD-BAD");
        probar(lAsig.asignar(itHead, pHeadInvalido), false, "asignar en cabeza devuelve false si el nuevo ts viola el orden con el siguiente (1200 > 1050)");

        // Caso B: Sin nodo siguiente (nodo Cola en lista de varios elementos)
        IteradorLista itTail = lAsig.begin();
        itTail.step(); // 1050
        itTail.step(); // 1500 (sin nodo siguiente)

        PuntoGPS pTailValido(38.0, -0.5, 100.0, 2000, "TAIL-OK");
        probar(lAsig.asignar(itTail, pTailValido), true, "asignar en cola modifica correctamente con ts mayor al anterior (2000 > 1050)");
        probar(lAsig.getPuntoGPS(itTail).getTimestamp(), 2000UL, "El timestamp en cola se actualizó correctamente a 2000");

        PuntoGPS pTailInvalido(38.0, -0.5, 100.0, 800, "TAIL-BAD");
        probar(lAsig.asignar(itTail, pTailInvalido), false, "asignar en cola devuelve false si el nuevo ts viola el orden con el anterior (800 < 1050)");

        // Caso C: Con nodo anterior y siguiente (nodo Intermedio)
        IteradorLista itMid = lAsig.begin();
        itMid.step(); // apunta a 1050 (anterior: 500, siguiente: 2000)

        PuntoGPS pMidValido(38.0, -0.5, 100.0, 1000, "MID-OK");
        probar(lAsig.asignar(itMid, pMidValido), true, "asignar en nodo intermedio modifica con ts válido (500 < 1000 < 2000)");

        PuntoGPS pMidMenorAnterior(38.0, -0.5, 100.0, 400, "MID-BAD1");
        probar(lAsig.asignar(itMid, pMidMenorAnterior), false, "asignar en nodo intermedio devuelve false si ts < anterior (400 < 500)");

        PuntoGPS pMidMayorSiguiente(38.0, -0.5, 100.0, 2500, "MID-BAD2");
        probar(lAsig.asignar(itMid, pMidMayorSiguiente), false, "asignar en nodo intermedio devuelve false si ts > siguiente (2500 > 2000)");

        // Caso D: Sin anterior ni siguiente (Único elemento en la lista)
        ListaPuntosGPS lUnicoAsig;
        lUnicoAsig.insertar(p1); // ts 1000
        IteradorLista itUnico = lUnicoAsig.begin();
        PuntoGPS pUnicoNuevo(38.0, -0.5, 100.0, 9999, "SOLO-OK");
        probar(lUnicoAsig.asignar(itUnico, pUnicoNuevo), true, "asignar en lista de un único elemento modifica sin restricciones de orden");
        probar(lUnicoAsig.getPuntoGPS(itUnico).getTimestamp(), 9999UL, "El timestamp del único elemento se actualizó correctamente a 9999");
    }

    // 4.2 Métodos de Filtrado (filtrarRangoTimestamp, filtrarDispositivo)
    cout << "--- Subpruebas: Métodos de Filtrado ---" << endl;
    {
        ListaPuntosGPS trayecto;
        trayecto.insertar(p1); // 1000 (GARMIN-01)
        trayecto.insertar(p2); // 1050 (GARMIN-01)
        trayecto.insertar(p3); // 1200 (DEV-B)
        trayecto.insertar(p4); // 1500 (GARMIN-01)
        trayecto.insertar(p5); // 2000 (GARMIN-02)

        ListaPuntosGPS filtroRango = trayecto.filtrarRangoTimestamp(1020, 1300);
        probar(filtroRango.longitud(), 2, "filtrarRangoTimestamp [1020, 1300] devuelve exactamente 2 puntos (1050 y 1200)");
        probar(filtroRango.existeTimestamp(1050) && filtroRango.existeTimestamp(1200), true, "Los puntos filtrados por rango coinciden con los esperados");

        ListaPuntosGPS filtroDev = trayecto.filtrarDispositivo("GARMIN-01");
        probar(filtroDev.longitud(), 3, "filtrarDispositivo('GARMIN-01') devuelve exactamente 3 puntos");
        probar(filtroDev.contarMismaUbicacion(p3), 0, "El punto de DEV-B fue excluido correctamente en la lista filtrada por dispositivo");

        // 4.3 Métodos globales sobre la trayectoria completa
        double dist12 = p1.distancia3D(p2);
        double dist23 = p2.distancia3D(p3);
        double dist34 = p3.distancia3D(p4);
        double dist45 = p4.distancia3D(p5);
        double distTotalEsperada = dist12 + dist23 + dist34 + dist45;

        probar(trayecto.distanciaTotal(), distTotalEsperada, "distanciaTotal() coincide exactamente con la suma de tramos 3D");

        double velMediaEsperada = distTotalEsperada / (2000.0 - 1000.0);
        probar(trayecto.velocidadMedia(), velMediaEsperada, "velocidadMedia() calcula correctamente distanciaTotal / (ts_fin - ts_inicio)");

        double v12 = p1.velocidad(p2);
        double v23 = p2.velocidad(p3);
        double v34 = p3.velocidad(p4);
        double v45 = p4.velocidad(p5);
        double velMaxEsperada = max({v12, v23, v34, v45});

        probar(trayecto.velocidadMaxima(), velMaxEsperada, "velocidadMaxima() identifica correctamente el tramo con velocidad más alta");

        // Casos frontera (lista con 1 solo elemento)
        ListaPuntosGPS trayectoUnico;
        trayectoUnico.insertar(p1);
        probar(trayectoUnico.distanciaTotal(), 0.0, "distanciaTotal en lista con 1 solo elemento devuelve 0.0");
        probar(trayectoUnico.velocidadMedia(), 0.0, "velocidadMedia en lista con 1 solo elemento devuelve 0.0");
        probar(trayectoUnico.velocidadMaxima(), 0.0, "velocidadMaxima en lista con 1 solo elemento devuelve 0.0");
    }

    cout << endl << "==========================================================" << endl;
    cout << "  Resumen: " << pruebasPasadas << " de " << pruebasTotales << " pruebas pasadas correctamente." << endl;
    cout << "==========================================================" << endl;

    return 0;
}
