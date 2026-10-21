#include "PuntoGPS.h"
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

using namespace std;

//1. make
//2. g++ -std=c++11 -g -Wall Ejemplo.cc PuntoGPS.cc -o Ejemplo
//3. ./Ejemplo

//  VALGRIND
// sudo apt update && sudo apt install valgrind
// valgrind --leak-check=full ./Ejemplo
// make evalua
// valgrind ./evalua
//valgrind --leak-check=full ./Ejemplo


// Constantes utiles
const double RADIO_TIERRA = 6371000.0;
const double PI = 3.14159265358979323846;
const double EPSILON = 1e-6;

// Asigno los valores de latitud, longitud y altitud
const double LAT_MIN = -90.0;
const double LAT_MAX = 90.0;
const double LON_MIN = -180.0;
const double LON_MAX = 180.0;
const double ALT_MIN = 0.0;
const double ALT_MAX = numeric_limits<double>::max();

// Mira si las coordenadas son validas (True/False)
bool PuntoGPS::enRango(double valor, double minimo, double maximo) {
    return valor >= minimo && valor <= maximo;
}

// Constructor basico (inicializa todo a 0)
PuntoGPS::PuntoGPS()
    :   latitud(0.0), 
        longitud(0.0), 
        altitud(0.0), 
        timestamp(0),
        dispositivo(""), 
        num_datos_extra(0), 
        datos_extra(nullptr) {
}

// Constructor de 5, sin cosas extras
PuntoGPS::PuntoGPS(double lat, double lon, double alt, unsigned long t, string disp)
    :   latitud(0.0), 
        longitud(0.0), 
        altitud(0.0), 
        timestamp(0),
        dispositivo(""), 
        num_datos_extra(0), 
        datos_extra(nullptr) {

    // revisa si las coordenadas son validas, si lo son, asigna los valores, sino, por defecto
    if (enRango(lat, LAT_MIN, LAT_MAX) &&
        enRango(lon, LON_MIN, LON_MAX) &&
        enRango(alt, ALT_MIN, ALT_MAX)) {
        latitud = lat;
        longitud = lon;
        altitud = alt;
        timestamp = t;
        dispositivo = disp;
    }
}

// Constructor de 6, con cosas extras
PuntoGPS::PuntoGPS(double lat, double lon, double alt, unsigned long t, string disp, unsigned int numExtra)
    :   latitud(0.0), 
        longitud(0.0), 
        altitud(0.0), 
        timestamp(0),
        dispositivo(""), 
        num_datos_extra(0), 
        datos_extra(nullptr) {
    // revisa si las coordenadas son validas, si lo son, asigna los valores, sino, por defecto
    if (enRango(lat, LAT_MIN, LAT_MAX) &&
        enRango(lon, LON_MIN, LON_MAX) &&
        enRango(alt, ALT_MIN, ALT_MAX)) {
        latitud = lat;
        longitud = lon;
        altitud = alt;
        timestamp = t;
        dispositivo = disp;
        // Trozo del parametro extra, si es mas que cero, crea un vector dinamico, sino, nullptr
        num_datos_extra = numExtra;
        if (num_datos_extra > 0) {
            datos_extra = new double[num_datos_extra]();
        }
    }
}

// Constructor de copia
PuntoGPS::PuntoGPS(const PuntoGPS &otro)
    :   latitud(otro.latitud), 
        longitud(otro.longitud), 
        altitud(otro.altitud),
        timestamp(otro.timestamp), 
        dispositivo(otro.dispositivo),
        num_datos_extra(otro.num_datos_extra), 
        datos_extra(nullptr) {

    if (num_datos_extra > 0) {
        datos_extra = new double[num_datos_extra];
        for (unsigned int i = 0; i < num_datos_extra; ++i) {
            datos_extra[i] = otro.datos_extra[i];
        }
    }
}

// Destructor (quita la memoria dinamica)
PuntoGPS::~PuntoGPS() {
    delete[] datos_extra;
}

PuntoGPS& PuntoGPS::operator=(const PuntoGPS &otro) {
    if (this != &otro) {
        double *nuevos_datos = nullptr;
        if (otro.num_datos_extra > 0) {
            nuevos_datos = new double[otro.num_datos_extra];
            for (unsigned int i = 0; i < otro.num_datos_extra; ++i) {
                nuevos_datos[i] = otro.datos_extra[i];
            }
        }

        delete[] datos_extra;

        latitud = otro.latitud;
        longitud = otro.longitud;
        altitud = otro.altitud;
        timestamp = otro.timestamp;
        dispositivo = otro.dispositivo;
        num_datos_extra = otro.num_datos_extra;
        datos_extra = nuevos_datos;
    }
    return *this;
}

