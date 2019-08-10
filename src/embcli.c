#include <stdio.h>  // snprintf
#include <string.h> // string ops, like strtok

#include "embcli.h" // basic defs in library, like embcli_error and embcli_state

#define SAFE_SPRINTF(err, s, n, format, ...)                 \
    int chars_written = snprintf(s, n, format, __VA_ARGS__); \
    if (chars_written < 0)                                   \
    {                                                        \
        return EMBCLI_ERROR_NO_PROMPT_SPACE;                 \
    }                                                        \
    if (err >= 0)                                            \
    {                                                        \
        return err;                                          \
    }
embcli_error embcli_interpret(embcli_state *state, char *in_text, size_t in_text_len, char *out_text, size_t out_text_len)
{
    char *token = strtok(in_text, " "); // always the command
    if (token == NULL)
    {
        SAFE_SPRINTF(EMBCLI_ERROR_NO_COMMAND, out_text, out_text_len, "%s\n", "No command received");
    }
    else if (strncmp(token, "read_analog", in_text_len) == 0)
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
    else if (strncmp(token, "write_analog", in_text_len) == 0)
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
    else
    {
        return EMBCLI_ERROR_UNRECOGNIZED_COMMAND;
    }
}
#undef SAFE_SPRINTF