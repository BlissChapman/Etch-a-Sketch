
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include "motor.h"

#define DELAY_US 1000

// WiringPi pin numbers
static const unsigned int dir_pins[] = {
    4,
    6
};
static const unsigned int step_pins[] = {
    2,
    12
};
#define NUM_PIN_ASSIGMENTS 2
static gpio_pinout_labels_t pinout_labels = {
    .labels = {
        [16] = "Direction 0", // WiringPi 4
        [22] = "Direction 1", // WiringPi 6
        [13] = "Step 0", // WiringPi 2
        [19] = "Step 1" // WiringPi 12
    }
};

void
motor_initialize(void)
{
    // Set up.
    wiringPiSetup();
}

int
motor_init(motor_t *motor)
{
    if (!motor) {
        return 1;
    }
    
    memset(motor, 0, sizeof(*motor));
    
    static unsigned int next_id = 0;
    unsigned int id = next_id++;
    if (id >= NUM_PIN_ASSIGMENTS) {
        return 1;
    }
    motor->pin_step = step_pins[id];
    motor->pin_dir  = dir_pins[id];
    motor->id = id;
    // Set the pins to output mode.
    pinMode(motor->pin_step, OUTPUT);
    pinMode(motor->pin_dir,  OUTPUT);
    
    return 0;
}

void
motor_move(const motor_t *motor, motor_dir_t dir)
{
    if (!motor) { // Safety first
        return;
    }
    
    // Set the direction pin
    switch (dir) {
    case DIR_CW:
        digitalWrite(motor->pin_dir, HIGH);
        break;
    
    case DIR_CCW:
        digitalWrite(motor->pin_dir, LOW);
        break;
    
    default:
        fprintf(stderr, "Invalid direction: %d\n", dir);
        return;
    }
    
    // Pulse the step pin
    digitalWrite(motor->pin_step, HIGH);
    delayMicroseconds(DELAY_US);
    digitalWrite(motor->pin_step, LOW);
    delayMicroseconds(DELAY_US);
}

static void
print_one_line(const char * const l1,
        const char * const l2,
        const char * const spaces)
{
    // Print the separator
    printf("%s +-+-+ %s\n", spaces, spaces);
    printf("%s | | | %s\n", l1, l2);
}

void
print_gpio_labels(void)
{
    gpio_pinout_labels_t *gpio = &pinout_labels;
    
    // Pad the labels to a uniform length first.
    for (int i = 0; i < numLabels; i++) {
        char * const str = gpio->labels[i];
        int j;
        for (j = strnlen(str, labelWidth); j <= labelWidth; j++) {
            str[j] = ' ';
        }
        str[j] = '\0';
    }
    // Print each label.
    const char * const spaces = gpio->labels[0];
    for (int i = 1; i < numPins; i += 2) {
        print_one_line(gpio->labels[i], gpio->labels[i+1], spaces);
    }
    // Close off the box.
    printf("%s +-+-+ %s\n", spaces, spaces);
}
