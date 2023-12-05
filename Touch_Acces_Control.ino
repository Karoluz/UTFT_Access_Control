/***************************************************************** 
 * UTFT Touch Acces Control
 * This program is a demo of how to use the touch module and screen 
 * LCD.
 * Display buttons to enter the password and if this is correct
 * grant access to the application.
 * this program was made with a screen LCD of 320x240 pixels
 * This program requires the UTFT library
 *  You can find the latest version of the library at 
 *  http://www.RinkyDinkElectronics.com/
 * version used:
 * UTFT 283
 * URTouch 202
 * UTFT_Buttons 103
 *****************************************************************/
#include <UTFT.h>
#include <URTouch.h>
#include <UTFT_Buttons.h>
#include <UTFT_Geometry.h>
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
URTouch my_Touch_LCD(TCLK_PIN_TCH,TCS_PIN_TCH,TDIN_PIN_TCH,TDOUT_PIN_TCH,RST_PIN_TCH);

/*****************************************************************
 * Setup UTFT Buttons
 * UTFT_Buttons(UTFT, UTouch)
 * UTFT: reference to an already created UTFT Object
 * UTouch: reference to an already created UTouch Object
 *****************************************************************/
 UTFT_Buttons myButtons(&my_Graphic_LCD, &my_Touch_LCD);
 
 UTFT_Geometry geo(&my_Graphic_LCD);

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
//int x_coordinate;
//int y_coordinate;
//char stCurrent[20]="";
//int stCurrentLen=0;
//char stLast[20]="";
int enter_button;
int clear_button;
int button_num_1,button_num_2,button_num_3,button_num_4,button_num_5,button_num_6,button_num_7,button_num_8,button_num_9;
int button_num_0,button_retro,button_empty;
int index = 0;
int pass[5]={NULL,NULL,NULL,NULL,NULL};
int storedpass[5]={9,9,9,9,9};

/*****************************************************************
 * Function void init_display_message
 * Description: Function to create the initial message on the LCD
 * Parameters: None
 * Return: None
 *****************************************************************/
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
  delay(4000); /*Wait 4 sec */
}

/*****************************************************************
 * Function void pass_accepted_message
 * Description: Function to create the message on the LCD when pass
 * is correct.
 * Parameters: None
 * Return: None
 *****************************************************************/
void pass_accepted_message(void)
{
  /* Clear LCD Screen background will be set to black */
  my_Graphic_LCD.clrScr();
  my_Graphic_LCD.setFont(BigFont);
  my_Graphic_LCD.setColor(0, 255, 0); /* Set color green */
  my_Graphic_LCD.fillRect(0, 0, 319, 13); /* Top frame */
  my_Graphic_LCD.setColor(0, 255, 0); /* Set color green */
  my_Graphic_LCD.fillRect(0, 226, 319, 239); /* low frame */
  my_Graphic_LCD.setColor(255, 255, 255); /*Set White Color to FONT*/
  my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
  my_Graphic_LCD.print("* Bienvenido *", CENTER, 30);
  my_Graphic_LCD.print("********************", CENTER, 70);
  my_Graphic_LCD.print(" password correcto  ", CENTER, 90);
  my_Graphic_LCD.print(" aplicacion activada", CENTER, 110);
  my_Graphic_LCD.print(" menu aplicacion.   ", CENTER, 130);
  my_Graphic_LCD.print("********************", CENTER, 150);

  /* enter in a loop to simulate an animation */
  while (true)
  {
    for (int i=20; i <= 280; i += 10)
    {
      my_Graphic_LCD.setColor(VGA_YELLOW);
      /* use the UTFT geometry - create a pie */
      geo.drawPie(i,180,10,150,55);
      my_Graphic_LCD.setColor(VGA_BLACK);
      geo.drawPie(i,180,10,150,55);
      delay(5);
    }
    for (int i=280; i >= 20; i -= 10)
    {
      my_Graphic_LCD.setColor(VGA_YELLOW);
      geo.drawPie(i,180,10,280,220);
      my_Graphic_LCD.setColor(VGA_BLACK);
      geo.drawPie(i,180,10,280,220);
      delay(5);
    }
    delay(100);
  }
}

