/***************************************************************** 
 * UTFT Touch Acces Control
 * This program is a demo of how to use the touch module and screen 
 * LCD.
 * Display buttons to enter the password and if this is correct
 * grant access to the application.
 * this program was made with a screen LCD of 320x240 pixels
 * This program requires the UTFT library
 *****************************************************************/
#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>
/***************************************************************** 
 *  Macros 
 *****************************************************************/
 #define RS_PIN_LCD 38
 #define WR_PIN_LCD 39
 #define CS_PIN_LCD 40
 #define RST_PIN_LCD 41

 #define TCLK_PIN_TCH 6
 #define TCS_PIN_TCH 5
 #define TDIN_PIN_TCH 4
 #define TDOUT_PIN_TCH 3
 #define RST_PIN_TCH 2
/***************************************************************** 
 *Initialize display
 *
 * Standard Arduino Mega/Due shield : 
 * UTFT (Model, RS, WR, CS, RST)
 * Model: Supported displays in this case ITDB32S.
 * RS: Pin for Register Select
 * WR: Pin for Write
 * CS: Pin for Chip Select
 * RST: Pin for Reset
 * <display model>,38,39,40,41 
 *****************************************************************/
UTFT my_Graphic_LCD(ITDB32S,RS_PIN_LCD,WR_PIN_LCD,CS_PIN_LCD,RST_PIN_LCD);

/*****************************************************************
 * Initialize touchscreen
 * Standard Arduino Mega/Due shield
 * UTouch (TCLK, TCS, TDIN, TDOUT,IRQ)
 * TCLK: Pin for Touch Clock (D_CLK)
 * TCS: Pin for Touch Chip Select
 * TDIN: Pin for Touch Data Input (D_DIN)
 * TDOUT: Pin for Touch Data Output (D_OUT)
 * IRQ: Pin for Touch IRQ (DPenirq)
 *****************************************************************/
UTouch my_Touch_LCD(TCLK_PIN_TCH,TCS_PIN_TCH,TDIN_PIN_TCH,TDOUT_PIN_TCH,RST_PIN_TCH);

/*****************************************************************
 * Setup UTFT Buttons
 * UTFT_Buttons(UTFT, UTouch)
 * UTFT: reference to an already created UTFT Object
 * UTouch: reference to an already created UTouch Object
 *****************************************************************/
 UTFT_Buttons myButtons(&my_Graphic_LCD, &my_Touch_LCD);
 
/*****************************************************************
 * Declare which fonts we will be using
 *****************************************************************/
extern uint8_t BigFont[];
extern uint8_t SmallFont[];
extern uint8_t SevenSegNumFont[];
extern uint8_t Dingbats1_XL[];

/*****************************************************************
 * Global Variables
 *****************************************************************/
int x_coordinate;
int y_coordinate;
char stCurrent[20]="";
int stCurrentLen=0;
char stLast[20]="";
int enter_button;
int clear_button;
int button_num_1,button_num_2,button_num_3,button_num_4,button_num_5,button_num_6,button_num_7,button_num_8,button_num_9;
int button_num_0,button_retro,button_empty;
int index = 0;
int pass[4];

