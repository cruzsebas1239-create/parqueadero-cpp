#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>

struct Celda {
    char tipo;          // 'P' Parqueo, 'V' Via, 'E' Entrada, 'S' Salida, '#' Pared
    bool ocupado;
    std::string placa;
    time_t tiempoEntrada;
    char tipoVehiculo;  // 'M' Moto, 'C' Carro, 'G' Grande
};

const int DIM = 20; 
void generarMapa(Celda** mapa);
void imprimirMapa(Celda** mapa);
void registrarIngreso(Celda** mapa);
void registrarSalida(Celda** mapa);
double calcularTarifa(time_t inicio, char tipo);
void liberarMemoria(Celda** mapa);

int main() {
    Celda** parqueadero = new Celda*[DIM];
    for (int i = 0; i < DIM; ++i) {
        parqueadero[i] = new Celda[DIM];
    }

    generarMapa(parqueadero);
    int opcion = 0;

    do {
        std::cout << "\n========================================";
        std::cout << "\n   SUPER PARQUEADERO INTELIGENTE - MENU PRINCIPAL :)";
        std::cout << "\n========================================";
        std::cout << "\n1. Visualizar Estado Actual (Mapa)";
        std::cout << "\n2. Ingreso de Vehiculo";
        std::cout << "\n3. Salida y Facturacion";
        std::cout << "\n4. Cerrar Sistema";
        std::cout << "\nSeleccione: ";
        std::cin >> opcion;

        if (opcion == 1) imprimirMapa(parqueadero);
        else if (opcion == 2) registrarIngreso(parqueadero);
        else if (opcion == 3) registrarSalida(parqueadero);

    } while (opcion != 4);

    liberarMemoria(parqueadero);
    return 0;
}

void generarMapa(Celda** mapa) {
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            mapa[i][j].ocupado = false;
            
            if (i == 0 || i == DIM - 1 || j == 0 || j == DIM - 1) mapa[i][j].tipo = '#';
             
            else if (j == 5 || j == 14 || i == 10) mapa[i][j].tipo = 'V';
            else mapa[i][j].tipo = 'P';
        }
    }
    mapa[0][5].tipo = 'E'; 
    mapa[DIM - 1][14].tipo = 'S'; 
}

void imprimirMapa(Celda** mapa) {
    std::cout << "\n    ";
    for (int j = 0; j < DIM; ++j) std::cout << std::setw(2) << j;
    std::cout << "\n";

    for (int i = 0; i < DIM; ++i) {
        std::cout << std::setw(2) << i << "  ";
        for (int j = 0; j < DIM; ++j) {
            if (mapa[i][j].tipo == '#') std::cout << "##";
            else if (mapa[i][j].tipo == 'V') std::cout << "  ";
            else if (mapa[i][j].tipo == 'E') std::cout << "EE";
            else if (mapa[i][j].tipo == 'S') std::cout << "SS";
            else if (mapa[i][j].ocupado) std::cout << "XX"; 
            else std::cout << "[]"; 
        }
        std::cout << "\n";
    }
}

void registrarIngreso(Celda** mapa) {
    int f, c;
    std::cout << "Ingrese Coordenadas (Fila Columna): ";
    std::cin >> f >> c;

    if (f > 0 && f < DIM && c > 0 && c < DIM && mapa[f][c].tipo == 'P' && !mapa[f][c].ocupado) {
        Celda* slot = &mapa[f][c]; 
        std::cout << "Tipo de Vehiculo (M: Moto, C: Carro, G: Camion): ";
        std::cin >> slot->tipoVehiculo;
        std::cout << "Ingrese Placa: ";
        std::cin >> slot->placa;
        slot->tiempoEntrada = time(0);  
        slot->ocupado = true;
        std::cout << "-> Ingreso registrado correctamente.\n";
    } else {
        std::cout << "Coordenadas invalidas o celda ocupada.\n";
    }
}

void registrarSalida(Celda** mapa) {
    int f, c;
    std::cout << "Ingrese Coordenadas de Salida (Fila Columna): ";
    std::cin >> f >> c;

    if (f >= 0 && f < DIM && c >= 0 && c < DIM && mapa[f][c].ocupado) {
        Celda* slot = &mapa[f][c];
        time_t tiempoSalida = time(0);
        double tarifa = calcularTarifa(slot->tiempoEntrada, slot->tipoVehiculo);
        std::cout << "Placa: " << slot->placa << "\n";
        std::cout << "Tiempo estacionado: " << difftime(tiempoSalida, slot->tiempoEntrada) / 3600 << " horas\n";
        std::cout << "Tarifa a pagar: $" << tarifa << "\n";
        slot->ocupado = false;
        slot->placa = "";
        slot->tiempoEntrada = 0;
        slot->tipoVehiculo = '\0';
        std::cout << "-> Salida registrada correctamente.\n";
    } else {
        std::cout << "Coordenadas invalidas o celda no ocupada.\n";
    }
}

double calcularTarifa(time_t inicio, char tipo) {
    time_t ahora = time(0);
    double horas = difftime(ahora, inicio) / 3600.0;
    double tarifaPorHora;
    if (tipo == 'M') tarifaPorHora = 1.0;  // Ejemplo: $1 por hora para moto
    else if (tipo == 'C') tarifaPorHora = 2.0;  // $2 para carro
    else if (tipo == 'G') tarifaPorHora = 5.0;  // $5 para camion
    else tarifaPorHora = 0.0;
    return horas * tarifaPorHora;
}

void liberarMemoria(Celda** mapa) {
    for (int i = 0; i < DIM; ++i) {
        delete[] mapa[i];
    }
    delete[] mapa;
}