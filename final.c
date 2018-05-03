// John Harvey
// MSP430 Final Project
// The Azerbaijan Project


#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include <libemb/serial/serial.h>
#include <libemb/conio/conio.h>
#include <libemb/shell/shell.h>


//***************************PROTOTYPES************************************//
int shell_cmd_help(shell_cmd_args *args);
int shell_cmd_argt(shell_cmd_args *args);
int shell_cmd_setc(shell_cmd_args *args);
int shell_cmd_howitworks(shell_cmd_args *args);


//***************************CONSTANTS*************************************//
const char digit[] = { BIT5, BIT6, BIT7 };        // Digit Select


//********************CHAR ARRAY FOR NUMERIC DISPLAY***********************//
const char num[] = { /*                           //
    0 */ BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5, /* Numeric Display
    1 */ BIT1 | BIT2, /*                          //
    2 */ BIT0 | BIT1 | BIT3 | BIT4 | BIT6, /*
    3 */ BIT0 | BIT1 | BIT2 | BIT3 | BIT6, /*
    4 */ BIT1 | BIT2 | BIT5 | BIT6, /*
    5 */ BIT0 | BIT2 | BIT3 | BIT5 | BIT6, /*
    6 */ BIT0 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6, /*
    7 */ BIT0 | BIT1 | BIT2, /*
    8 */ BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6, /*
    9 */ BIT0 | BIT1 | BIT2 | BIT5 | BIT6, /*
    r */ BIT4 | BIT6, /*
    g */ BIT0 | BIT1 | BIT2 | BIT3 | BIT5 | BIT6, /*
    b */ BIT2 | BIT3 | BIT4 | BIT5 | BIT6
  };


//****************************************GLOBALS******************************************************//
unsigned char values[] = { 0, 0, 0};  //this will hold the values (0-99) inputed by the user for 'r', 'g', and 'b'
unsigned int  PWM      = 0;           //PWM will be used in TIMER0_A0 for lighting the LED
unsigned int  digitc   = 0;           //digitc will be used as a counter in TIMER1_A0
unsigned int  digitc2  = 0;           //digitc2 will also be used as a counter in TIMER1_A0
char cyc[3];


//***************************************SHELL COMMANDS STRUCT*****************************************//
shell_cmds my_shell_cmds = {
  .count = 4,
  .cmds  = {
    {
      .cmd  = "help",                                         //set command to 'help'
      .desc = "prints out a list of acceptable commands",     //description of command
      .func = shell_cmd_help,                                 //set function to shell_cmd_help
    },
    {
      //this will display/print out the arguments given by the user
      .cmd  = "args",                                         //set command to 'args'
      .desc = "print back given arguments",                   //desciption of command
      .func = shell_cmd_argt,                                 //set function to shell_cmd_argt
    },
    {
      //this will set the color of the LED based of user input
      .cmd  = "setc",                                         //set command to 'setc'
      .desc = "set <color> <value> of LED",                   //description of command
      .func = shell_cmd_setc,                                 //set function to shell_cmd_setc
    },
    {
      //this will print out a short description of how the program works
      .cmd  = "howitworks",                                   //set command to 'howitworks'
      .desc = "prints out a short description of the program",//description of command
      .func = shell_cmd_howitworks,                           //set function to shell_cmd_howitworks
    },
  },
};


//****************************************CALLBACK HANDLERS************************************************//
int shell_process(char *cmd_line)
{
  return shell_process_cmds(&my_shell_cmds, cmd_line);
}

int shell_cmd_help(shell_cmd_args *args)          //'help' will display all available/acceptable commands      
{
  int k;      

  for(k = 0; k < my_shell_cmds.count; k++) {      //for loop to loop through commands and display them
    cio_printf("%s: %s\n\r", my_shell_cmds.cmds[k].cmd, my_shell_cmds.cmds[k].desc);
  }
  return 0;
}

int shell_cmd_argt(shell_cmd_args *args)          //'args' will display the arguments given
{
  int k;

  cio_print((char *)"args given:\n\r");

  for(k = 0; k < args->count; k++) {              //for loop to loop through args given and display them
    cio_printf(" - %s\n\r", args->args[k].val);
  }
  return 0;
}

