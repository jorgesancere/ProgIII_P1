#ifndef LISTAPUNTOSGPS_H
#define LISTAPUNTOSGPS_H
using namespace std;

//Includes
#include <string>
#include "PuntoGPS.h"

class NodoLista {

private:
    NodoLista* siguiente;
    PuntoGPS puntoGPS;
public: 
    NodoLista* head;
    NodoLista();
    NodoLista(NodoLista*, const PuntoGPS&);
    NodoLista(const NodoLista&);
    NodoLista& operator=(const NodoLista&);
    ~NodoLista();
    NodoLista* getSiguiente() const;
    PuntoGPS getPuntoGPS() const;
    void setSiguiente(NodoLista*);
    void setPuntoGPS(const PuntoGPS&);
};

class IteradorLista {

private:
    NodoLista* pt;
public:
    IteradorLista();
    IteradorLista(const IteradorLista&);
    ~IteradorLista();
    IteradorLista& operator=(const IteradorLista&);
    void step();
    bool operator==(const IteradorLista&) const;
    bool operator!=(const IteradorLista&) const;
friend class ListaPuntosGPS;

};

class ListaPuntosGPS{

private:
    NodoLista* head;
public:
    ListaPuntosGPS();
    IteradorLista begin() const;
    IteradorLista end() const;
    PuntoGPS getPuntoGPS(IteradorLista) const;
    bool esVacia() const;
    int longitud() const;
    bool insertar(const PuntoGPS&);


};

#endif
