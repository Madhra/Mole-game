/*
 * File:   Configuracion.c
 * Author: DP
 *
 * Created on 14 de abril 2022, 04:06 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Configuracion_reloj.h"

#define _XTAL_FREQ 48000000

//Numeros 0 - 9 para 7 Segmentos
#define CERO 0b01000000
#define UNO 0b01111001
#define DOS 0b00100100
#define TRES 0b0110000
#define CUATRO 0b00011001
#define CINCO 0b00010010
#define SEIS 0b00000010
#define SIETE 0b01111000
#define OCHO 0b00000000
#define NUEVE 0b00010000

int i = 0;

int delay();

//Numeros 0 - 9 para 7 Segmentos
char numeros[10] = {CERO, UNO, DOS, TRES, CUATRO, CINCO, SEIS, SIETE, OCHO, NUEVE}; // Arreglo con los numeros del display

//Generar un numero aleatorio entre el 0 y 2

int aleatorio() {

    int x = rand() % 3;
    return x;
}

//Declarar funciones de interrupcion
void interrupt interrupcion();

void main(void) {

    //Configuracion de Puertos
    ADCON1 = 0X0F;

    TRISD = 0; //Puerto de salida para Display 7 Segmentos
    LATD = numeros[0]; // Display 7 Segmentos en 0


    //Entrada de los botones
    TRISBbits.TRISB0 = 1; //Entrada RB0
    TRISBbits.TRISB1 = 1; //Entrada RB1
    TRISBbits.TRISB2 = 1; //Entrada RB2

    //Salida de LEDs
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;

    //Interrupciones

    RCONbits.IPEN = 1; //Habilitar prioridad de interrupciones
    INTCONbits.PEIE = 1; //Habilitar interrupciones perifericas
    INTCONbits.GIE = 1; //Habilita todas las interrupciones de alta prioridad

    INTCON2bits.INTEDG0 = 0; //Detectar la interrupcion RB0 en el flanco de bajada
    INTCON2bits.INTEDG1 = 0; //Detectar la interrupcion RB1 en el flanco de bajada
    INTCON2bits.INTEDG2 = 0; //Detectar la interrupcion RB2 en el flanco de bajada

    INTCONbits.INT0IE = 1; //Habilitar interrupcion RB0
    INTCONbits.INT0IF = 0; //Bandera en 0

    INTCON3bits.INT1IE = 1; //Habilitar interrupcion RB1
    INTCON3bits.INT1IF = 0; //Bandera en 0
    INTCON3bits.INT1IP = 1; //Prioridad alta en RB1

    INTCON3bits.INT2IE = 1; //Habilitar interrupcion RB2
    INTCON3bits.INT2IF = 0; //Bandera en 0
    INTCON3bits.INT2IP = 1; //Prioridad alta en RB2

    //Timer
    T0CONbits.T08BIT = 0; //Configuracion timer 16 bits
    T0CONbits.PSA = 0; //Pre-escalador activado, los pulsos vienen del preescaldor
    T0CONbits.T0CS = 0; // Reloj interno para el timer
    T0CONbits.T0PS = 7; // Preescalador en 7 para 256
    TMR0 = 18660;
    T0CONbits.TMR0ON = 1; //Habilitar el timer en 0
    INTCONbits.TMR0IE = 1; //Habilitar interrupcion del timer 0
    INTCONbits.T0IF = 0; //Limpiar bandera de la interrupcion

    while (1) {
    }
}

void interrupt interrupcion() {

    int x;
    int nivel = i;



    if (INTCONbits.TMR0IF == 1) {

        //Colocar un switch evaluando nivel
        switch (nivel) {

            case 0:
            case 1:
            case 2:
                switch (x = rand() % 3) {//Nivel 1 1s

                    case 0:
                        LATBbits.LATB3 = !LATBbits.LATB3;
                        TMR0 = 18660;
                        break;

                    case 1:
                        LATBbits.LATB4 = !LATBbits.LATB4;
                        TMR0 = 18660;
                        break;

                    case 2:
                        LATBbits.LATB5 = !LATBbits.LATB5;
                        TMR0 = 18660;
                        break;
                }
                break;

            case 3:
            case 4:
            case 5:

                switch (x = rand() % 3) {//Nivel 2 0.5s

                    case 0:
                        LATBbits.LATB3 = !LATBbits.LATB3;
                        TMR0 = 42097;
                        break;

                    case 1:
                        LATBbits.LATB4 = !LATBbits.LATB4;
                        TMR0 = 42097;
                        break;

                    case 2:
                        LATBbits.LATB5 = !LATBbits.LATB5;
                        TMR0 = 42097;
                        break;
                }
                break;

            case 6:
            case 7:
            case 8:
            case 9:

                switch (x = rand() % 3) {//Nivel 3 0.25s

                    case 0:
                        LATBbits.LATB3 = !LATBbits.LATB3;
                        TMR0 = 53816;
                        break;

                    case 1:
                        LATBbits.LATB4 = !LATBbits.LATB4;
                        TMR0 = 53816;
                        break;

                    case 2:
                        LATBbits.LATB5 = !LATBbits.LATB5;
                        TMR0 = 53816;
                        break;
                }
                break;
        }

        INTCONbits.TMR0IF = 0;
    }

    //Si se presiona el Primer boton
    if (INTCONbits.INT0IF == 1) {

        //Verificar que el LED 0 este encendido
        if (PORTBbits.RB3 == 1) {

            incremento();
            LATBbits.LATB3 = 0;

        } else {//Si el Led esta apagado

            reinicio(); //Si se presiona el boton incorrecto

            //Apagar Leds si no es el boton correcto
            LATBbits.LATB3 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB5 = 0;
        }

        INTCONbits.INT0IF = 0; //Poner la vandera RB0 en 0
        return;
    }

    //Si se preciona el segundo boton
    if (INTCON3bits.INT1IF == 1) {
        //Verificar que el LED 1 este encendido
        if (PORTBbits.RB4 == 1) {

            incremento();
            LATBbits.LATB4 = 0;

        } else {//Siel Led esta apagado

            reinicio(); //Si se presiona el boton incorrecto

            //Apagar Leds si no es el boton correcto
            LATBbits.LATB3 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB5 = 0;
        }
        INTCON3bits.INT1IF = 0; //Poner la bandera RB1 en 0
        return;
    }

    //Si se preciona el tercer boton
    if (INTCON3bits.INT2IF == 1) {

        //Verificar que el LED 2 este encendido
        if (PORTBbits.RB5 == 1) {

            incremento();
            LATBbits.LATB5 = 0;

        } else {//Si el Led esta apagado

            reinicio(); //Si se presiona el boton incorrecto

            //Apagar Leds si no es el boton correcto
            LATBbits.LATB3 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB5 = 0;
        }
        INTCON3bits.INT2IF = 0; //Poner la bandera RB2 en 0
        return;
    }

}

int incremento() {
    i++;
    if (i == 10) i = 0; //Reiniciar juego cada 10 aciertos
    PORTD = numeros[i];
}

int reinicio() {
    i = 0;
    PORTD = numeros[0];
}