/*****************************************************************
 * Function void draw_buttons_lib
 * Description: Function to create the buttons on the LCD
 * Parameters: None
 * Return: None
 *****************************************************************/
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

/*****************************************************************
 * Function void store_pass_num
 * Description: Function to store the number received
 * Parameters: int num, number received via touch or serial.
 * Return: None
 *****************************************************************/
void store_pass_num(int num)
{
  pass[index] = num;
        
  if (index < 4)
  {
    /*increment one point in the array */
    index++;
  }
}

/*****************************************************************
 * Function void set_sev_seg_num
 * Description: Function to set the number received to display it.
 * Parameters: int num, number received via touch or serial.
 * Return: None
 *****************************************************************/
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
        store_pass_num(num);
      }
      break;
    }
    case 1:
    {      
      if (index < 4)
      {
        my_Graphic_LCD.print("1", (index*30)+180, 135); /* Set the num on the display */
        store_pass_num(num);
      }
      break;
    }
    case 2:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("2", (index*30)+180, 135); /* Set the num on the display */
        store_pass_num(num);
      }
      break;
    }
    case 3:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("3", (index*30)+180, 135); /* Set the num on the display */
        store_pass_num(num);
      }
      break;
    }
    case 4:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("4", (index*30)+180, 135); /* Set the num on the display */
        store_pass_num(num);
      }
      break;
    }
    case 5:
    {  
      if (index < 4)
      {
        my_Graphic_LCD.print("5", (index*30)+180, 135); /* Set the num on the display */
        store_pass_num(num);
      }
      break;
    }
    case 6:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("6", (index*30)+180, 135); /* Set the num on the display */
        store_pass_num(num);
      }
      break;
    }
    case 7:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("7", (index*30)+180, 135); /* Set the num on the display */
        store_pass_num(num);
      }
      break;
    }
    case 8:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("8", (index*30)+180, 135); /* Set the num on the display */
        store_pass_num(num);
      }
      break;
    }
    case 9:
    {
      if (index < 4)
      {
        my_Graphic_LCD.print("9", (index*30)+180, 135); /* Set the num on the display */
        store_pass_num(num);
      }
      break;
    }
  }
}

/*****************************************************************
 * Function void clear_last_num
 * Description: Function to clear the last number in buffer.
 * Parameters: None
 * Return: None
 *****************************************************************/
void clear_last_num()
{
  my_Graphic_LCD.setColor(0, 255, 0);      /*Set Green Color to FONT*/
  my_Graphic_LCD.setBackColor(0, 0, 0);    /*Set black bckgrd */
  my_Graphic_LCD.setFont(SevenSegNumFont); /*Set Seven Segment font type */
  
  if (index < 5)
  {
    my_Graphic_LCD.setColor(0, 0, 0); /* Set Black Color */
    /* Clear the last number on the display */
    my_Graphic_LCD.fillRect((index*30)+180, 135, (index*30)+212, 185); /* Black Frame */
        
    /*Check if the number is valid*/
    if (index > 0)
    {
     /*Decrement one point */
      index--;
    }
    /* Clear the last number on the array */
    pass[index] = NULL;
    my_Graphic_LCD.print("_", (index*30)+180, 135); /* Set the num on the display */
  }
}

/*****************************************************************
 * Function void set_num
 * Description: Function to set the number received via touch or
 * serial in a sevent segement format.
 * Parameters: int num, number received.
 * Return: None
 *****************************************************************/
void set_num(int num)
{
  /*Set number received to seven segment display*/
  set_sev_seg_num(num);
  /* Clear if the messages is active */
  my_Graphic_LCD.setFont(SmallFont);
  my_Graphic_LCD.print("               ", 185, 200);
}

/*****************************************************************
 * Function set_clear
 * Description: Function to clear all number in the buffer.
 * Parameters: None
 * Return: None
 *****************************************************************/
