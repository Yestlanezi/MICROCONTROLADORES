/*******************************************************
This program was created by the CodeWizardAVR V3.49a 
Automatic Program Generator
� Copyright 1998-2023 Pavel Haiduc, HP InfoTech S.R.L.
http://www.hpinfotech.ro

Project : Proyecto - "Ping pong" 
Version : 1.0
Date    : 24/05/2023
Author  : 
Company : 
Comments: 


Chip type               : ATmega8535
Program type            : Application
AVR Core Clock frequency: 1.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 128
*******************************************************/

// I/O Registers definitions
#include <mega8535.h>
#include <delay.h>
#include <stdlib.h>

#define inP1_right PINA.0  //pin 40
#define inP1_left PINA.1  //pin 39
#define inP2_right PINA.2  //pin 38
#define inP2_left PINA.3  //pin 37
#define in_reset PINA.4  //pin 36
#define in_play PINA.5  //pin 35

//Protoypes
void verVector(unsigned char myVector[7]);
void checa_boton (void);
void jugador1_move(int my_opc);
void jugador2_move(int my_opc);
void recorreVector_der();
void recorreVector_izq();
void pelota_move();
void verMarcador();


// Declare your global variables here
unsigned char num[7] = {0x06, 0x00, 0x00, 0x04, 0x00, 0x00, 0x0c};
bit botonp;
bit botona, game = 1;
bit dir; //direccion izq 0 - der 1 
int opc, index_ball;
int dir_ball,timer; // 0 supizq / 1 sup / 2 supder / 3 infder / 4 inf / 5 indizq 
unsigned char scoreP1, scoreP2;
const char tabla7segmentos[10]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x6f};

