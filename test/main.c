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

embcli_error read_analog(embcli_state *state, char *token, size_t text_len, char *out_text, size_t out_text_len)
{
    token = strtok(NULL, " ");
    if (token != NULL)
    {
        return EMBCLI_ERROR_TOO_MANY_ARGUMENTS;
    }

    for (size_t i = 0; i < state->number_of_analog_pins; i++)
    {
        int chars_written = snprintf(out_text, out_text_len, "%0.2f ", state->analog_pins[i]);
        if (chars_written < 0)
        {
            return EMBCLI_ERROR_NO_PROMPT_SPACE;
        }
        out_text += chars_written;
    }
    *out_text = '\n';
    return EMBCLI_NO_ERROR;
}

embcli_error write_analog(embcli_state *state, char *token, size_t text_len, char *out_text, size_t out_text_len)
{
    token = strtok(NULL, " ");
    if (token == NULL)
    {
        SAFE_SPRINTF(EMBCLI_ERROR_EXPECTED_ARGUMENT, out_text, out_text_len, "%s\n", "Expected at least one index:volt_value after write_analog command");
    }
    while (token != NULL)
    {
        int pin_id = 0;
        float target_value = 0.0;
        sscanf(token, "%d:%f", &pin_id, &target_value);
        if (pin_id >= 0 && (pin_id < state->number_of_analog_pins))
        {
            state->analog_pins[pin_id] = target_value;
        }
        else
        {
            SAFE_SPRINTF(EMBCLI_ERROR_PIN_ID_OUT_OF_RANGE, out_text, out_text_len, "Pin ID %d is out of range (pin number is %lu)\n", pin_id, state->number_of_analog_pins);
        }

        token = strtok(NULL, " ");
    }
    SAFE_SPRINTF(EMBCLI_NO_ERROR, out_text, out_text_len, "%s\n", "OK");
}

embcli_error (*embcli_commands[])(embcli_state *state, char *token, size_t text_len, char *out_text, size_t out_text_len) = {
    read_analog,
    write_analog
};

const char *embcli_command_names[] = {
    "read_analog",
    "write_analog"
};
int embcli_command_names_len = 2;

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