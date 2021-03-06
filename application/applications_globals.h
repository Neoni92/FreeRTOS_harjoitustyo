#ifndef APPLICATIONS_GLOBALS_H_
#define APPLICATIONS_GLOBALS_H_

/* message structure */
typedef struct
{
	char idMessage; /* message ID */
	char *data; /* message data */
}DISPLAY_MESSAGE;

/* months */
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

/* tasks global data */
#define INTS_MAX 6
unsigned long secondsFromMidNight = 0;	/* seconds long, because otherwise there's not enough */
unsigned  mainScreenTimer = 5;			/* initialize with > 3 so the main screen shows immediately */
unsigned previousScreen = 0;
#define WAIT_BEFORE_MAIN_DISPLAY 3
bool mainScreenTimerStopped = false;

int ints[INTS_MAX] = {0};

/* (IDM = identification of message) */
#define IDM_UPDATE_DISPLAY 1
#define IDM_DISPLAY_MINMAX 2
#define IDM_DISPLAY_MAIN 5

/* (IDD = identification of data) */
#define IDD_TEMPERATURE 0
#define IDD_MAXTEMPERATURE 1
#define IDD_MINTEMPERATURE 2
float temperatures[INTS_MAX] = {0};

/* introducing the tasks */
static void vLcdHandler( void *pvParameters );
static void vKeyPadHandler( void *pvParameters );
static void vDoMeasurements( void *pvParameters );
static void vClock( void *pvParameters );
static void vTerminal( void *pvParameters );

/* Baud rate used by the serial port tasks */
#define mainCOM_BAUD_RATE			(9600)
#define comBUFFER_LEN				(50)

/* queues */
static QueueHandle_t xDisplay; /* queue for display */

// semaphores
static SemaphoreHandle_t xADC; /* Semaphore for ADC */
static SemaphoreHandle_t xDisplaySemaphore; /* Semaphore for Display */
static SemaphoreHandle_t xClock; /* Semaphore for clock */

xComPortHandle xSerialPort;

void StartTimer( int ticks);

#endif /* APPLICATIONS_GLOBALS_H_ */