void main(void){
    // Declare your local variables here
    opc = -1;
    index_ball = 3;
    timer = 0;
    dir = 1;
    scoreP1 = 0;
    scoreP2 = 0;
    dir_ball = 1 ;
    // Input/Output Ports initialization
    // Port A initialization
    // Function: Bit7=Out Bit6=Out Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRA=(1<<DDA7) | (1<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
    // State: Bit7=0 Bit6=0 Bit5=P Bit4=P Bit3=P Bit2=P Bit1=P Bit0=P 
    PORTA=(0<<PORTA7) | (0<<PORTA6) | (1<<PORTA5) | (1<<PORTA4) | (1<<PORTA3) | (1<<PORTA2) | (1<<PORTA1) | (1<<PORTA0);

    // Port B initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
    DDRB=(1<<DDB7) | (1<<DDB6) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

    // Port C initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
    DDRC=(1<<DDC7) | (1<<DDC6) | (1<<DDC5) | (1<<DDC4) | (1<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
    PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

    // Port D initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
    DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

    while (1){
        
        if(scoreP1 > 9 || scoreP2 > 9){
            game = 1;
        }//if
        
        checa_boton();
        if(game == 0){                        
            if(timer > 5){
               timer = 0; 
               pelota_move(); 
            }else{
                timer++;
            }//if-else
       
        }//if
        verVector(num);
        verMarcador();

    }//while
}//main


void verVector(unsigned char myVector[7]){
    unsigned char i, v = 0x01;
        
        for(i = 0; i< 7; i++){
            PORTD = ~v;
            PORTC= myVector[i];   
            delay_ms(3);
            v = v << 1;
        }//for

}//verVector

void checa_boton (void){
    if (inP1_right == 0){
        botona = 0;
        opc = 1;
    }else if(inP1_left == 0){
        botona = 0;
        opc = 2;
    }else if(inP2_right == 0){
        botona = 0;
        opc = 3;
    }else if(inP2_left == 0){
        botona = 0;
        opc = 4;
    }else if(in_reset == 0){
        botona = 0;
        opc = 5;
    }else if(in_play == 0){
        botona = 0;
        opc = 6;
    }else{
        botona=1;
    }//if-else
    
    if((botonp == 1) && (botona == 0)){ //hubo cambio de flanco de 1 a 0 
        
        if(opc == 1 || opc == 2){      
            jugador1_move(opc);
        }else if(opc == 3 || opc == 4){
            jugador2_move(opc);
        }else if(opc == 5){
            scoreP1 = 0;
            scoreP2 = 0;
            game = 1;
        }else{
             game = 0;
        }//if-else
        delay_ms(8); //40ms para eliminar rebotes
    }//if
        
    
    if((botonp == 0) && (botona == 1)){ //hubo cambio de flanco de 0 a 1
        delay_ms(8); //40ms para eliminar rebotes
    }//if  
    botonp = botona;
    opc = -1;
}//checa_boton

void jugador1_move(int my_opc){
    unsigned char temp;
    
    if(my_opc == 1){
        temp = num[0] << 1;
        if(temp > 0x18){
            num[0] = 0x18; 
        }else{
            num[0] = temp;
        }//if-else
    }else{
       temp = num[0] >> 1;
        if(temp < 0x03){
            num[0] = 0x03; 
        }else{
            num[0] = temp;
        }//if-else

    }//if-else

    
}//jugador1_move


void jugador2_move(int my_opc){
    unsigned char temp;
    
    if(my_opc == 3){
        temp = num[6] << 1;
        if(temp > 0x18){
            num[6] = 0x18; 
        }else{
            num[6] = temp;
        }//if-else
    }else{
       temp = num[6] >> 1;
        if(temp < 0x03){
            num[6] = 0x03; 
        }else{
            num[6] = temp;
        }//if-else

    }//if-else

}//jugador2_move

void recorreVector_der(){
    unsigned int r_num = rand() %6;
    if(index_ball + 1 == 6){
        dir = 0;
        if((num[index_ball + 1] ^ num[index_ball]) > num[index_ball + 1]){
            scoreP2++;
            num[index_ball] = 0x00;
            index_ball = 3;
            num[index_ball] = 0x04;
            //game = 1;
        }else{
            if(r_num < 3){
                dir_ball = r_num;
            }else{
                dir_ball = 1;
            }//if-else
 
        }//if-else
        
    }else{
        num[index_ball + 1] = num[index_ball];
        num[index_ball] = 0x00;
        index_ball++;
    }//if-else

}//recorreVector_der

void recorreVector_izq(){
    unsigned int r_num = rand() %6;
    if(index_ball - 1 == 0){
        dir = 1;
        if((num[index_ball - 1] ^ num[index_ball]) > num[index_ball - 1]){
            scoreP1++;
            num[index_ball] = 0x00;
            index_ball = 3;
            num[index_ball] = 0x04;
            //game = 1;
        }else{
            if(r_num >= 3){
                dir_ball = r_num;
            }else{
                dir_ball = 4;
            }//if-else 
        }//if-else
        
    }else{
        num[index_ball - 1] = num[index_ball];
        num[index_ball] = 0x00;
        index_ball--;
    }//if-else

}//recorreVector_izq


void pelota_move(){
     unsigned char temp;
     
     if(dir == 1){ //abajo
        switch(dir_ball){
            case 3:{
                temp = num[index_ball] >> 1;
                if(temp != 0x00){
                   num[index_ball] = temp; 
                   recorreVector_der(); 
                }else{
                    dir_ball = 5;
                    recorreVector_der(); 
                }//if-else 
                
                break;
            }
            case 4:{
                recorreVector_der();
                break;
            }
            case 5:{
                temp = num[index_ball] << 1;
                if(temp != 0x10){
                    num[index_ball] = temp;
                   recorreVector_der(); 
                }else{
                    dir_ball = 3;
                    recorreVector_der(); 
                }//if-else
                break;
            }
            default:
                dir_ball = 4;
            break;
        }//switch
        
     }else{
        switch(dir_ball){
            case 2:{
                temp = num[index_ball] >> 1;
                if(temp != 0x00){
                    num[index_ball] = temp;
                   recorreVector_izq(); 
                }else{
                    dir_ball = 0;
                    recorreVector_izq(); 
                }//if-else
                break;
            }
            case 1:{
                recorreVector_izq();
                break;
            }
            case 0:{
                temp = num[index_ball] << 1;
                if(temp != 0x10){
                    num[index_ball] = temp;
                   recorreVector_izq(); 
                }else{
                    dir_ball = 2;
                    recorreVector_izq(); 
                }//if-else
                break;
            }
            default:
                dir_ball = 1;
            break;
        }//switch

     }//if-else
     
}//pelota_move


void verMarcador(){
    
   PORTA.7 = 0;
    PORTB = 0x00;
    PORTA.6 = 1;
    PORTB = tabla7segmentos[scoreP1];
    delay_ms(2);
    
    PORTA.6 = 0;
    PORTB = 0x00;
    PORTA.7 = 1;  
    PORTB = tabla7segmentos[scoreP2];
    delay_ms(2);

}//verMarcador


