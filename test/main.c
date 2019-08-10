#include <stdio.h>  // printing
#include <assert.h> // testing
#include <string.h> // putting strings into in buffer

#include "embcli.h"

#define BUFFER_SIZE 512

#define TEST_CMPR(a, afmt, b, bfmt)                                                                                        \
    if (a != b)                                                                                                            \
    {                                                                                                                      \
        fprintf(stderr, "%s:%d: %s: Values " afmt " and " bfmt " are not the same\n", __FILE__, __LINE__, __func__, a, b); \
        fprintf(stderr, "Out buffer: %s", out_buffer);                                                                     \
        return -1;                                                                                                         \
    }

#define STR_CMPR(a, b)                                                                                          \
    if (strcmp(a, b) != 0)                                                                                      \
    {                                                                                                           \
        fprintf(stderr, "%s:%d: %s: Strings %s and %s are not the same\n", __FILE__, __LINE__, __func__, a, b); \
        fprintf(stderr, "Out buffer: %s", out_buffer);                                                          \
        return -1;                                                                                              \
    }

int main(int argc, char **argv)
{
    char in_buffer[BUFFER_SIZE] = {0};
    char out_buffer[BUFFER_SIZE] = {0};

    const size_t number_of_pins = 4;
    float analog_pins[number_of_pins];
    for (size_t i = 0; i < number_of_pins; i++)
    {
        analog_pins[i] = 0.0;
    }
    embcli_state state = {0};
    state.number_of_analog_pins = number_of_pins;
    state.analog_pins = analog_pins;

    strcpy(in_buffer, "read_analog");
    embcli_error error = embcli_interpret(&state, in_buffer, BUFFER_SIZE, out_buffer, BUFFER_SIZE);
    TEST_CMPR(error, "%d", EMBCLI_NO_ERROR, "%d");
    STR_CMPR(out_buffer, "0.00 0.00 0.00 0.00 \n");

    strcpy(in_buffer, "write_analog 0:1.0 1:2.0");
    error = embcli_interpret(&state, in_buffer, BUFFER_SIZE, out_buffer, BUFFER_SIZE);
    TEST_CMPR(error, "%d", EMBCLI_NO_ERROR, "%d");
    strcpy(in_buffer, "read_analog");
    error = embcli_interpret(&state, in_buffer, BUFFER_SIZE, out_buffer, BUFFER_SIZE);
    TEST_CMPR(error, "%d", EMBCLI_NO_ERROR, "%d");
    STR_CMPR(out_buffer, "1.00 2.00 0.00 0.00 \n");

    return 0;
}