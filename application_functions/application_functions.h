#ifndef F_CPU
#define F_CPU  16000000
#endif
#include <util/delay.h>

#ifndef STDIO_H
#include <stdio.h>
#endif

#include <stdlib.h>
#include <avr\io.h>
#include <stdbool.h>
#ifndef APPLICATION_FUNCTIONS_H_
#define APPLICATION_FUNCTIONS_H_

#include "../devices/device.h"
#include "ShowTime.h"
#include "ShowDate.h"
#include "StartTimer.h"
#include "ReadKeyPadWithLCD.h"
#include "FloatToString.h"
#include "LCDWrite.h"
#include "ChangeTime.h"
#include "ChangeDate.h"

#endif /* APPLICATION_FUNCTIONS_H_ */