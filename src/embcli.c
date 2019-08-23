#include <stdio.h>  // snprintf
#include <string.h> // string ops, like strtok

#include "embcli.h" // basic defs in library, like embcli_error and embcli_state




embcli_error embcli_interpret(embcli_state *state, char *in_text, size_t in_text_len, char *out_text, size_t out_text_len)
{
    char *token = strtok(in_text, " "); // always the command
    if (token == NULL)
    {
        SAFE_SPRINTF(EMBCLI_ERROR_NO_COMMAND, out_text, out_text_len, "%s\n", "No command received");
    }
    for(int i = 0; i < embcli_command_names_len; i++)
    {
        if (strncmp(token, embcli_command_names[i], in_text_len) == 0)
        {
            return embcli_commands[i](state, token, in_text_len, out_text, out_text_len);
        }
    }
    SAFE_SPRINTF(EMBCLI_ERROR_UNRECOGNIZED_COMMAND, out_text, out_text_len, "Unrecognized command: %s\n", token);
}
#undef SAFE_SPRINTF