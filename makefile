
COMP=g++
OPT=-Wall -std=c++11 -g

Ejemplo: Ejemplo.o PuntoGPS.o
	$(COMP) $(OPT) -o Ejemplo Ejemplo.o PuntoGPS.o

Ejemplo.o: Ejemplo.cc PuntoGPS.h
	$(COMP) $(OPT) -c  Ejemplo.cc

PuntoGPS.o: PuntoGPS.cc PuntoGPS.h
	$(COMP) $(OPT) -c PuntoGPS.cc

clean:
	rm -f *.o Ejemplo