void set_clear()
{
  int i;

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

/*****************************************************************
 * Function set_enter
 * Description: Function to enter the numbers to verify if the pass
 * is correct.
 * Parameters: None
 * Return: None
 *****************************************************************/
void set_enter()
{
  int i;
  bool pass_valid = true;

  my_Graphic_LCD.setColor(255, 255, 255); /*Set White Color to FONT*/
  my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
  my_Graphic_LCD.setFont(SmallFont);
  my_Graphic_LCD.print("Enter Pressed", 185, 200);

  for ( i = 0; i < 4; i++)
  {
    if (pass[i] == NULL)
    {
      my_Graphic_LCD.print("Enter 4 numbers", 185, 200);
      break;
    }
    else
    {
      if (pass[i] != storedpass[i])
      {
        /* Stop pass are incorrect */
        my_Graphic_LCD.print("Incorrect Pass", 185, 200);
        pass_valid = false;
        break;
      }

      Serial.print(pass[i]);
    }
  }
  
  if (pass_valid)
  {
    /* enter in passed app */
    pass_accepted_message();
  }
}

/*****************************************************************
 * Function set_back
 * Description: Function to clear the last number introduced.
 * Parameters: None
 * Return: None
 *****************************************************************/
void set_back()
{
  /*Clear the last number entered*/
  clear_last_num();
  my_Graphic_LCD.setFont(SmallFont);
  my_Graphic_LCD.print("               ", 185, 200);
}

/*****************************************************************
 * Function setup
 * Description: Function to start the setup initialization.
 * Parameters: None
 * Return: None
 *****************************************************************/
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

/*****************************************************************
 * Function loop
 * Description: main function to read periodically the serial input
 * or touch buttons.
 * Parameters: None
 * Return: None
 *****************************************************************/
void loop()
{
  int pressed_button;
  char inChar;

  while(true)
  {
    if (Serial.available() > 0)
    {
      inChar = (char)Serial.read();
      Serial.print(inChar);
      
      switch(inChar)
      {
        case'1':
          set_num(1);
          break;
        case '2':
          set_num(2);
          break;
        case '3':
          set_num(3);
          break;
        case '4':
          set_num(4);
          break;
        case '5':
          set_num(5);
          break;
        case '6':
          set_num(6);
          break;
        case '7':
          set_num(7);
          break;
        case '8':
          set_num(8);
          break;
        case '9':
          set_num(9);
          break;
        case '0':
          set_num(0);
          break;
        case 'c':
          set_clear();
          break;
        case 'e':
          set_enter();
          break;
        case 'r':
          set_back();
          break;
        default:
          Serial.print("Invalid character");
          break;
      }
    }

    if (my_Touch_LCD.dataAvailable() == true)
    {
      pressed_button = myButtons.checkButtons();

      if (pressed_button == enter_button)
      {
        set_enter();
      }
      else if (pressed_button == clear_button)
      {
        set_clear();
      }
      else if (pressed_button == button_num_1)
      {
        set_num(1);
      }
      else if (pressed_button == button_num_2)
      {
        set_num(2);
      }
      else if (pressed_button == button_num_3)
      {
       set_num(3);
      }
      else if (pressed_button == button_num_4)
      {
        set_num(4);
      }
      else if (pressed_button == button_num_5)
      {
        set_num(5);
      }
      else if (pressed_button == button_num_6)
      {
       set_num(6);
      }
      else if (pressed_button == button_num_7)
      {
        set_num(7);
      }
      else if (pressed_button == button_num_8)
      {
        set_num(8);
      }
      else if (pressed_button == button_num_9)
      {
        set_num(9);
      }
      else if (pressed_button == button_num_0)
      {
        set_num(0);
      }
      else if (pressed_button == button_retro)
      {
       set_back();
      }
      else if (pressed_button == button_empty)
      {
        /* Do nothing */
      }
      else
      {
        my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("               ", 185, 200);
      }
    }
  }
}