/*****************************************************************
 * Function drawButtons
 * Description: Function to create the graphic buttons on the LCD
 * Parameters: None
 * Return: None
 *****************************************************************/
 void draw_Nine_Buttons(void)
 {
  int row;
  int column;
  int max_num_buttons_per_line = 3;
  int max_num_column = 4;
  /* Clear LCD Screen background will be set to black */
  my_Graphic_LCD.clrScr();
  
  /* Create and matrix of buttons */
  for (row = 0; row < max_num_buttons_per_line; row++)
  {
    for (column = 0; column < max_num_column; column++)
    {
       /* Set Color r,g,b */
       my_Graphic_LCD.setColor(0,0,255); /* Blue Color */
       /* Draw a filled rectangle with slightly rounded */
       my_Graphic_LCD.fillRoundRect(5+(row*60),5+(column*60),55+(row*60),55+(column*60));
       /* Set Color r,g,b */
       my_Graphic_LCD.setColor(255,255,255); /* White Color */
       /* Draw a rectangle with slightly rounder corners */
       my_Graphic_LCD.drawRoundRect(5+(row*60),5+(column*60),55+(row*60),55+(column*60)); 
    }
  }
}
void draw_numbers_on_buttons(void)
{
  my_Graphic_LCD.setFont(BigFont);
  my_Graphic_LCD.setBackColor(0, 0, 255); /*Set blue bckgrd */
  /* Draw numbers into the buttons */
  my_Graphic_LCD.print("1",  22,  20);
  my_Graphic_LCD.print("2",  82,  20);
  my_Graphic_LCD.print("3", 142,  20);
  my_Graphic_LCD.print("4",  22,  80);
  my_Graphic_LCD.print("5",  82,  80);
  my_Graphic_LCD.print("6", 142,  80);
  my_Graphic_LCD.print("7",  22, 140);
  my_Graphic_LCD.print("8",  82, 140);
  my_Graphic_LCD.print("9", 142, 140);
  /* Clear */
  my_Graphic_LCD.print("#",  22, 200);
  my_Graphic_LCD.print("0",  82, 200);
  /* Enter */
  my_Graphic_LCD.print("*", 142, 200);
}
void init_display_message(void)
{
  my_Graphic_LCD.setColor(0, 255, 0); /* Set color green */
  my_Graphic_LCD.fillRect(0, 0, 319, 13); /* Top frame */
  my_Graphic_LCD.setColor(0, 255, 0); /* Set color green */
  my_Graphic_LCD.fillRect(0, 226, 319, 239); /* low frame */
  my_Graphic_LCD.setColor(255, 255, 255); /*Set White Color to FONT*/
  my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
  my_Graphic_LCD.print("* Iniciando *", CENTER, 30);
  my_Graphic_LCD.setFont(SmallFont);
  my_Graphic_LCD.print("*************************", CENTER, 70);
  my_Graphic_LCD.print("* Introduce el password *", CENTER, 90);
  my_Graphic_LCD.print("* correcto para entrar  *", CENTER, 110);
  my_Graphic_LCD.print("*    a la aplicacion.   *", CENTER, 130);
  my_Graphic_LCD.print("*************************", CENTER, 150);
  delay(1000); /*Wait 1 sec */
}
void draw_buttons_lib()
{
  /* Clear LCD Screen background will be set to black */
  my_Graphic_LCD.clrScr();
  
  button_num_1 = myButtons.addButton(   5,   5,  50,  50, "1");
  button_num_2 = myButtons.addButton(  60,   5,  50,  50, "2");
  button_num_3 = myButtons.addButton( 115,   5,  50,  50, "3");
  button_num_4 = myButtons.addButton(   5,  60,  50,  50, "4");
  button_num_5 = myButtons.addButton(  60,  60,  50,  50, "5");
  button_num_6 = myButtons.addButton( 115,  60,  50,  50, "6");
  button_num_7 = myButtons.addButton(   5, 115,  50,  50, "7");
  button_num_8 = myButtons.addButton(  60, 115,  50,  50, "8");
  button_num_9 = myButtons.addButton( 115, 115,  50,  50, "9");
  button_retro = myButtons.addButton(   5, 170,  50,  50, "<-");
  button_num_0 = myButtons.addButton(  60, 170,  50,  50, "0");
  button_empty = myButtons.addButton( 115, 170,  50,  50, " ");
  enter_button = myButtons.addButton( 185,   5, 125,  50, "Enter");
  clear_button = myButtons.addButton( 185,  65, 125,  50, "Clear");
  myButtons.drawButtons();
}

void set_sev_seg_num(int num)
{
  my_Graphic_LCD.setColor(0, 255, 0);      /*Set Green Color to FONT*/
  my_Graphic_LCD.setBackColor(0, 0, 0);    /*Set black bckgrd */
  my_Graphic_LCD.setFont(SevenSegNumFont); /*Set Seven Segment font type */
  
  switch (num)
  {
    case 0:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("0", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 0;
        /*increment one point in the array */
        index++;
      }
      break;
    }
    case 1:
    {      
      if (index < 4)
      {
        my_Graphic_LCD.print("1", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 1;
        /*increment one point in the array */
        index++;
      }
      break;
    }
    case 2:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("2", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 2;
        /*increment one point in the array */
        index++;
      }
      break;
    }
    case 3:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("3", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 3;
        /*increment one point in the array */
        index++;
      }
      break;
    }
    case 4:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("4", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 4;
        /*increment one point in the array */
        index++;
      }
      break;
    }
    case 5:
    {  
      if (index < 4)
      {
        my_Graphic_LCD.print("5", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 5;
        /*increment one point in the array */
        index++;
      }
      break;
    }
    case 6:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("6", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 6;
        /*increment one point in the array */
        index++;
      }
      break;
    }
    case 7:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("7", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 7;
        /*increment one point in the array */
        index++;
      }
      break;
    }
    case 8:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("8", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 9;
        /*increment one point in the array */
        index++;
      }
      break;
    }
    case 9:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("9", (index*30)+180, 135); /* Set the num on the display */
        pass[index] = 9;
        /*increment one point in the array */
        index++;
      }
      break;
    }
  }
}

