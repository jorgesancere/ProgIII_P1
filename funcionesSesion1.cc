#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

// Constantes globales recomendadas
const double RADIO_TIERRA = 6371000.0; // en metros
const double PI = 3.14159265358979323846;


// 1. Distancia sobre la superficie terrestre (en metros). 
double distanciaSuperficie(double lat1, double lon1, double alt1, unsigned long t1,
                            double lat2, double lon2, double alt2, unsigned long t2) {
    double lat1rad = lat1*PI/180;
    double lat2rad = lat2*PI/180;
    double diflat = (lat2 - lat1)*PI/180;
    double diflon = (lon2 - lon1)*PI/180;
    double a = ((1 - cos(diflat))/2 + (cos(lat1rad) * cos(lat2rad)) * ((1- cos(diflon))/2));
    double d = 2 * RADIO_TIERRA * asin(sqrt(a));
    return d;
}

// 2. Distancia euclídea en tres dimensiones (en metros)
double distancia3D(double lat1, double lon1, double alt1, unsigned long t1,
                   double lat2, double lon2, double alt2, unsigned long t2) {
    double dSup = distanciaSuperficie(lat1, lon1, alt1, t1, lat2, lon2, alt2, t2);
    double diffAlt = alt2 - alt1;
    return sqrt(pow(dSup, 2) + pow(diffAlt, 2));
}
// 3. Velocidad media (en m/s)
double velocidad(double lat1, double lon1, double alt1, unsigned long t1,
                  double lat2, double lon2, double alt2, unsigned long t2) {
    if(t2<=t1)
        return -1;

    double v = distancia3D(lat1, lon1, alt1, t1, lat2, lon2, alt2, t2)/ (t2-t1);
    	return v;
}
int main() {
    cout << "Ejecutando suite completa de pruebas de la sesión 1..." << endl;

    // ------------------------------------------------------------------
    // Test 1: Mismo punto (distancia 0 m, velocidad invalida por t2 == t1)
    // ------------------------------------------------------------------
    assert(distanciaSuperficie(38.3452, -0.4815, 10.0, 1000, 38.3452, -0.4815, 10.0, 1000) == 0.0);
    assert(distancia3D(38.3452, -0.4815, 10.0, 1000, 38.3452, -0.4815, 10.0, 1000) == 0.0);
    assert(velocidad(38.3452, -0.4815, 10.0, 1000, 38.3452, -0.4815, 10.0, 1000) == -1.0); // t2 == t1

    // ------------------------------------------------------------------
    // Test 2: Validación de errores temporales (t2 < t1)
    // ------------------------------------------------------------------
    assert(velocidad(38.3452, -0.4815, 10.0, 1000, 38.1905, -0.5562, 5.0, 500) == -1.0);  // t2 < t1

    // ------------------------------------------------------------------
    // Test 3: Cambio de altitud puro (misma latitud y longitud)
    // La distancia en superficie debe ser 0, pero la 3D debe ser |alt2 - alt1|
    // ------------------------------------------------------------------
    double dSupPuroAlt = distanciaSuperficie(0.0, 0.0, 10.0, 100, 0.0, 0.0, 100.0, 200);
    double d3DPuroAlt  = distancia3D(0.0, 0.0, 10.0, 100, 0.0, 0.0, 100.0, 200);
    double vPuroAlt    = velocidad(0.0, 0.0, 10.0, 100, 0.0, 0.0, 100.0, 200);

    assert(dSupPuroAlt == 0.0);
    assert(d3DPuroAlt == 90.0);  // 100m - 10m = 90m
    assert(vPuroAlt == 0.9);      // 90m / 100s = 0.9 m/s

    // ------------------------------------------------------------------
    // Test 4: Caso teórico límite - Antípodas en el Ecuador (180° de longitud)
    // Distancia teórica esperada = PI * RADIO_TIERRA (aprox 20.015.087 metros)
    // ------------------------------------------------------------------
    double dAntipodas = distanciaSuperficie(0.0, 0.0, 0.0, 0, 0.0, 180.0, 0.0, 1000);
    double dEsperadaAntipodas = PI * RADIO_TIERRA;
    assert(abs(dAntipodas - dEsperadaAntipodas) < 1.0); // Tolerancia < 1 metro

    // ------------------------------------------------------------------
    // Test 5: Transición entre hemisferios (Norte/Sur y Este/Oeste)
    // Ejemplo: Madrid (40.4168, -3.7038) a Buenos Aires (-34.6037, -58.3816)
    // Rango esperado aproximado: ~10.040 km (10.040.000 metros)
    // ------------------------------------------------------------------
    double dMadridBA = distanciaSuperficie(40.4168, -3.7038, 650.0, 1000, -34.6037, -58.3816, 25.0, 40000);
    assert(dMadridBA > 10000000.0 && dMadridBA < 10100000.0);

    // ------------------------------------------------------------------
    // Test 6: Desplazamiento de 1 grado de latitud sobre el meridiano 0
    // 1° de latitud equivale a aproximadamente 111,1 km (111.100 m)
    // ------------------------------------------------------------------
    double d1Grado = distanciaSuperficie(0.0, 0.0, 0.0, 100, 1.0, 0.0, 0.0, 200);
    assert(d1Grado > 110000.0 && d1Grado < 112000.0);
    
    double v1Grado = velocidad(0.0, 0.0, 0.0, 100, 1.0, 0.0, 0.0, 200);
    assert(v1Grado == d1Grado / 100.0); // Verificación exacta de v = d3D / delta_t

    // ------------------------------------------------------------------
    // Test 7: Alicante - Santa Pola (prueba regional con altitud)
    // ------------------------------------------------------------------
    double dSupAliSP = distanciaSuperficie(38.3452, -0.4815, 10.0, 1000, 38.1905, -0.5562, 5.0, 1600);
    double d3DAliSP  = distancia3D(38.3452, -0.4815, 10.0, 1000, 38.1905, -0.5562, 5.0, 1600);
    double vAliSP    = velocidad(38.3452, -0.4815, 10.0, 1000, 38.1905, -0.5562, 5.0, 1600);

    assert(dSupAliSP > 18000.0 && dSupAliSP < 19000.0);
    assert(d3DAliSP >= dSupAliSP); 
    assert(vAliSP > 0.0 && vAliSP == (d3DAliSP / 600.0));

    cout << "¡Todas las pruebas (7/7) se ejecutaron con exito!" << endl;
    return 0;
}
