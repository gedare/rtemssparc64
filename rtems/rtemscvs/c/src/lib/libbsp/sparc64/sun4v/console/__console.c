#include <bsp.h>

#include <libchip/serial.h>

console_tbl     Console_Port_Tbl[] = {
{
  "/dev/console",                          /* sDeviceName */
   0,                         			   /* deviceType */
   pooled_functions,                       /* pDeviceFns */
   NULL,                                   /* deviceProbe, assume it is there */
   NULL,                                   /* pDeviceFlow */
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
}
};



/*
 *  Declare some information used by the console driver
 */

#define NUM_CONSOLE_PORTS (sizeof(Console_Port_Tbl)/sizeof(console_tbl))

unsigned long  Console_Port_Count = NUM_CONSOLE_PORTS;

console_data  Console_Port_Data[NUM_CONSOLE_PORTS];

rtems_device_minor_number  Console_Port_Minor;


/*
 *  Polled mode functions
 */
console_fns pooled_functions={
 NULL, /*bool    (*deviceProbe)(int minor);*/
 NULL, /*int     (*deviceFirstOpen)(int major, int minor, void *arg);*/
 NULL, /*int     (*deviceLastClose)(int major, int minor, void *arg);*/
 NULL, /*int     (*deviceRead)(int minor);*/
 sun4v_console_poll_write, /*int     (*deviceWrite)(int minor, const char *buf, int len);*/
 sun4v_console_deviceInitialize, /*void    (*deviceInitialize)(int minor);*/
 NULL, /*void    (*deviceWritePolled)(int minor, char cChar);*/
 NULL, /*int     (*deviceSetAttributes)(int minor, const struct termios *t);*/
 NULL /*bool    deviceOutputUsesInterrupts;*/
}

static int sun4v_console_poll_write(int minor, const char *buf, int n)
{
    int i = 0;
    /* Write */
    for (i = 0; i < n; ++i) {
        //my_driver_write_char(e, buf [i]);
		ofw_write(buf [i], 1);
    }

    return 0;
}

void sun4v_console_deviceInitialize (int minor)
{
	printk("\n\rmy_driver_deviceInitialize()\n\r");
}



/* putchar/getchar for printk */

static void bsp_out_char (char c)
{
  ofw_write(&c, 1);
}

BSP_output_char_function_type BSP_output_char = bsp_out_char;

