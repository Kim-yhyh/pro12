

#include "AT89S52.H"				


#define  U_C      unsigned char
#define  U_I      unsigned int
#define  U_L      unsigned long

unsigned char FLG1=0, FLG2=0;				
unsigned char RX_DATA, FL1=0, FL2=0;



Delay_us(unsigned char us)	//us지연함수 
{ unsigned char i; 		 
  for(i = 1; i <= us; i++);
} 

Delay_ms(unsigned int ms)	//time delay for ms[ms] 
{ unsigned int i, j;
  for(i = 1; i <= ms; i++)

  for(j = 1; j <= 125; j++);	
}



OPEN(void)			//창문 오픈
 { if(FLG1 == 0) 		 
   { P1_0 = 1;        
     Delay_ms(6000);		//
     P1_0 = 0;    
     FLG1 = 1;
 } }  
    		 
CLOSE(void)			//창문 클로즈.
 { if(FLG1 == 1) 		 
   { P1_1 = 1;        
     Delay_ms(6000);		
     P1_1 = 0;    
     FLG1 = 0;
 } } 
 
OPEN1(void)			//가스벨브 오픈
 { if(FLG2 == 0) 		 
   { P1_2 = 1;        
     Delay_ms(3000);
     P1_2 = 0;    
     FLG2 = 1;
 } }  
    		 
CLOSE1(void)			//가스벨브 클로즈.
 { if(FLG2 ==1) 		 
   { P1_3 =1;        
     Delay_ms(3000);		
     P1_3 =0;    
     FLG2 =0;
 } } 


void TIRI_ISR(void) interrupt 4	// serial int. service routine  
{ if(RI)			// receive ready ?
   { 
     RX_DATA = (SBUF);		// if yes, receive and RI=0      

     RI = 0;			 
    		
    }
}


main()         		
{   TMOD=0x22;			//블루투스 통신가능 하게. 
    SCON=0x50; 
    TH1=0xFD;     		

    TR1=1; EA=1;  ES=1;		//인터럽트 허용.	  



          
 while(1){ 
 	   
  if(RX_DATA=='0') FL1=1;	//수동 모드
  if(RX_DATA=='9') FL1=0;	//자동 모드.

//-------- 자 동  모 드 -------------------    	
  if(!(FL1))
   {if(!(P2_0)) CLOSE(); //낮이면 닫기 
    else        OPEN();  //밤이면 열기
 
    if(!(P2_1)) CLOSE1();//낮이면 닫기 
    else        OPEN1(); //밤이면 열기
   }

//--------- 수 동  모 드 ------------------
  else 
   {if(RX_DATA=='1') OPEN();
    if(RX_DATA=='2') CLOSE();
    if(RX_DATA=='3') OPEN1();
    if(RX_DATA=='4') CLOSE1();    
    if(RX_DATA=='5') {P1_4 =1; FL2=1;}
    if(RX_DATA=='6') {P1_4 =0; FL2=0;}  
   }

//--------- 상 태  조 회 --------------------    
  if(RX_DATA=='7') 
     {
     
    
      if(FLG1) {TX0_STR((char*)"WINDOW OPEN !");}
      else     {TX0_STR((char*)"WINDOW CLOSE !");}     
      
     	
      
      
      if(FLG2) {TX0_STR((char*)"GAS OPEN !");}
      else     {TX0_STR((char*)"GAS CLOSE !");}     
      
     
      
      
      if(FL2) {TX0_STR((char*)"FAN ON !");}
      else    {TX0_STR((char*)"FAN OFF !");}     
      
     
     }   	   
 }	
}    
    
