#ifndef H_EMBCLI
#define H_EMBCLI

// copies error to out buffer, panicking if not enough space
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


typedef struct embcli_state
{
    float *analog_pins;
    size_t number_of_analog_pins;
} embcli_state;

typedef enum embcli_error
{
    EMBCLI_NO_ERROR,
    EMBCLI_ERROR_UNRECOGNIZED_COMMAND, // did not recognize command in input
    EMBCLI_ERROR_NO_PROMPT_SPACE,      // not enough space in output argument
    EMBCLI_ERROR_NO_COMMAND,           // no command given
    EMBCLI_ERROR_TOO_MANY_ARGUMENTS,   // some commands don't take arguments
    EMBCLI_ERROR_EXPECTED_ARGUMENT,    // no arguments given
    EMBCLI_ERROR_PIN_ID_OUT_OF_RANGE   // pin ID given out of range
} embcli_error;

extern embcli_error (*embcli_commands[])(embcli_state *state, char *token, size_t text_len, char *out_text, size_t out_text_len);
extern const char * embcli_command_names[];
extern int embcli_command_names_len;

/* interprets the input and attempts to write to out_text, returning various errors
 * read_analog - writes output of all analog pins to output buffer
*/
embcli_error embcli_interpret(embcli_state *state, char *in_text, size_t in_text_len, char *out_text, size_t out_text_len);

#endif // H_EMBCLI