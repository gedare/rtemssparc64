#include <bsp.h>

#include <libchip/serial.h>

int sun4v_console_device_first_open(int major, int minor, void *arg)
{
	
}

static int sun4v_console_poll_write(int minor, const char *buf, int n)
{
	ofw_write(buf, n);
}

void sun4v_console_deviceInitialize (int minor)
{
	
}

int sun4v_console_poll_read(int minor){
	printk("\n\sun4v_console_poll_read() NOT YET IMPLEMENTED\n\r");
    return -1;
}


bool sun4v_console_deviceProbe (int minor)
{
	return true;
}



/*
 *  Polled mode functions
 */
console_fns pooled_functions={
 sun4v_console_deviceProbe, /*bool    (*deviceProbe)(int minor);*/
 sun4v_console_device_first_open, /*int     (*deviceFirstOpen)(int major, int minor, void *arg);*/
 NULL, /*int     (*deviceLastClose)(int major, int minor, void *arg);*/
 sun4v_console_poll_read, /*int     (*deviceRead)(int minor);*/
 sun4v_console_poll_write, /*int     (*deviceWrite)(int minor, const char *buf, int len);*/
 sun4v_console_deviceInitialize, /*void    (*deviceInitialize)(int minor);*/
 NULL, /*void    (*deviceWritePolled)(int minor, char cChar);*/
 NULL, /*int     (*deviceSetAttributes)(int minor, const struct termios *t);*/
 NULL /*bool    deviceOutputUsesInterrupts;*/
};

console_flow sun4v_console_console_flow={
  NULL,/*int (*deviceStopRemoteTx)(int minor);*/
  NULL/*int (*deviceStartRemoteTx)(int minor);*/
};


console_tbl     Console_Port_Tbl[] = {
{
  "/dev/console",                          /* sDeviceName */
   0,                         			   /* deviceType */
   &pooled_functions,                      /* pDeviceFns */
   NULL,              						/* deviceProbe, assume it is there */
   &sun4v_console_console_flow,             /* pDeviceFlow */
   0,                                      /* ulMargin */
   0,                                      /* ulHysteresis */
   (void *) NULL,               /* NULL */ /* pDeviceParams */
   0,  									   /* ulCtrlPort1 */
   0,                   				   /* ulCtrlPort2 */
   1,                          	           /* ulDataPort */
   NULL,                                   /* getRegister */
   NULL,                                   /* setRegister */
   NULL, /* unused */                      /* getData */
   NULL, /* unused */                      /* setData */
   0,                             		   /* ulClock */
   NULL      							   /* ulIntVector -- base for port */
},
};



/*
 *  Declare some information used by the console driver
 */

#define NUM_CONSOLE_PORTS 1

unsigned long  Console_Port_Count = NUM_CONSOLE_PORTS;

console_data  Console_Port_Data[NUM_CONSOLE_PORTS];

rtems_device_minor_number  Console_Port_Minor;


/* putchar/getchar for printk */

static void bsp_out_char (char c)
{
  ofw_write(&c, 1);
}

BSP_output_char_function_type BSP_output_char = bsp_out_char;

