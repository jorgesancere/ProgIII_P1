
COMP=g++
OPT=-Wall -std=c++11 -g

main: Ejemplo.o PuntoGPS.o ListaPuntosGPS.o
	$(COMP) $(OPT) -o main Ejemplo.o PuntoGPS.o ListaPuntosGPS.o

Ejemplo.o: Ejemplo.cc PuntoGPS.h ListaPuntosGPS.h
	$(COMP) $(OPT) -c  Ejemplo.cc

PuntoGPS.o: PuntoGPS.cc PuntoGPS.h
	$(COMP) $(OPT) -c PuntoGPS.cc

ListaPuntosGPS.o: ListaPuntosGPS.cc ListaPuntosGPS.h PuntoGPS.h
	$(COMP) $(OPT) -c ListaPuntosGPS.cc


clean:
	rm -f *.o main
