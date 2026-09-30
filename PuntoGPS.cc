#include "PuntoGPS.h"
#include <cmath>
#include <limits>

using namespace std;

// Constantes
const double RADIO_TIERRA = 6371000.0;
const double PI = 3.14159265358979323846;

const double LAT_MIN = -90.0;
const double LAT_MAX = 90.0;
const double LON_MIN = -180.0;
const double LON_MAX = 180.0;
const double ALT_MIN = 0.0;
const double ALT_MAX = numeric_limits<double>::max();

// Método auxiliar privado
bool PuntoGPS::enRango(double valor, double minimo, double maximo) {
    return valor >= minimo && valor <= maximo;
}

// Constructor por defecto
PuntoGPS::PuntoGPS()
    : latitud(0.0), longitud(0.0), altitud(0.0), timestamp(0),
      dispositivo(""), num_datos_extra(0), datos_extra(nullptr) {
}

// Constructor con 5 parametros
PuntoGPS::PuntoGPS(double lat, double lon, double alt, unsigned long t, string disp)
    : latitud(0.0), longitud(0.0), altitud(0.0), timestamp(0),
      dispositivo(""), num_datos_extra(0), datos_extra(nullptr) {

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

// Constructor con 6 parametros
PuntoGPS::PuntoGPS(double lat, double lon, double alt, unsigned long t, string disp, unsigned int numExtra)
    : latitud(0.0), longitud(0.0), altitud(0.0), timestamp(0),
      dispositivo(""), num_datos_extra(0), datos_extra(nullptr) {

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

// Constructor de copia
PuntoGPS::PuntoGPS(const PuntoGPS &otro)
    : latitud(otro.latitud), longitud(otro.longitud), altitud(otro.altitud),
      timestamp(otro.timestamp), dispositivo(otro.dispositivo),
      num_datos_extra(0), datos_extra(nullptr) {
}

// Destructor
PuntoGPS::~PuntoGPS() {
}

// Operador de asignacion
PuntoGPS& PuntoGPS::operator=(const PuntoGPS &otro) {
    if (this != &otro) {
        latitud = otro.latitud;
        longitud = otro.longitud;
        altitud = otro.altitud;
        timestamp = otro.timestamp;
        dispositivo = otro.dispositivo;
    }
    return *this;
}

// Getters y setters
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


int PuntoGPS::getNumDatosExtra() const { return num_datos_extra; }
double PuntoGPS::getDatoExtra(int) const { return 0.0; }
bool PuntoGPS::setDatoExtra(int, double) { return false; }

// Operadores NO IMPLEMENTADO AUN
bool PuntoGPS::operator<(const PuntoGPS&) const { return false; }
bool PuntoGPS::operator>(const PuntoGPS&) const { return false; }
bool PuntoGPS::operator==(const PuntoGPS&) const { return false; }
bool PuntoGPS::operator!=(const PuntoGPS&) const { return false; }

// Distancias y velocidad
double PuntoGPS::distanciaSuperficie(const PuntoGPS& otro) const {
    double lat1rad = latitud * PI / 180;
    double lat2rad = otro.latitud * PI / 180;
    double diflat = (otro.latitud - latitud) * PI / 180;
    double diflon = (otro.longitud - longitud) * PI / 180;
    double a = (1 - cos(diflat)) / 2 + cos(lat1rad) * cos(lat2rad) * ((1 - cos(diflon)) / 2);
    double raiz = sqrt(a);
    if (raiz > 1.0) raiz = 1.0;
    return 2 * RADIO_TIERRA * asin(raiz);
}
// Distancia 3D
double PuntoGPS::distancia3D(const PuntoGPS& otro) const {
    double dSup = distanciaSuperficie(otro);
    double diffAlt = otro.altitud - altitud;
    return sqrt(pow(dSup, 2) + pow(diffAlt, 2));
}
// Funcion velocidad
double PuntoGPS::velocidad(const PuntoGPS& otro) const {
    if (otro.timestamp <= timestamp) return -1;
    return distancia3D(otro) / (otro.timestamp - timestamp);
}
//Representacion en cadena
string PuntoGPS::aCadena() const { return ""; }