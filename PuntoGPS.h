#ifndef PUNTOGPS_H
#define PUNTOGPS_H

#include <string>

class PuntoGPS {
private:
    // Atributos
    double latitud;            // grados, rango [-90, 90]
    double longitud;           // grados, rango [-180, 180]
    double altitud;            // metros, >= 0
    unsigned long timestamp;   // segundos (marca de tiempo)
    std::string dispositivo;   // identificador del dispositivo que tomo la medida
    int num_datos_extra;       // numero de datos adicionales
    double* datos_extra;       // vector dinamico de datos adicionales (nullptr si no hay)

    // Metodo auxiliar: true si minimo <= valor <= maximo
    static bool enRango(double valor, double minimo, double maximo);

public:
    // Forma canónica
    PuntoGPS();
    PuntoGPS(double lat, double lon, double alt, unsigned long t, std::string disp);
    PuntoGPS(double lat, double lon, double alt, unsigned long t, std::string disp, unsigned int numExtra);
    PuntoGPS(const PuntoGPS &otro);
    ~PuntoGPS();
    PuntoGPS& operator=(const PuntoGPS &otro);

    // Getters y setters
    double getLatitud() const;
    bool setLatitud(double lat);
    double getLongitud() const;
    bool setLongitud(double lon);
    double getAltitud() const;
    bool setAltitud(double alt);
    unsigned long getTimestamp() const;
    void setTimestamp(unsigned long t);
    std::string getDispositivo() const;
    void setDispositivo(const std::string& disp);
    int getNumDatosExtra() const; 
    double getDatoExtra(int indice) const;
    bool setDatoExtra(int indice, double valor);

    // Sobrecarga de operadores
    bool operator<(const PuntoGPS& otro) const;
    bool operator>(const PuntoGPS& otro) const;
    bool operator==(const PuntoGPS& otro) const;
    bool operator!=(const PuntoGPS& otro) const;

    // Distancias y velocidad
    double distanciaSuperficie(const PuntoGPS& otro) const;
    double distancia3D(const PuntoGPS& otro) const;
    double velocidad(const PuntoGPS& otro) const;

    // Representación
    std::string aCadena() const;
};

#endif