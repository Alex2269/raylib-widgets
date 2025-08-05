#ifndef __OSC_H
#define __OSC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "raylib.h"
#include "stdint.h"
#include "math.h"
#include "stdlib.h"


#define WIDTH 800
#define HEIGHT 480
#define CELL_SIZE 40
#define BEGIN_RAY (CELL_SIZE * 1)
#define WIDTH_RAY (WIDTH - (CELL_SIZE - 1))
#define HALF_RAY ((WIDTH_RAY - BEGIN_RAY) / 2)
#define MIN_Y_BORDER CELL_SIZE
#define MAX_Y_BORDER (HEIGHT - CELL_SIZE)

// Константи для кращої читабельності
#define ADC_RESOLUTION 4096
#define BUFFER_NOISE_REDUCTION 1.0f
#define TEST_SIGNAL_MULTIPLIER 0.001f
#define TEST_SIGNAL_OFFSET 0.0f
#define TRIGGER_MIN 50
#define TRIGGER_MAX (ADC_RESOLUTION - 50)
#define MAX_X_BUFFER WIDTH_RAY
#define MIN_X_BUFFER 0
#define MAX_Y_BUFFER (HEIGHT - CELL_SIZE)
#define MIN_Y_BUFFER CELL_SIZE
// #define Y_WORKSPACE (MAX_Y_BUFFER - MIN_Y_BUFFER)
#define Y_WORKSPACE (MAX_Y_BORDER - MIN_Y_BORDER)
#define X_WORKSPACE (WIDTH - (BEGIN_RAY + CELL_SIZE))

#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))


#ifdef __cplusplus
}
#endif

#endif /* __OSC_H */
