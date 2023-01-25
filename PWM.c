
#include "lib/include.h"



extern void Configura_Reg_PWM0(uint16_t freq)
{
    //EN ESTA LINEA SE HABILITA EL CLOCK DEL MODULO PWM0. RECORDAR QUE LA TIVA GRANDE SOLO TIENE 1, PAG 398 
    SYSCTL->RCGCPWM |= (1<<0); 
    
    //AQUÍ HABILITAMOS EL RELJO DEL GPIO EN EL PORT G, PIN PG0- P.383
    SYSCTL->RCGCGPIO |= (1<<6); 
    
    //AQUÍ SE HABILITA LA FUNCIPON ALTERNATIVA CON EL AFSEL PARA PIN PG0 P.770  
    GPIOG_AHB->AFSEL |= (1<<0); /*Control de registros ya sea por GPIO o Otros Pag 672*/
   //         (ENABLE << PIN)
   
   //AQUÍ HABILITAMOS COMO PIN DE SALIDA AL PG0, P.760
    GPIOG_AHB->DIR = (1<<0);
    //(ENTRADA / SALIDA<< PIN)

    GPIOG_AHB->PCTL |= (GPIOG_AHB->PCTL&0xFFFFFFF0) | 0x00000006;
    
    //GPIOF_AHB->AFSEL |= 0x0E;
    //GPIOF_AHB->PCTL |= (GPIOF_AHB->PCTL&0xFFFF000F) | 0x00006660; /*Combinado con la tabla Pag 1351 y el registro PCTL le digo que es pwm Pag 689*/
    
    //HABILITAMOS NUESTRO PIN COMO DIGITAL P.781
    GPIOG_AHB->DEN |= (1<<0); 
    // (DIGITAL/ANALOG<<PIN)  1->DIGITAL
    //                        0->ANALOG   

    //ACTIVACIÓN DEL DIVISOR 
    PWM0->CC &= ~(1<<20);  /*Enable o Disable Divisor  Pag 1747*/
    
    //________________________________________________PWM SETTING
    
    //BLOQUE Y DESBLOQUEO, EN ESTE CASO, BLOQUEO
    PWM0->_2_CTL = (0 << 0);
    
    /*USAREMOS  EL COMAPARADOR A, EL PWM-GENA LANZA UN DISPARO EN EL PWM
    CUANDO LOS VALORES DE COMPRARACIÓN COINCIDEN */
    PWM0->_2_GENA = 0x0000008C; /*Registro de las acciones del pwm Pag 1282*/

    //ASIGNAMOS EL VALOR
    PWM0->_2_LOAD = 5000;
    /*PARA SACAR CUENTAS ES:
            CUENTAS=F.CLK /F.PWM
            EJ. CON 50 kHz 
            CUENTAS=50 X10^6/ 10 000 = 5000 CUENTAS*/

    //ASIGNACIÓN DEL CICLO DE TRABAJO-> ESTA A 70%
    PWM0->_2_CMPA = 3500;// DE LAS 5000, 3500 EN ALTO Y 1500 EN BAJO (70% / 30%)
    
    // DESBLOQUEAMOS
    PWM0->_2_CTL = (1<<0);

    //HABILITO; RECORDAR LA LOGICA DE TRABAJO DE LA TIVA, APAGO, CONFIGURO Y LUEGO ENCIENDO.
    PWM0->ENABLE = (1<<4); /*habilitar el bloque pa que pase P.1687*/

}
