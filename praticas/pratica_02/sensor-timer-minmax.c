#include "contiki.h"
#include "sys/etimer.h"
#include "button-sensor.h"
#include "batmon-sensor.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
#define BUF_SIZE 8
static int buffer[BUF_SIZE];
static int buf_c = 0;
static int min = 100;
static int max = 0;

static struct etimer et_sensor;
static struct etimer et_uart;
static struct etimer et_minmax;

/*---------------------------------------------------------------------------*/
PROCESS(sensor_process, "Sensor process");
PROCESS(uart_process, "Serial process");
PROCESS(minmax_process, "Serial process");

/*---------------------------------------------------------------------------*/
AUTOSTART_PROCESSES(&sensor_process,&uart_process,&minmax_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(sensor_process, ev, data)
{
  PROCESS_BEGIN();

  SENSORS_ACTIVATE(batmon_sensor);

  etimer_set(&et_sensor, 1*CLOCK_SECOND); // a cada segundo

  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER)  // se passou um segundo
    {
        etimer_reset(&et_sensor); // reinicia timer

        int val = batmon_sensor.value(BATMON_SENSOR_TYPE_TEMP); // lê sensor

        /* Insira seu código aqui */

        buffer[buf_c++] = val;
        if(buf_c > 7) buf_c = 0;

        printf("Leu %d\n", val);
    }
  }

  PROCESS_END();
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(uart_process, ev, data)
{
  PROCESS_BEGIN();

  /* Insira seu código aqui */

  etimer_set(&et_uart, 10*CLOCK_SECOND); // a cada segundo

  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER)  // se passaram 10 segundos
    {
        /* Insira seu código aqui */

        etimer_reset(&et_uart); // reinicia timer

        int avg = 0;
        for(int i=0;i<BUF_SIZE;i++) {
            avg += buffer[i];
            if(min > buffer[i]) min = buffer[i];
            if(max < buffer[i]) max = buffer[i];
        }
        avg = avg/BUF_SIZE;
        printf("Temperatura media: %d\n", avg);
    }
  }

  PROCESS_END();
  }
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(minmax_process, ev, data)
{
  PROCESS_BEGIN();

  /* Insira seu código aqui */

  etimer_set(&et_minmax, 20*CLOCK_SECOND); // a cada segundo

  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER)  // se passaram 20 segundos
    {
        /* Insira seu código aqui */

        etimer_reset(&et_minmax); // reinicia timer

        printf("Temperatura maxima: %d\n", max);
        printf("Temperatura minima: %d\n", min);

    }
  }

  PROCESS_END();
  }