// GETTERS Y SETTERS -------------------------------
double PuntoGPS::getLatitud() const { return latitud; }

bool PuntoGPS::setLatitud(double lat) {
    if (!enRango(lat, LAT_MIN, LAT_MAX)) return false;
    latitud = lat;
    return true;
}

double PuntoGPS::getLongitud() const { return longitud; }

bool PuntoGPS::setLongitud(double lon) {
    if (!enRango(lon, LON_MIN, LON_MAX)) return false;
    longitud = lon;
    return true;
}

double PuntoGPS::getAltitud() const { return altitud; }

bool PuntoGPS::setAltitud(double alt) {
    if (!enRango(alt, ALT_MIN, ALT_MAX)) return false;
    altitud = alt;
    return true;
}

unsigned long PuntoGPS::getTimestamp() const { return timestamp; }

void PuntoGPS::setTimestamp(unsigned long t) { timestamp = t; }

string PuntoGPS::getDispositivo() const { return dispositivo; }

void PuntoGPS::setDispositivo(const string& disp) { dispositivo = disp; }

int PuntoGPS::getNumDatosExtra() const {
    return static_cast<int>(num_datos_extra);
}

double PuntoGPS::getDatoExtra(int indice) const {
    if (indice < 0 || static_cast<unsigned int>(indice) >= num_datos_extra) {
        return 0.0;
    }
    return datos_extra[indice];
}

bool PuntoGPS::setDatoExtra(int indice, double valor) {
    if (indice < 0 || static_cast<unsigned int>(indice) >= num_datos_extra) {
        return false;
    }
    datos_extra[indice] = valor;
    return true;
}

void PuntoGPS::anadirDatoExtra(double nuevoValor) {
    double*nuevo_array = new double[num_datos_extra+1];
    for (unsigned int i = 0; i < num_datos_extra; ++i) {
        nuevo_array[i] = datos_extra[i];
        }
    nuevo_array[num_datos_extra] = nuevoValor;
    num_datos_extra +=1;
    delete[] datos_extra;
    datos_extra = nuevo_array;

    
}

// SOBRECARGA DE OPERADORES -------------------------------

bool PuntoGPS::operator<(const PuntoGPS& otro) const {
    return timestamp < otro.timestamp;
}

bool PuntoGPS::operator>(const PuntoGPS& otro) const {
    return timestamp > otro.timestamp;
}

bool PuntoGPS::operator==(const PuntoGPS& otro) const {
    return abs(latitud - otro.latitud) < EPSILON &&
           abs(longitud - otro.longitud) < EPSILON &&
           abs(altitud - otro.altitud) < EPSILON &&
           timestamp == otro.timestamp;
}

bool PuntoGPS::operator!=(const PuntoGPS& otro) const {
    return !(*this == otro);
}

//Sesion 1


// 1. Distancia sobre la superficie terrestre (en metros)
double PuntoGPS::distanciaSuperficie(const PuntoGPS& otro) const {
    double lat1rad = latitud * PI / 180;
    double lat2rad = otro.latitud * PI / 180;
    double diflat = (otro.latitud - latitud) * PI / 180;
    double diflon = (otro.longitud - longitud) * PI / 180;

    double a = ((1 - cos(diflat)) / 2 + (cos(lat1rad) * cos(lat2rad)) * ((1 - cos(diflon)) / 2));
    
    // Control de limites por precision decimal
    if (a > 1.0) a = 1.0;
    if (a < 0.0) a = 0.0;

    double d = 2 * RADIO_TIERRA * asin(sqrt(a));
    return d;
}

// 2. Distancia euclídea en tres dimensiones (en metros)
double PuntoGPS::distancia3D(const PuntoGPS& otro) const {
    double dSup = distanciaSuperficie(otro);
    double diffAlt = otro.altitud - altitud;
    return sqrt(pow(dSup, 2) + pow(diffAlt, 2));
}

// 3. Velocidad media (en m/s)
double PuntoGPS::velocidad(const PuntoGPS& otro) const {
    if (otro.timestamp <= timestamp)
        return -1;

    double v = distancia3D(otro) / (otro.timestamp - timestamp);
    return v;
}

// representacion en cadena

string PuntoGPS::aCadena() const {
    ostringstream salida;
    salida << fixed << setprecision(6);
    salida << "(" << latitud << "," << longitud << "," << altitud
           << "," << timestamp << ") " << dispositivo;

    for (unsigned int i = 0; i < num_datos_extra; ++i) {
        salida << " " << datos_extra[i];
    }

    return salida.str();
}