void clear_last_num()
{
  my_Graphic_LCD.setColor(0, 255, 0);      /*Set Green Color to FONT*/
  my_Graphic_LCD.setBackColor(0, 0, 0);    /*Set black bckgrd */
  my_Graphic_LCD.setFont(SevenSegNumFont); /*Set Seven Segment font type */
  
  if (index <= 4)
  {
    /* Clear the last number on the display */
    my_Graphic_LCD.setColor(0, 0, 0); /* Set Black Color */
    my_Graphic_LCD.fillRect((index*30)+180, 135, (index*30)+212, 185); /* Black Frame */
    /* Clear the last number on the array */
    pass[index] = NULL;

    /*Check if the number is valid*/
    if (index == 0)
    {
      /*Remain the last state [0] */
    }
    else
    {
      /*Decrement one point */
      index--;
    }
  }
}

void setup()
{
  /* Setup Serial port at 9600 bps */
  Serial.begin(9600);
  /* Initialize the LCD Module LANDSCAPE is the default orientation */
  my_Graphic_LCD.InitLCD();
  /* Clear LCD Screen background will be set to black */
  my_Graphic_LCD.clrScr();
  /* Set the precision of the Touch Screen */
  my_Touch_LCD.setPrecision(PREC_MEDIUM);
  /* Select font to use */
  my_Graphic_LCD.setFont(BigFont);
  /* Set background color */
  my_Graphic_LCD.setBackColor(0, 0, 255);/* Blue Color */
  /* Set the text font to use for buttons labels */
  myButtons.setTextFont(BigFont);
  /* Set which font to use for buttons symbols */
  myButtons.setSymbolFont(Dingbats1_XL);
  /* Show a initial message */
  init_display_message();
  /* Initialize the Touch Module LANDSCAPE is the default orientation */
  my_Touch_LCD.InitTouch();
  /*Call function to create the buttons on the display */
  draw_buttons_lib();
}

void loop()
{
  int pressed_button;
  int i;
  
  while(true)
  {
    if (my_Touch_LCD.dataAvailable() == true)
    {
      pressed_button = myButtons.checkButtons();

      if (pressed_button == enter_button)
      {
        my_Graphic_LCD.setColor(255, 255, 255); /*Set White Color to FONT*/
        my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("Enter Pressed", 185, 200);

        for ( i = 0; i < 4; i++)
        {
          if (pass[i] = NULL)
          {
            my_Graphic_LCD.print("Enter 4 numbers", 185, 200);
            break;
          }
          else
          {
            Serial.print('A');
            Serial.println(pass[i]);
          }
        }
      }
      else if (pressed_button == clear_button)
      {
        my_Graphic_LCD.setColor(255, 255, 255); /*Set White Color to FONT*/
        my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("Clear Pressed", 185, 200);
        
        my_Graphic_LCD.setColor(0, 0, 0); /* Set Black Color */
        my_Graphic_LCD.fillRect(180, 120, 310, 200); /* Black Frame */

        /*Clear all values of the array */
        for (i=0; i<4;i++)
        {
          pass[i]=NULL;
        }
        /* Set the index to the initial value */
        index = 0;
      }
      else if (pressed_button == button_num_1)
      {
        /*Set 1 to seven segment display*/
        set_sev_seg_num(1);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_num_2)
      {
        /*Set 2 to seven segment display*/
        set_sev_seg_num(2);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_num_3)
      {
        /*Set 3 to seven segment display*/
        set_sev_seg_num(3);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_num_4)
      {
        /*Set 4 to seven segment display*/
        set_sev_seg_num(4);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_num_5)
      {
        /*Set 5 to seven segment display*/
        set_sev_seg_num(5);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_num_6)
      {
        /*Set 6 to seven segment display*/
        set_sev_seg_num(6);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_num_7)
      {
        /*Set 7 to seven segment display*/
        set_sev_seg_num(7);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_num_8)
      {
        /*Set 8 to seven segment display*/
        set_sev_seg_num(8);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_num_9)
      {
        /*Set 9 to seven segment display*/
        set_sev_seg_num(9);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_num_0)
      {
        /*Set 0 to seven segment display*/
        set_sev_seg_num(0);
        /* Clear if the messages is active */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_retro)
      {
        /*Clear the last number entered*/
        clear_last_num();
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
      else if (pressed_button == button_empty)
      {
        /* Do nothing */
      }
      else
      {
        my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("             ", 185, 200);
      }
    }
  }
}
