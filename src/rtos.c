/*=============================================================================
 * Copyright (c) 2020, felipesarche <sarche_2004@hotmail.com>
 * All rights reserved.
 * License:  (see LICENSE.txt)
 * Date: 2020/05/21
 * Version: 1
 *===========================================================================*/

/*=====[Inclusions of function dependencies]=================================*/

#define SAPI_USE_INTERRUPTS
#include "rtos.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "sapi.h"
#include "semphr.h"
#include "userTasks.h"
#include "sapi_print.h"
#include "supporting_functions.h"
#include "rtcLayer.h"
#include "tiraled.h"




/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/
rtclock_t rtc;
/*=====[Definitions of private global variables]=============================*/

/*=====[Main function, program entry point after power on or reset]==========*/

SemaphoreHandle_t SEM1;
SemaphoreHandle_t SEM2;
SemaphoreHandle_t SEMTONEMARIO;
SemaphoreHandle_t SEMTONEMEME;
SemaphoreHandle_t SEMLED;
SemaphoreHandle_t SEMCORTINAOPEN;
SemaphoreHandle_t SEMCORTINACLOSE;
SemaphoreHandle_t SEMDESPERTADOR;

rgb_t tira;
alarma_t despertador;

void onRxBt( void *noUsado )
{
	BaseType_t despiertoprima=pdFALSE;
	static uint8_t nRecepcion;
    static uint8_t vec[20];
	char f = uartRxRead( UART_232 );
	vec[nRecepcion]=f;
	nRecepcion=nRecepcion+1;
	if((f == '#')||(nRecepcion>19))
	{
		if(vec[0]=='$')
		{
			if(vec[1]=='A')
			{
				xSemaphoreGiveFromISR(SEM1,&despiertoprima);
			}
			if(vec[1]=='B')
			{
				xSemaphoreGiveFromISR(SEM2,&despiertoprima);
			}
			if(vec[1]=='C')
			{
				xSemaphoreGiveFromISR(SEMTONEMARIO,&despiertoprima);
			}
			if(vec[1]=='D')
			{
				xSemaphoreGiveFromISR(SEMTONEMEME,&despiertoprima);
			}
			if(vec[1]=='E')
			{
				xSemaphoreGiveFromISR(SEMCORTINAOPEN,&despiertoprima);
			}
			if(vec[1]=='F')
			{
				xSemaphoreGiveFromISR(SEMCORTINACLOSE,&despiertoprima);
			}
			if(vec[1]=='Z')
			{

				despertador.hour=vec[2];
				despertador.min=vec[3];
				xSemaphoreGiveFromISR(SEMDESPERTADOR,&despiertoprima);

			}
			if(vec[1]=='L')
			{
				tira.blue=vec[2];
				tira.green=vec[3];
				tira.red=vec[4];
				xSemaphoreGiveFromISR(SEMLED,&despiertoprima);
			}

		}
		nRecepcion=0;
	}
	//printf( "Recibimos <<%c>> por UART\r\n", f);
	portYIELD_FROM_ISR(despiertoprima);
}
void onRx( void *noUsado )
{
	BaseType_t despierto=pdFALSE;
	static uint8_t nRecepcion;
    static uint8_t vec[20];
	char c = uartRxRead( UART_USB );
	vec[nRecepcion]=c;
	nRecepcion=nRecepcion+1;
	if((c == '#')||(nRecepcion>19))
	{
		if(vec[0]=='$')
		{
			if(vec[1]=='A')
			{
				xSemaphoreGiveFromISR(SEM1,&despierto);
			}
			if(vec[1]=='B')
			{
				xSemaphoreGiveFromISR(SEM2,&despierto);
			}
			if(vec[1]=='C')
			{
				xSemaphoreGiveFromISR(SEMTONEMARIO,&despierto);
			}
			if(vec[1]=='D')
			{
				xSemaphoreGiveFromISR(SEMTONEMEME,&despierto);
			}
			if(vec[1]=='E')
			{
				xSemaphoreGiveFromISR(SEMCORTINAOPEN,&despierto);
			}
			if(vec[1]=='F')
			{
				xSemaphoreGiveFromISR(SEMCORTINACLOSE,&despierto);
			}
			if(vec[1]=='Z')
			{

				despertador.hour=vec[2];
				despertador.min=vec[3];
				xSemaphoreGiveFromISR(SEMDESPERTADOR,&despierto);

			}
			if(vec[1]=='L')
			{
				tira.blue=vec[2];
				tira.green=vec[3];
				tira.red=vec[4];
				xSemaphoreGiveFromISR(SEMLED,&despierto);
			}
		}
		nRecepcion=0;
	}
	//printf( "Recibimos <<%c>> por UART,%u\r\n", c,nRecepcion );
	portYIELD_FROM_ISR(despierto);
}
int main( void )
{



	   SEM1=xSemaphoreCreateBinary();//creación semáforo para encender foco

	   SEM2=xSemaphoreCreateBinary();//creación semáforo para apagar foco

	   SEMTONEMARIO=xSemaphoreCreateBinary();//creación semáforo tocar melodía de mario

	   SEMTONEMEME=xSemaphoreCreateBinary();//creación semáforo tocar melodía meme

	   SEMLED=xSemaphoreCreateBinary();//creación semáforo tira led

	   SEMCORTINAOPEN=xSemaphoreCreateBinary();//creación semáforo abrir cortina

	   SEMCORTINACLOSE=xSemaphoreCreateBinary();//creación semáforo cerrar cortina

	   SEMDESPERTADOR=xSemaphoreCreateBinary();//creación semáforo para activar alarma

	   boardInit();

	   tiraled_init();
	   gpioInit( T_FIL2, GPIO_OUTPUT );

	   uartConfig(UART_USB, 9600);/* Inicializar la UART_USB junto con las interrupciones de Tx y Rx */

	   uartCallbackSet(UART_USB, UART_RECEIVE, onRx, NULL);// Seteo un callback al evento de recepcion y habilito su interrupcion

	   uartInterrupt(UART_USB, true);// Habilito todas las interrupciones de UART_USB


	   uartConfig(UART_232, 9600);/* Inicializar la UART_232 junto con las interrupciones de Tx y Rx */

	   uartCallbackSet(UART_232, UART_RECEIVE, onRxBt, NULL);// Seteo un callback al evento de recepcion y habilito su interrupcion

	   uartInterrupt(UART_232, true);// Habilito todas las interrupciones de UART_232


   // Create a task in freeRTOS with dynamic memory
	  BaseType_t tarea1=xTaskCreate(
			   ledOn,                     // Function that implements the task.
			   (const char *)"ENCENDER LUZ",     // Text name for the task.
			   configMINIMAL_STACK_SIZE*1, // Stack size in words, not bytes.
			   0,                          // Parameter passed into the task.
			   tskIDLE_PRIORITY+1,         // Priority at which the task is created.
			   0                           // Pointer to the task created in the system
	   );
	   BaseType_t tarea3=xTaskCreate(
			   ledOff,                     // Function that implements the task.
			   (const char *)"APAGAR LUZ",     // Text name for the task.
			   configMINIMAL_STACK_SIZE*1, // Stack size in words, not bytes.
			   0,                          // Parameter passed into the task.
			   tskIDLE_PRIORITY+1,         // Priority at which the task is created.
			   0                           // Pointer to the task created in the system
	   );
	   BaseType_t tarea2  =xTaskCreate(
			   myTask2,                     // Function that implements the task.
			   (const char *)"MOSTRAR HORA",     // Text name for the task.
			   configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
			   0,                          // Parameter passed into the task.
			   tskIDLE_PRIORITY+4,         // Priority at which the task is created.
			   0                           // Pointer to the task created in the system
	   );
	   BaseType_t tarea4  =xTaskCreate(
			   playMarioSound,                     // Function that implements the task.
			   (const char *)"PLAY MARIO",     // Text name for the task.
			   configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
			   0,                          // Parameter passed into the task.
			   tskIDLE_PRIORITY+5,         // Priority at which the task is created.
			   0                           // Pointer to the task created in the system
	   );
	   BaseType_t tarea5  =xTaskCreate(
			   manejoDeLed,                     // Function that implements the task.
			   (const char *)"MANEJO LEDS",     // Text name for the task.
			   configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
			   0,                          // Parameter passed into the task.
			   tskIDLE_PRIORITY+5,         // Priority at which the task is created.
			   0                           // Pointer to the task created in the system
	   );

	   BaseType_t tarea6  =xTaskCreate(
			   playMemeSound,                     // Function that implements the task.
			   (const char *)"PLAY MEME",     // Text name for the task.
			   configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
			   0,                          // Parameter passed into the task.
			   tskIDLE_PRIORITY+5,         // Priority at which the task is created.
			   0                           // Pointer to the task created in the system
	   );
	   BaseType_t tarea7  =xTaskCreate(
			   cortinaOpen,                     // Function that implements the task.
			   (const char *)"Abrir cortina",     // Text name for the task.
			   configMINIMAL_STACK_SIZE*1, // Stack size in words, not bytes.
			   0,                          // Parameter passed into the task.
			   tskIDLE_PRIORITY+5,         // Priority at which the task is created.
			   0                           // Pointer to the task created in the system
	   );
	   BaseType_t tarea8  =xTaskCreate(
			   cortinaClose,                     // Function that implements the task.
			   (const char *)"Cerrar cortina",     // Text name for the task.
			   configMINIMAL_STACK_SIZE*1, // Stack size in words, not bytes.
			   0,                          // Parameter passed into the task.
			   tskIDLE_PRIORITY+5,         // Priority at which the task is created.
			   0                           // Pointer to the task created in the system
	   );
	   BaseType_t tarea9  =xTaskCreate(
			   alarma,                     // Function that implements the task.
			   (const char *)"desp",     // Text name for the task.
			   configMINIMAL_STACK_SIZE*2, // Stack size in words, not bytes.
			   0,                          // Parameter passed into the task.
			   tskIDLE_PRIORITY+5,         // Priority at which the task is created.
			   0                           // Pointer to the task created in the system
	   );


	//Chequeo de errores de creación de tareas
   if(tarea1 == pdFAIL)
   {
	   printf("No se creo la tarea1");
   }
   else
   {
	   printf(" Creo la tarea1");
   }
   if(tarea2 == pdFAIL)
   {
	   printf("No se creo la tarea2");
   }
   else
   {
	   printf(" Creo la tarea2");
   }
   if(tarea3 == pdFAIL)
   {
	   printf("No se creo la tarea3");
   }
   else
   {
	   printf(" Creo la tarea3");
   }
   if(tarea4 == pdFAIL)
   {
	   printf("No se creo la tarea4");
   }
   else
   {
	   printf(" Creo la tarea4");
   }
   if(tarea5 == pdFAIL)
   {
	   printf("No se creo la tarea5");
   }
   else
   {
	   printf(" Creo la tarea5");
   }
   if(tarea6 == pdFAIL)
   {
	   printf("No se creo la tarea6");
   }
   else
   {
	   printf(" Creo la tarea6");
   }
   if(tarea7 == pdFAIL)
   {
	   printf("No se creo la tarea7");
   }
   else
   {
	   printf(" Creo la tarea7");
   }
   if(tarea8 == pdFAIL)
   {
	   printf("No se creo la tarea8");
   }
   else
   {
	   printf(" Creo la tarea8");
   }
   if(tarea9 == pdFAIL)
   {
	   printf("No se creo la tarea9");
   }
   else
   {
	   printf(" Creo la tarea9");
   }


   vTaskStartScheduler(); // Initialize scheduler
   rtc_config();// Inicializar RTC
   while( true )
	   // If reach heare it means that the scheduler could not start
   {

   }

   // YOU NEVER REACH HERE, because this program runs directly or on a
   // microcontroller and is not called by any Operating System, as in the 
   // case of a PC program.
   return 0;
}
