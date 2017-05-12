/*****************************************************************
 * UTFT Touch Acces Control
 * This program is a demo of how to use the touch module and screen
 * LCD.
 * Display buttons to enter the password and if this is correct
 * grant access to the application.
 * this program was made with a screen LCD of 320x240 pixels
 * This program requires the UTFT library
 *****************************************************************/
// This code block is only needed to support multiple
// MCU architectures in a single sketch.
#if defined(__AVR__)
  #define imagedatatype  unsigned int
#elif defined(__PIC32MX__)
  #define imagedatatype  unsigned short
#elif defined(__arm__)
  #define imagedatatype  unsigned short
#endif
// End of multi-architecture block
 
#include <UTFT.h>
#include <UTouch.h>
#include <UTFT_Buttons.h>
#include <UTouchCD.h>
/*****************************************************************
 *  Macros
 *****************************************************************/
 #define RS_PIN_LCD  ((int)38) /* Register Pin */
 #define WR_PIN_LCD  ((int)39) /* Write Pin */
 #define CS_PIN_LCD  ((int)40) /* Chip Select */
 #define RST_PIN_LCD ((int)41) /* Reset Pin */

 #define TCLK_PIN_TCH  ((int)6) /* Clock pin */
 #define TCS_PIN_TCH   ((int)5) /* Chip Select */
 #define TDIN_PIN_TCH  ((int)4) /* Data Input */
 #define TDOUT_PIN_TCH ((int)3) /* Data Output */
 #define RST_PIN_TCH   ((int)2) /* Reset Pin */

 #define SEVEN_SEG_NUM_X_POS ((int)56) /* X possition for number label */
 #define SEVEN_SEG_NUM_Y_POS ((int)50) /* Y possition for number label */
 #define LBL_MSG_X_POS       ((int)115) /* X possition for label message */
 #define LBL_MSG_Y_POS       ((int)5) /* Y possition for label message */

/*****************************************************************
 * Declare which bitmaps we will be using
 *****************************************************************/
extern imagedatatype spider_peke[];

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
int button_num_1,button_num_2,button_num_3,button_num_4,button_num_5;
int button_num_6,button_num_7,button_num_8,button_num_9;
int button_num_0,button_retro,button_empty;
int index = 0;
int pass[4] = {~0, ~0, ~0, ~0};
int ssap[4] = {1,2,3,4};
bool fullbuffer;
bool pass_incorrect = false;
bool pass_correct = false;

