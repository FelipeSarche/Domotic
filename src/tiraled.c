/*=============================================================================
 * Copyright (c) 2020, felipesarche <sarche_2004@hotmail.com>
 * All rights reserved.
 * License:  (see LICENSE.txt)
 * Date: 2020/05/21
 * Version: 1
 * http://tronicslab.com/gamma-correction-pulsing-leds/
 *===========================================================================*/

/*=====[Inclusion of own header]=============================================*/


#include "sapi.h"
#include "tiraled.h"
/*=====[Inclusions of private function dependencies]=========================*/

/*=====[Definition macros of private constants]==============================*/

/*=====[Private function-like macros]========================================*/

/*=====[Definitions of private data types]===================================*/

/*=====[Definitions of external public global variables]=====================*/
uint8_t redcorrection[8]={0,  5,  21,  47,  83,  130,  187,  255};
uint8_t greencorrection[8]={0,  5,  21,  47,  83,  130,  187,  255};
uint8_t bluecorrection[8]={0,  5,  21,  47,  83,  130,  187,  255};
/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/

/*=====[Prototypes (declarations) of private functions]======================*/

/*=====[Implementations of public functions]=================================*/

void tiraled_init(void)
{
	   bool_t valor = 0;
	   valor = pwmConfig( 0, PWM_ENABLE );
	   valor = pwmConfig( PWM6, PWM_ENABLE_OUTPUT );
	   valor = pwmConfig( PWM7, PWM_ENABLE_OUTPUT );
	   valor = pwmConfig( PWM8, PWM_ENABLE_OUTPUT );
	   valor = pwmConfig( PWM9, PWM_ENABLE_OUTPUT );
	   valor = pwmWrite( PWM7, 127 );
	   valor = pwmWrite( PWM8, 127 );
	   valor = pwmWrite( PWM9, 127 );

}

/*=====[Implementations of interrupt functions]==============================*/

/*=====[Implementations of private functions]================================*/