int shell_cmd_setc(shell_cmd_args *args)          //'setc' will accept an 'r', 'g', or 'b' followed by a value between 0-99
{                                                 //this will set the colors on the LED light

  int inputValue = shell_parse_int(args->args[1].val);      // parses and stores argument[1] as inputValue. this is the rgb number between 0-99

  if (inputValue < 0 || inputValue > 99)                    // test to make sure the value is between 0 and 99
    cio_print((char *) "ERROR, color value must be between 0-99\n\r");

  char inputColor = *(args->args[0].val);                   // create char inputColor and point it at argument[0] given by user. 
  switch (inputColor)                                       // switch statement to test and make sure either an 'r', 'g', or 'b' was entered
   {
    case 'r':                                               // if char inputColor is 'r', store the inputValue in values[0]
      values[0] = inputValue;                               
      cio_printf("RED has been set to %i\n\r", inputValue);
      break;
    case 'g':                                               // if char inputColor is 'g', store the inputValue in values[1]
      values[1] = inputValue;
      cio_printf("GREEN has been set to %i\n\r", inputValue);
      break;
    case 'b':                                               // if char inputColor is 'b', store the inputValue in values[2]                                 
      values[2] = inputValue;
      cio_printf("BLUE has been set to %i\n\r", inputValue);
      break;
    default:
      cio_print((char *) "ERROR, color must be r, g, or b\n\r");  // ERROR if character input was not r, g, or b
      break;
    }
  return 0;
}

int shell_cmd_howitworks(shell_cmd_args *args)          //'howitworks' will display a short instructional paragraph as to how the program works
{
  cio_print((char *) "The goal is to light up the LED light with various percentages of red, blue, and green light.\r\nThe user will type 'setc' followed by an 'r', 'g', or 'b', followed by a digit between 0-99.\r\n");
  cio_print((char *) "The bubble display will show the values given, and the LED light will light up.\r\nThe color of the LED will be determined by the values entered for 'r', 'g', and 'b'.\r\nExample input: \r\n");
  cio_print((char *) "$ setc r 88\r\n$ setc g 25\r\n$ setc b 72");
  return 0;
}


//***************************************MAIN***************************************************//
int main(void)
{
  WDTCTL   = WDTPW | WDTHOLD;               // Disable Watchdog
  BCSCTL1  = CALBC1_1MHZ;                   // Run @ 1MHz
  DCOCTL   = CALDCO_1MHZ;
  P2SEL   &= ~(BIT6 | BIT7);                // set P2.6 and P2.7 as Outputs

  serial_init(9600);                        // Initialize Serial Comms
  _BIS_SR(GIE);                             // sets the GIE (Global Interrupt Enable) bit in the status register

        P1DIR |= 0b11111001;                // set P1DIR bits
        P2DIR |= 0b01111111;                // set P2DIR bits

        TA0CCTL0= CCIE;                     // enable interrupt for LED 
        TA0CCR0= 100;                       // set timer to count up to 100
        TA0CTL= TASSEL_2 | MC_1 | ID_0;     // configure and start timer. TASSEL2 will use INCLK, MC_1 will set mode control to UP mode
                                            // ID_0 will select the divider for input clock. 00 = /1

        TA1CCTL0= CCIE;                     // enable interrupt for bubble display
        TA1CCR0= 600;                       // set timer to count up to 600
        TA1CTL= TASSEL_2 | MC_1 | ID_3;     // configure and start timer. TASSEL2 will use INCLK, MC_1 will set mode control to UP mode
                                            // ID_3 will select the divider for input clock. 10 = /4

  mainMenu();                               //run the mainMenu

  return 0;
}


//***********************************************MENU***********************************************//
int mainMenu(void)                          //menu to handle screen/shell commands
{
  for (;;)                                  // endless menu loop
  {
    int index = 0;                          // char array counter/index for user input
    char cmd_line[90] = {0};                // empty array to hold user input
    cio_print((char *) "-------------------------MENU-----------------------\n\r");
    cio_print((char *) "Type help for a list of commands\n\r");
    cio_print((char *) "$ ");               // prompt user for input

    char input = cio_getc();                // get input from user

    while(input != '\r')                    // listen/recieve input until ENTER is pressed
    {
      if(input == 0x7f)
        {                                   //  test to see if DELETE key was enterted
          if(index != 0)
            {                               //  test to see if cursor is at the start
              cmd_line[--index] = 0;        //  delete key logic
              cio_printc(0x08); cio_printc(' '); cio_printc(0x08);
            }
        } 
      else                                  // else, store user input into cmd_line array
        {
          cmd_line[index++] = input;        
          cio_printc(input);                // print out what was typed in 
        }

    input = cio_getc();                   // listen for another command 
    } 

    cio_print((char *) "\n\r");           // new line   

    switch(shell_process(cmd_line)){         // switch statement to handle shell commands
      case SHELL_PROCESS_ERR_CMD_UNKN:
        cio_print((char *) "ERROR - command unknown\n\r");    // error message for unrecognized command
      break;
      case SHELL_PROCESS_ERR_ARGS_LEN:                    
        cio_print((char *) "ERROR - command too long\n\r");   // error message for too long of a command
      break;
      case SHELL_PROCESS_ERR_ARGS_MAX:
        cio_print((char *) "ERROR - too many arguments\n\r"); // error message for too many arguements
      break;
      default:
    break;
    }

    cio_print((char *) "\n");               // new line     
  }
}

