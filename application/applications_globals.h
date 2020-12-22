#ifndef APPLICATIONS_GLOBALS_H_
#define APPLICATIONS_GLOBALS_H_

// Scheduler include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "semphr.h"
#include "serial.h"
#include "queue.h"
#include "event_groups.h"

// message structure
typedef struct
{
	char idMessage; // viestin tunniste
	char data;      // dataa
}DISPLAY_MESSAGE;

// months
typedef enum
{
	January = 1,
	February,
	March,
	April,
	May,
	June,
	July,
	August,
	September,
	October,
	November,
	December
} Months;

// tasks global data
#define INTS_MAX 5
#define INTS_MAX_TIME 6
unsigned long secondsFromMidNight = 0;	// seconds long, because otherwise there's not enough
unsigned  mainScreenTimer = 5;			// initialize with >3 so the main screen shows immediately

int ints[INTS_MAX] = {0};
int intsTime[INTS_MAX_TIME] = {0};

// (IDM = identification of message)
#define IDM_UPDATE_DISPLAY 1
#define IDM_DISPLAY_MINMAX 2
#define IDM_DISPLAY_AVERAGE 3
#define IDM_DISPLAY_HISTORY 4
#define IDM_DISPLAY_MAIN 5
#define IDM_DISPLAY_UPDATE_TIME	6
#define IDM_DISPLAY_UPDATE_DATE 7

// (IDD = identification of data)
#define IDD_TEMPERATURE 2
#define IDD_MAXTEMPERATURE 3
#define IDD_MINTEMPERATURE 4

// introducing the tasks
static void vLcdHandler( void *pvParameters );
static void vKeyPadHandler( void *pvParameters );
static void vDoMeasurements( void *pvParameters );
static void vClock( void *pvParameters );
static void vTerminal( void *pvParameters );

/* Baud rate used by the serial port tasks. */
#define mainCOM_BAUD_RATE			(9600)
#define comBUFFER_LEN				(50)

// queues
static QueueHandle_t xDisplay; // queue for display

// semaphores
static SemaphoreHandle_t xADC; // Semaphore for ADC
static SemaphoreHandle_t xDisplaySemaphore; // Semaphore for Display
static SemaphoreHandle_t xClock; // Semaphore for clock

xComPortHandle xSerialPort;

#endif /* APPLICATIONS_GLOBALS_H_ */