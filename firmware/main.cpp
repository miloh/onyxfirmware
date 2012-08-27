/**************************************************
*                                                 *
*            Safecast Geiger Counter              *
*                                                 *
**************************************************/

#include "wirish_boards.h"
#include "power.h"
#include "safecast_config.h"

#include "Buzzer.h"
#include "Accelerometer.h"
#include "UserInput.h"
#include "Geiger.h"
#include "Led.h"
#include "GUI.h"
#include "Controller.h"
#include "RealTime.h"
#include <stdint.h>

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void
premain()
{
  gpio_init_all();
  // manual power up beep
  gpio_set_mode (GPIOB,9, GPIO_OUTPUT_PP);
  for(int n=0;n<100;n++) {
    gpio_toggle_bit(GPIOB,9);
    delay_us(1000);
  }
  gpio_write_bit(GPIOB,9,0);

  init();
  delay_us(100000);
}

int main(void) {

    Buzzer        b;
    Accelerometer a;
    Geiger        g;
    Led           l;
    RealTime      r;

    power_set_debug(0);
    power_init();
    
    power_set_debug(1);

    power_set_state(PWRSTATE_USER);
    display_initialise();
    b.initialise();
    //a.initialise();
    l.initialise();
    r.initialise();
    g.initialise();

    //d.test();
    delay_us(1000000);

    l.set_on();


    //d.powerdown();
    //b.powerdown();
    //u.powerdown();
    //a.powerdown();

    //int array[100];
    //for(uint32_t n=0;n<100;n++) array[n] =5;
    uint16_t h[6];
    for(uint32_t n=0;n<6;n++) h[n]=0;
    h[0]=65535;

    Controller c(g);
    GUI m_gui(c);
    c.set_gui(m_gui);
    UserInput  u(m_gui);
    u.initialise();
    for(;;) {
      c.update();
      m_gui.render();
      power_wfi();
/*
      char *c;
      delay_us(1000);
 //   c = diag_data(8);
 //   display_draw_text(0,0,c,0);

      c = diag_data(3);
      display_draw_text(0,16,c,0);

      c = diag_data(0);
      display_draw_text(0,32,c,0);

      c = diag_data(6);
      display_draw_text(0,48,c,0);

      c = diag_data(4);
      display_draw_text(0,64,c,0);

      c = diag_data(2);
      display_draw_text(0,80,c,0);

      display_draw_number(0,100,n,5,0);
      n++;
*/
    }

    // should never get here
    for(int n=0;n<60;n++) {
      delay_us(100000);
      b.buzz();
    }
    return 0;
}
