#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include "PuntoGPS.h"

using namespace std;

const double EPSILON = 1e-6;

bool sonIguales(double a, double b) {
    return abs(a - b) < EPSILON;
}

// Función de comprobación simple
void probar(bool condicion, const string& mensaje) {
    if (!condicion) {
        cerr << "[ERROR EN PRUEBA] " << mensaje << endl;
        exit(EXIT_FAILURE);
    } else {
        cout << "[OK] " << mensaje << endl;
    }
}

int main() {
    cout << "Iniciando batería completa de pruebas para PuntoGPS..." << endl << endl;

    // ------------------------------------------------------------------
    // 1. Constructores y validaciones de rango
    // ------------------------------------------------------------------
    cout << "--- 1. Pruebas de Constructores y Validaciones ---" << endl;
    PuntoGPS pDef;
    probar(pDef.getLatitud() == 0.0, "El constructor por defecto inicializa la latitud a 0.0");
    probar(pDef.getLongitud() == 0.0, "El constructor por defecto inicializa la longitud a 0.0");
    probar(pDef.getAltitud() == 0.0, "El constructor por defecto inicializa la altitud a 0.0");
    probar(pDef.getTimestamp() == 0, "El constructor por defecto inicializa el timestamp a 0");
    probar(pDef.getDispositivo() == "", "El constructor por defecto inicializa el dispositivo como cadena vacía");
    probar(pDef.getNumDatosExtra() == 0, "El constructor por defecto inicializa num_datos_extra a 0");

    PuntoGPS p5(38.3854, -0.5127, 120.5, 1758024000, "GARMIN-01");
    probar(p5.getLatitud() == 38.3854, "El constructor de 5 parámetros asigna correctamente la latitud");
    probar(p5.getLongitud() == -0.5127, "El constructor de 5 parámetros asigna correctamente la longitud");
    probar(p5.getAltitud() == 120.5, "El constructor de 5 parámetros asigna correctamente la altitud");
    probar(p5.getTimestamp() == 1758024000, "El constructor de 5 parámetros asigna correctamente el timestamp");
    probar(p5.getDispositivo() == "GARMIN-01", "El constructor de 5 parámetros asigna el nombre del dispositivo");
    probar(p5.getNumDatosExtra() == 0, "El constructor de 5 parámetros inicializa num_datos_extra a 0");

    PuntoGPS p6(38.3854, -0.5127, 120.5, 1758024000, "GARMIN-01", 2);
    probar(p6.getNumDatosExtra() == 2, "El constructor de 6 parámetros reserva la cantidad correcta de datos extra");
    probar(p6.getDatoExtra(0) == 0.0, "Los datos extra se inicializan a 0.0 por defecto (índice 0)");
    probar(p6.getDatoExtra(1) == 0.0, "Los datos extra se inicializan a 0.0 por defecto (índice 1)");

    PuntoGPS pInvalidoLat(95.0, -0.5127, 120.5, 1000, "DEV-ERR");
    probar(pInvalidoLat.getLatitud() == 0.0 && pInvalidoLat.getDispositivo() == "", 
           "Constructor con latitud fuera de rango (+95) reinicia el objeto a valores por defecto");

    PuntoGPS pInvalidoAlt(38.3854, -0.5127, -10.0, 1000, "DEV-ERR", 3);
    probar(pInvalidoAlt.getAltitud() == 0.0 && pInvalidoAlt.getNumDatosExtra() == 0, 
           "Constructor con altitud negativa (-10) ignora los datos extra y reinicia a defecto");

    // ------------------------------------------------------------------
    // 2. Modificadores (Setters), Consultores (Getters) y límites
    // ------------------------------------------------------------------
    cout << endl << "--- 2. Pruebas de Setters, Getters y Datos Extra ---" << endl;
    PuntoGPS pMod;
    probar(pMod.setLatitud(40.4168) == true, "setLatitud devuelve true para una latitud válida (40.4168)");
    probar(pMod.setLatitud(100.0) == false, "setLatitud devuelve false para una latitud fuera de rango (100.0)");
    probar(pMod.getLatitud() == 40.4168, "getLatitud obtiene el valor asignado tras latitud válida");

    probar(pMod.setLongitud(-3.7038) == true, "setLongitud devuelve true para una longitud válida (-3.7038)");
    probar(pMod.setLongitud(-200.0) == false, "setLongitud devuelve false para una longitud fuera de rango (-200.0)");
    probar(pMod.getLongitud() == -3.7038, "getLongitud obtiene el valor asignado tras longitud válida");

    probar(pMod.setAltitud(650.0) == true, "setAltitud devuelve true para una altitud válida (650.0)");
    probar(pMod.setAltitud(-5.0) == false, "setAltitud devuelve false para una altitud negativa (-5.0)");
    probar(pMod.getAltitud() == 650.0, "getAltitud obtiene el valor asignado tras altitud válida");

    pMod.setTimestamp(1758024100);
    probar(pMod.getTimestamp() == 1758024100, "setTimestamp actualiza correctamente la marca de tiempo");

    pMod.setDispositivo("DESKTOP-TEST");
    probar(pMod.getDispositivo() == "DESKTOP-TEST", "setDispositivo actualiza correctamente el nombre del dispositivo");

    probar(p6.setDatoExtra(0, 15.5) == true, "setDatoExtra devuelve true para un índice válido (0)");
    probar(p6.setDatoExtra(1, 42.1) == true, "setDatoExtra devuelve true para un índice válido (1)");
    probar(p6.setDatoExtra(2, 99.0) == false, "setDatoExtra rechaza la escritura fuera de rango positivo (índice 2 en tamaño 2)");
    probar(p6.setDatoExtra(-1, 99.0) == false, "setDatoExtra rechaza la escritura en un índice negativo (-1)");
    probar(p6.getDatoExtra(0) == 15.5, "getDatoExtra devuelve el valor previamente asignado");
    probar(p6.getDatoExtra(5) == 0.0, "getDatoExtra fuera de rango devuelve 0.0 por seguridad");

    // ------------------------------------------------------------------
    // 3. Copia profunda y operador de asignación
    // ------------------------------------------------------------------
    cout << endl << "--- 3. Pruebas de Memoria Dinámica y Copia ---" << endl;
    PuntoGPS pCopia(p6);
    probar(pCopia.getDatoExtra(0) == 15.5, "El constructor de copia duplica correctamente los datos extra");
    pCopia.setDatoExtra(0, 999.9);
    probar(p6.getDatoExtra(0) == 15.5, "Copia profunda correcta en constructor de copia (el original permanece intacto)");

    PuntoGPS pAsignado;
    pAsignado = pCopia;
    probar(pAsignado.getDatoExtra(0) == 999.9, "El operador de asignación copia los datos extra correctamente");
    pAsignado.setDatoExtra(0, 1.1);
    probar(pCopia.getDatoExtra(0) == 999.9, "Copia profunda correcta en operator= (memoria independiente)");

    pAsignado = pAsignado;
    probar(pAsignado.getDatoExtra(0) == 1.1, "La auto-asignación (obj = obj) mantiene la integridad de los datos");

    // ------------------------------------------------------------------
    // 4. Operadores de comparación
    // ------------------------------------------------------------------
    cout << endl << "--- 4. Pruebas de Operadores Relacionales ---" << endl;
    PuntoGPS pT1(40.0, -3.0, 100.0, 1000, "DEV-A");
    PuntoGPS pT2(40.0, -3.0, 100.0, 2000, "DEV-B");
    PuntoGPS pT1Igual(40.0, -3.0, 100.0, 1000, "DEV-C");

    probar(pT1 < pT2, "operator< devuelve true cuando t1 < t2 (1000s < 2000s)");
    probar(!(pT2 < pT1), "operator< devuelve false cuando t1 > t2");
    probar(pT2 > pT1, "operator> devuelve true cuando t1 > t2");
    probar(!(pT1 < pT1Igual), "operator< devuelve false cuando los timestamps son iguales");

    probar(pT1 == pT1Igual, "operator== devuelve true cuando coinciden lat, lon, alt y timestamp");
    probar(pT1 != pT2, "operator!= devuelve true cuando difieren los timestamps");

    PuntoGPS pEpsilon(40.0 + 1e-7, -3.0, 100.0, 1000, "DEV-D");
    probar(pT1 == pEpsilon, "operator== aplica tolerancia EPSILON (1e-6) en diferencias flotantes");

    // ------------------------------------------------------------------
    // 5. Cálculos de distancia y velocidad (Pruebas exhaustivas Sesión 1)
    // ------------------------------------------------------------------
    cout << endl << "--- 5. Pruebas Geográficas y Temporales (Sesión 1) ---" << endl;
    const double PI_VAL = 3.14159265358979323846;
    const double RADIO_TIERRA = 6371000.0;

    PuntoGPS pMismo1(38.3452, -0.4815, 10.0, 1000, "ALC1");
    PuntoGPS pMismo2(38.3452, -0.4815, 10.0, 1000, "ALC2");
    probar(pMismo1.distanciaSuperficie(pMismo2) == 0.0, "Distancia en superficie para el mismo punto es 0.0");
    probar(pMismo1.distancia3D(pMismo2) == 0.0, "Distancia 3D para el mismo punto es 0.0");
    probar(pMismo1.velocidad(pMismo2) == -1.0, "Velocidad devuelve -1.0 cuando t2 == t1");

    PuntoGPS pTiempo1(38.3452, -0.4815, 10.0, 1000, "DEV1");
    PuntoGPS pTiempo2(38.1905, -0.5562, 5.0, 500, "DEV2");
    probar(pTiempo1.velocidad(pTiempo2) == -1.0, "Velocidad devuelve -1.0 cuando t2 < t1");

    PuntoGPS pAltPura1(0.0, 0.0, 10.0, 100, "ALT1");
    PuntoGPS pAltPura2(0.0, 0.0, 100.0, 200, "ALT2");
    double dSupAlt = pAltPura1.distanciaSuperficie(pAltPura2);
    double d3DAlt  = pAltPura1.distancia3D(pAltPura2);
    double vAlt    = pAltPura1.velocidad(pAltPura2);
    probar(dSupAlt == 0.0, "Distancia en superficie con solo cambio de altitud es 0.0");
    probar(d3DAlt == 90.0, "Distancia 3D con solo cambio de altitud es exactamente |alt2 - alt1| (90m)");
    probar(sonIguales(vAlt, 0.9), "Velocidad en cambio de altitud puro es 0.9 m/s");

    PuntoGPS pAntipoda1(0.0, 0.0, 0.0, 0, "EQ1");
    PuntoGPS pAntipoda2(0.0, 180.0, 0.0, 1000, "EQ2");
    double dAntipodas = pAntipoda1.distanciaSuperficie(pAntipoda2);
    double dEsperadaAntipodas = PI_VAL * RADIO_TIERRA;
    probar(abs(dAntipodas - dEsperadaAntipodas) < 1.0, "Distancia a antípodas en el ecuador es cercana a PI * R");

    PuntoGPS pMadrid(40.4168, -3.7038, 650.0, 1000, "MAD");
    PuntoGPS pBuenosAires(-34.6037, -58.3816, 25.0, 40000, "BA");
    double dMadridBA = pMadrid.distanciaSuperficie(pBuenosAires);
    probar(dMadridBA > 10000000.0 && dMadridBA < 10100000.0, "Distancia Madrid-Buenos Aires se encuentra en el rango esperado (~10.040 km)");

    PuntoGPS p1DegA(0.0, 0.0, 0.0, 100, "M0");
    PuntoGPS p1DegB(1.0, 0.0, 0.0, 200, "M1");
    double d1Grado = p1DegA.distanciaSuperficie(p1DegB);
    double v1Grado = p1DegA.velocidad(p1DegB);
    probar(d1Grado > 110000.0 && d1Grado < 112000.0, "Distancia de 1 grado de latitud equivale a aprox 111.1 km");
    probar(sonIguales(v1Grado, d1Grado / 100.0), "Velocidad en desplazamiento de 1 grado coincide con d3D / delta_t");

    PuntoGPS pOrigen(38.3452, -0.4815, 10.0, 1000, "ALC");
    PuntoGPS pDestino(38.1905, -0.5562, 5.0, 1600, "SANTA_POLA");
    double dSupAli = pOrigen.distanciaSuperficie(pDestino);
    double d3DAli  = pOrigen.distancia3D(pDestino);
    double vAli    = pOrigen.velocidad(pDestino);
    probar(dSupAli > 18000.0 && dSupAli < 19000.0, "Distancia en superficie Alicante-Santa Pola correcta (~18.3 km)");
    probar(d3DAli >= dSupAli, "Distancia 3D es mayor o igual a la distancia en superficie");
    probar(sonIguales(vAli, d3DAli / 600.0), "Velocidad Alicante-Santa Pola calculada correctamente");

    // ------------------------------------------------------------------
    // 6. Cadena de representación (aCadena)
    // ------------------------------------------------------------------
    cout << endl << "--- 6. Pruebas del Método aCadena ---" << endl;
    PuntoGPS pCad1(38.3854, -0.5127, 120.5, 1758024000, "GARMIN-01");
    probar(pCad1.aCadena() == "(38.385400,-0.512700,120.500000,1758024000) GARMIN-01", 
           "aCadena genera la representación correcta para puntos sin datos extra");

    p6.setDatoExtra(0, 12.5);
    p6.setDatoExtra(1, 3.4);
    probar(p6.aCadena() == "(38.385400,-0.512700,120.500000,1758024000) GARMIN-01 12.500000 3.400000", 
           "aCadena genera la representación correcta para puntos con datos extra");

    cout << endl << "¡Todas las pruebas han pasado con éxito!" << endl;
    return 0;
}