//*********************************************INTERRUPTS*************************************************//

//*********************************************LED TIMER**************************************************//
#pragma vector=TIMER0_A0_VECTOR                         //TIMER0_A0 will light the LED based on PWM
__interrupt void Timer0_A0_ISR (void)                   //PWM will continuously count from 0-100
{
            if (PWM == 1) {                             //if PWM = 1, inclusive OR P1OUT with BIT0, BIT3, and BIT4
                        P1OUT |= BIT0 | BIT3 | BIT4;    //this sets the P1OUT bits needed to control the RED, GREEN, 
            }                                           //and BLUE pins of the LED to HIGH

            if (PWM == values[0]) {                     //if PWM = the value inputed for RED, xor P1OUT with BIT0     
                        P1OUT ^= BIT0;                  //BIT0 controls the RED light, so this if statement will set
            }                                           //the RED based on the user inputed value for setc r

            if (PWM == values[1]) {                     //if PWM = the value inputed for GREEN, xor P1OUT with BIT3   
                        P1OUT ^= BIT3;                  //BIT3 controls the GREEN light, so this if statement will set
            }                                           //the GREEN based on the user inputed value for setc g

            if (PWM == values[2]) {                     //if PWM = the value inputed for BLUE, xor P1OUT with BIT4   
                        P1OUT ^= BIT4;                  //BIT4 controls the BLUE light, so this if statement will set
            }                                           //the BLUE based on the user inputed value for setc b

            if (PWM == 100) {                           //once PWM hits 100, reset it back to 0
                        PWM = 0;
            }
            PWM++;                                      //increment PWM by 1 each time
}


//*********************************************BUBBLE DISPLAY TIMER**************************************************//
#pragma vector=TIMER1_A0_VECTOR                                  //TIMER1_A0 will set the bubble display
__interrupt void Timer1_A0_ISR (void)
{
    P2OUT &= ~( BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6); //set all P2OUT segments to low
    P1OUT |= BIT5 | BIT6 | BIT7;                                 //set P1OUT digits to high. these pins activate the three cathodes
                                                                 //being used in the bubble display. 1.5 = C2, 1.6 = C3, 1.7 = C4
                                                   
    P1OUT ^= digit[digitc];               //toggle between digits/cathodes
    P2OUT ^= cyc[digitc];                 //toggle between values inputed by user

    digitc++;                             //increment the first digit counter which will count from 0-3
    digitc2++;                            //increment the second digit counter which will count from 0-900
    
    if (digitc==3){                       //if digit1 hits 3, set back to 0
        digitc= 0;                                               

      //set and display the 'r' value
      if (digitc2 == 300){                //when digit2 counter hits 300, 
      cyc[0]=num[10];                     //set cyc[0] to 'r' aka num[10], which will be displayed on cathode 2
      cyc[1]=num[values[0]/10];           //set cyc[1] to the first digit entered by the user, which will be displayed on cathode 3
      cyc[2]=num[values[0] % 10];         //set cyc[2] to the second digit entered by the user, which will be displayed on cathode 4                               
      }
      
      //set and display the 'g' value
      if (digitc2 == 600){                //when digit2 counter hits 600, 
      cyc[0]=num[11];                     //set cyc[0] to 'g' aka num[11], which will be displayed on cathode 2
      cyc[1]=num[values[1]/10];           //set cyc[1] to the first digit entered by the user, which will be displayed on cathode 3
      cyc[2]=num[values[1] % 10];         //set cyc[2] to the second digit entered by the user, which will be displayed on cathode 4 
      }
      
      //set and display the 'b' value  
      if (digitc2 == 900){                //when digit2 counter hits 900, 
      cyc[0]=num[12];                     //set cyc[0] to 'b' aka num[12], which will be displayed on cathode 2
      cyc[1]=num[values[2]/10];           //set cyc[1] to the first digit entered by the user, which will be displayed on cathode 3
      cyc[2]=num[values[2] % 10];         //set cyc[2] to the second digit entered by the user, which will be displayed on cathode 4 

          digitc2= 0;                     //reset the digit2 counter back to 0
        }     
      }
}