void init_display_message(void)
{
  my_Graphic_LCD.setColor(0, 255, 0); /* Set color green */
  my_Graphic_LCD.fillRect(0, 0, 239, 20); /* Top frame */
  my_Graphic_LCD.setColor(0, 255, 0); /* Set color green */
  my_Graphic_LCD.fillRect(0, 300, 236, 320); /* low frame */
  my_Graphic_LCD.setColor(0, 255, 0); /*Set GREEN Color to FONT*/
  my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
  my_Graphic_LCD.print("* Iniciando *", CENTER, 50);
  my_Graphic_LCD.setFont(SmallFont);
  my_Graphic_LCD.print("*************************", CENTER, 90);
  my_Graphic_LCD.print("* Introduce el password *", CENTER, 110);
  my_Graphic_LCD.print("* correcto para entrar  *", CENTER, 130);
  my_Graphic_LCD.print("*    a la aplicacion.   *", CENTER, 150);
  my_Graphic_LCD.print("*************************", CENTER, 170);
  delay(1000); /*Wait 1 sec */
}
void draw_buttons_lib()
{
  /* Clear LCD Screen background will be set to black */
  my_Graphic_LCD.clrScr();

  button_num_1 = myButtons.addButton(  30, 105,  60,  50, "1");
  button_num_2 = myButtons.addButton(  90, 105,  60,  50, "2");
  button_num_3 = myButtons.addButton( 150, 105,  60,  50, "3");
  button_num_4 = myButtons.addButton(  30, 155,  60,  50, "4");
  button_num_5 = myButtons.addButton(  90, 155,  60,  50, "5");
  button_num_6 = myButtons.addButton( 150, 155,  60,  50, "6");
  button_num_7 = myButtons.addButton(  30, 205,  60,  50, "7");
  button_num_8 = myButtons.addButton(  90, 205,  60,  50, "8");
  button_num_9 = myButtons.addButton( 150, 205,  60,  50, "9");
  button_retro = myButtons.addButton(  30, 255,  60,  50, "<-");
  button_num_0 = myButtons.addButton(  90, 255,  60,  50, "0");
  button_empty = myButtons.addButton( 150, 255,  60,  50, " ");
  enter_button = myButtons.addButton(   5,   5, 100,  20, "Enter");
  clear_button = myButtons.addButton(   5,  28, 100,  20, "Clear");
  myButtons.setButtonColors(VGA_LIME,VGA_SILVER,VGA_LIME,VGA_GREEN,VGA_BLACK);
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
        my_Graphic_LCD.printNumI(0, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 0;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
      }
      break;
    }
    case 1:
    {
      if (index < 4)
      {
        my_Graphic_LCD.printNumI(1, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 1;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
      }
      break;
    }
    case 2:
    {
      if (index < 4)
      {
        my_Graphic_LCD.printNumI(2, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 2;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
      }
      break;
    }
    case 3:
    {
      if (index < 4)
      {
        my_Graphic_LCD.printNumI(3, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 3;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
      }
      break;
    }
    case 4:
    {
      if (index < 4)
      {
        my_Graphic_LCD.printNumI(4, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 4;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
      }
      break;
    }
    case 5:
    {
      if (index < 4)
      {
        my_Graphic_LCD.printNumI(5, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 5;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
      }
      break;
    }
    case 6:
    {
      if (index < 4)
      {
        my_Graphic_LCD.printNumI(6, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 6;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
      }
      break;
    }
    case 7:
    {
      if (index < 4)
      {
        my_Graphic_LCD.printNumI(7, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 7;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
      }
      break;
    }
    case 8:
    {
      if (index < 4)
      {
        my_Graphic_LCD.printNumI(8, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 9;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
      }
      break;
    }
    case 9:
    {
      if (index < 4)
      {
        my_Graphic_LCD.printNumI(9, (index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS); /* Set the num on the display */
        pass[index] = 9;
        /*increment one point in the array */
        index++;
      }
      if (index >= 3)
      {
        fullbuffer = true;
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
    my_Graphic_LCD.fillRect((index*30)+SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS, (index*30)+(SEVEN_SEG_NUM_X_POS + 32), SEVEN_SEG_NUM_Y_POS + 50); /* Black Frame */
    /* Clear the last number on the array */
    pass[index] = ~0;

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

void clear_label()
{
  my_Graphic_LCD.setColor(0, 0, 0); /* Set Black Color */
  my_Graphic_LCD.fillRect(LBL_MSG_X_POS, LBL_MSG_Y_POS, (LBL_MSG_X_POS + 120), (LBL_MSG_Y_POS * 3)); /* Black Frame */
}

void compare_pass()
{
  int i;
  /*compare the password entered */
  for (i = 0; i < 4; i++)
  {
    if (pass[i] != ssap[i])
    {
      pass_incorrect = true;
      break;
    }
  }
  
  if (pass_incorrect == true)
  {
    /* Incorrect Pass */
    my_Graphic_LCD.print("Incorrect Pass", LBL_MSG_X_POS, LBL_MSG_Y_POS);
    clear_pass_entered();
  }
  else
  {
    /* Pass correct */
    my_Graphic_LCD.print("Pass Correct", LBL_MSG_X_POS, LBL_MSG_Y_POS);
    pass_correct = true;
  }
}

void clear_pass_entered()
{
  int i;
  my_Graphic_LCD.setColor(0, 0, 0); /* Set Black Color */
  my_Graphic_LCD.fillRect(SEVEN_SEG_NUM_X_POS, SEVEN_SEG_NUM_Y_POS, 200, 100); /* Black Frame */
  /*Clear all values of the array */
  for (i = 0; i < 4 ; i++)
  {
    pass[i] = ~0;
  }
  /* Set the index to the initial value */
  index = 0;
  pass_incorrect = false;
}

void setup()
{
  /* Setup Serial port at 9600 bps */
  Serial.begin(9600);
  /* Initialize the LCD Module LANDSCAPE is the default orientation */
  my_Graphic_LCD.InitLCD(0); /* 0 = PORTRAIT */
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
  my_Touch_LCD.InitTouch(0); /* 0 = PORTRAIT */
  /*Call function to create the buttons on the display */
  draw_buttons_lib();
}

void loop()
{
  int pressed_button;
  int i;
  index = 0; /* Initialize Index */
  
  while(true)
  {
    if (my_Touch_LCD.dataAvailable() == true)
    {
      pressed_button = myButtons.checkButtons();

      if (pressed_button == enter_button)
      {
        my_Graphic_LCD.setColor(0, 255, 0); /*Set GREEN Color to FONT*/
        my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
        my_Graphic_LCD.setFont(SmallFont);

        /* Check if the pass is complete */
        for ( i = 0; i < 4; i++)
        {
          if (pass[i] == ~0)
          {
            fullbuffer = false;
            break;
          }
        }
        
        if (fullbuffer == false)
        {
          my_Graphic_LCD.print("Enter 4 numbers", LBL_MSG_X_POS, LBL_MSG_Y_POS);
        }
        else
        {
          my_Graphic_LCD.print("Enter Pressed ", LBL_MSG_X_POS, LBL_MSG_Y_POS);

          /* Send via serial the pass entered */
          Serial.print('P');
          for (i = 0; i < 4; i++)
          {
            Serial.print(pass[i]);
          }
          
          /* Compare the pass entered with the password */
          compare_pass();
        }
      }
      else if (pressed_button == clear_button)
      {
        my_Graphic_LCD.setColor(0, 255, 0); /*Set GREEN Color to FONT*/
        my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
        my_Graphic_LCD.setFont(SmallFont);
        my_Graphic_LCD.print("Clear Pressed  ", LBL_MSG_X_POS, LBL_MSG_Y_POS);

        clear_pass_entered();
      }
      else if (pressed_button == button_num_1)
      {
        /*Set 1 to seven segment display*/
        set_sev_seg_num(1);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_num_2)
      {
        /*Set 2 to seven segment display*/
        set_sev_seg_num(2);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_num_3)
      {
        /*Set 3 to seven segment display*/
        set_sev_seg_num(3);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_num_4)
      {
        /*Set 4 to seven segment display*/
        set_sev_seg_num(4);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_num_5)
      {
        /*Set 5 to seven segment display*/
        set_sev_seg_num(5);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_num_6)
      {
        /*Set 6 to seven segment display*/
        set_sev_seg_num(6);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_num_7)
      {
        /*Set 7 to seven segment display*/
        set_sev_seg_num(7);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_num_8)
      {
        /*Set 8 to seven segment display*/
        set_sev_seg_num(8);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_num_9)
      {
        /*Set 9 to seven segment display*/
        set_sev_seg_num(9);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_num_0)
      {
        /*Set 0 to seven segment display*/
        set_sev_seg_num(0);
        /* Clear if the messages is active */
        clear_label();
      }
      else if (pressed_button == button_retro)
      {
        /*Clear the last number entered*/
        clear_last_num();
        clear_label();
      }
      else if (pressed_button == button_empty)
      {
        /* Do nothing */
      }
      else
      {
        my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */
        clear_label();
      }

      if (pass_correct == true)
      {
        my_Graphic_LCD.clrScr();
        
        /* Disable Buttons */
        myButtons.disableButton(button_num_1);
        myButtons.disableButton(button_num_2);
        myButtons.disableButton(button_num_3);
        myButtons.disableButton(button_num_4);
        myButtons.disableButton(button_num_5);
        myButtons.disableButton(button_num_6);
        myButtons.disableButton(button_num_7);
        myButtons.disableButton(button_num_8);
        myButtons.disableButton(button_num_0);
        myButtons.disableButton(clear_button);
        myButtons.disableButton(enter_button);
        myButtons.disableButton(button_retro);
        myButtons.disableButton(button_empty);
        
        /* Initialize access */
        my_Graphic_LCD.setColor(0, 255, 0); /* Set color green */
        my_Graphic_LCD.fillRect(0, 0, 239, 20); /* Top frame */
        my_Graphic_LCD.setColor(0, 255, 0); /* Set color green */
        my_Graphic_LCD.fillRect(0, 300, 236, 320); /* low frame */
        my_Graphic_LCD.setColor(0, 255, 0); /*Set GREEN Color to FONT*/
        my_Graphic_LCD.setBackColor(0, 0, 0); /*Set black bckgrd */

        for (int i=0; i<360; i+=5)
        {
          my_Graphic_LCD.drawBitmap(70, 115, 90, 62, spider_peke, i,32,32);
        }
      }
    }
  }
}
