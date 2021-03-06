/****************************************************************/
/*   Author             :    Ahmed Atia Atia 				    */
/*	 Date 				:    20 May 2019 						*/
/*	 Version 			:    1.0V 							 	*/
/*	 Description 		:    prog.c for USART				 	*/
/****************************************************************/

/****************************************************************/
/*********************** STD LIB DIRECTIVES *********************/
/****************************************************************/

#include "Std_types.h"
#include "Bit_Math.h"
#include "Lbt_int.h"

/****************************************************************/
/*********************** Component DIRECTIVES *******************/
/****************************************************************/

#include "IIC_int.h"
#include "IIC_config.h"
#include "IIC_priv.h" 


/****************************************************************/
/*********************** Function Implementation  ***************/
/****************************************************************/


/****************************************************************/
/* Description    :  This function used to initialize USART     */
/*					 Inputs : void 								*/
/*					 return : void		 						*/
/****************************************************************/


void IIC_Init( u8 SlaveAddress)
{
		
	/*	BIT Rate (Clock) 	*/	
	#if BUS_CLOCK == 100	
	/**	Comment!	TWBR = 32 	*/
		CLEAR_BIT( TWBR , 7);
		CLEAR_BIT( TWBR , 6);
		SET_BIT( TWBR , 5);
		CLEAR_BIT( TWBR , 4);
		CLEAR_BIT( TWBR , 3);
		CLEAR_BIT( TWBR , 2);
		CLEAR_BIT( TWBR , 1);
		CLEAR_BIT( TWBR , 0);
	/** Comment!	prescaller is 00	*/
		CLEAR_BIT( TWSR , 1);
		CLEAR_BIT( TWSR , 0);

	#elif BUS_CLOCK == 400	
	/**	Comment!	TWBR = 32 	*/
		CLEAR_BIT( TWBR , 7);
		CLEAR_BIT( TWBR , 6);
		SET_BIT( TWBR , 5);
		CLEAR_BIT( TWBR , 4);
		CLEAR_BIT( TWBR , 3);
		CLEAR_BIT( TWBR , 2);
		CLEAR_BIT( TWBR , 1);
		CLEAR_BIT( TWBR , 0);
	/** Comment!	prescaller is 00	*/
		CLEAR_BIT( TWSR , 1);
		CLEAR_BIT( TWSR , 0);
		
	#endif	
	
	/** Comment!	Enable Acknowledge Bit	*/	
	SET_BIT( TWCR , 6);
	
	
	
	/** Comment!	Enable IIC Bit	*/	
	SET_BIT( TWCR , 2);
	
	
	
	
	
	
	
	
	u16 LOC_baudEquation = 0 ;

	/*	Operation mode Synchronous or Asynchronous	*/	
	#if MODE_OF_OPERATION == ASYNCHRONOUS
		CLEAR_BIT(UCSRC, 6);
	
	#elif MODE_OF_OPERATION == SYNCHRONOUS
		SET_BIT( UCSRC , 6 );
	
	#endif
	
	
	/*	Parity mode Disabled or Odd or Even 	*/	
	#if PARITY_MODE	== DISABLED
		CLEAR_BIT(UCSRC, 4 );
		CLEAR_BIT(UCSRC, 5 );
		
	#elif PARITY_MODE	== EVEN
		CLEAR_BIT(UCSRC, 4 );
		SET_BIT( UCSRC , 5 );

	#elif PARITY_MODE	== ODD
		SET_BIT( UCSRC , 4 );
		SET_BIT( UCSRC , 5 );
		
	#endif
	
	
	/*	Speed mode normal or Double		*/
	#if MODE_OF_OPERATION == ASYNCHRONOUS
	
		if( SPEED_OPERATION	== DOUBLE_SPEED_MODE )    
			SET_BIT(UCSRA, 1);
	
		else if( SPEED_OPERATION == NORMAL_SPEED_MODE )		
			CLEAR_BIT(UCSRA, 1);
	
	#elif MODE_OF_OPERATION == SYNCHRONOUS
		CLEAR_BIT(UCSRA, 1);
	
	#endif
	
	/*  Baud rate Equation	*/
	#if MODE_OF_OPERATION == ASYNCHRONOUS
			if (GET_BIT(UCSRA, 1) == 0) 
			/* Asynchronous Normal Mode */
			LOC_baudEquation = ((F_CPU / (16 * BAUD_RATE)) - 1 );
	
			else if (GET_BIT(UCSRA, 1) == 1)
			/* Asynchronous Double Speed Mode */
			LOC_baudEquation = ((F_CPU / (8 * BAUD_RATE)) - 1 );

	#elif MODE_OF_OPERATION == SYNCHRONOUS
		/* Synchronous Master Mode */
		LOC_baudEquation = ((F_CPU / (2 * BAUD_RATE)) - 1 );
	
	#endif
	
	
	/* Set baud rate */
	UBRRH = (u8)(LOC_baudEquation>>8);
	UBRRL = (u8)LOC_baudEquation;
	
	
	/* Enable receiver and transmitter */
	/*UCSRB = (1<<RXEN)|(1<<TXEN);*/
	SET_BIT( UCSRB , 3 );
	SET_BIT( UCSRB , 4 );


	
	/*	 Character Size N-Bits	*/		
	#if FRAME_SIZE	== 5
		CLEAR_BIT( UCSRC , 1 );
		CLEAR_BIT( UCSRC , 2 );
		CLEAR_BIT( UCSRB , 2 );
		
	#elif FRAME_SIZE == 6
		SET_BIT( UCSRC , 1 );
		CLEAR_BIT( UCSRC , 2 );
		CLEAR_BIT( UCSRB , 2 );
		
	#elif FRAME_SIZE == 7
		CLEAR_BIT( UCSRC , 1 );
		SET_BIT( UCSRC , 2 );
		CLEAR_BIT( UCSRB , 2 );
		
	#elif FRAME_SIZE == 8
		SET_BIT( UCSRC , 1 );
		SET_BIT( UCSRC , 2 );
		CLEAR_BIT( UCSRB , 2 );

	#elif FRAME_SIZE == 9
		SET_BIT( UCSRC , 1 );
		SET_BIT( UCSRC , 2 );
		SET_BIT( UCSRB , 2 );
		
	#endif

	/* Set frame format: 8-Bits data */
	/*	 Character Size 8-Bits	*/
	SET_BIT( UCSRC , 1 );
	SET_BIT( UCSRC , 2 );

	/*	Stop bit select number One or Two Bits */	
	#if STOP_BIT_SELECT	== TWO_BIT
			SET_BIT( UCSRC , 3 );
	
	#elif STOP_BIT_SELECT	== ONE_BIT
			CLEAR_BIT( UCSRC , 3 );
	
	#endif	

	/*  clock polarity	*/
	#if MODE_OF_OPERATION == SYNCHRONOUS
	
		#if CLOCK_POLARITY	== RISING
			CLEAR_BIT( UCSRC , 0 );
	
		#elif CLOCK_POLARITY	== FALLING
			SET_BIT( UCSRC , 0 );
	
		#endif	
	
	#endif

	
/****************************************************************/
/** !comment :Bit 7 – URSEL: Register Select					*/
/** 		  This bit selects between accessing the UCSRC or 	*/
/*							the UBRRH Register.					*/
/*				It is read as one when reading UCSRC.			*/
/*				The URSEL must be one when writing the UCSRC.	*/
/****************************************************************/

	SET_BIT( UCSRC , 7 );



}



/****************************************************************/
/* Description    : This function used to Send Data				*/ 
/*																*/
/*					Inputs : u8 data				  			*/
/*					Return : void		 					    */
/****************************************************************/
/* Pre_condition  :  this function must be used after USART		*/
/*     				 initialized 							    */
/****************************************************************/
void USART_Transmit( u8 data )
{
	
	/* Wait for empty transmit buffer */
	while( GET_BIT(UCSRA,5) == 0 );
	
	/*	while ( !( UCSRA & (1<<UDRE)) );	*/
	
	/* Put data into buffer, sends the data */
	UDR = data;

}

/****************************************************************/
/* Description    : This function used to Receive Data			*/ 
/*																*/
/*					Inputs : void					  			*/
/*					Return : u8			 					    */
/****************************************************************/
/* Pre_condition  :  this function must be used after USART		*/
/*     				 initialized 							    */
/****************************************************************/
u8 USART_Receive( )
{
	/* Wait for data to be received */
	while( GET_BIT( UCSRA , 7 ) ==0 );
	
	/*while ( !(UCSRA & (1<<RXC)) );*/
	
	/* Get and return received data from buffer */
	return UDR;
}

/***********************************************************************************************/
/************************************* END OF PROGRAM ******************************************/
/***********************************************************************************************/