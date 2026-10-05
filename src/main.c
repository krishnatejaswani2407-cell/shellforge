#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/history.h>
#include <readline/readline.h>

#include "history.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "expand.h"
#include "builtin.h"
#include "executor.h"

int main(void)
{
    /* =============================================
       WELCOME BANNER
       ============================================= */

    printf("=====================================\n");
    printf("            Shellforge\n");
    printf("    A Unix Style Shell written in C\n");
    printf("=====================================\n");

    /* =============================================
       INSTALL BACKGROUND PROCESS HANDLER
       ============================================= */

    setup_background_handler();

    /* Enable readline history */
    using_history();

    token_list_t tokens;
    pipeline_t pipeline;

    char *line;

    while (1)
    {
        /* =============================================
           READ COMMAND
           ============================================= */

        line = readline("shellforge$ ");

        if (line == NULL)
        {
            printf("\nGoodbye!\n");
            break;
        }

        /* Ignore empty input */
        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        /* =============================================
           HISTORY COMMAND
           ============================================= */

        if (strcmp(line, "history") == 0)
        {
            print_history();
            free(line);
            continue;
        }

        /* Add command to history */
        add_history(line);

        /* =============================================
           MILESTONE 2.1 - LEXER / TOKENIZATION
           ============================================= */

        lexer(line, &tokens);

        /* Uncomment for debugging */
        /* token_print(&tokens); */

        /* =============================================
           MILESTONE 2.2 - PARSER + EXPANSION
           ============================================= */

        /*
         * IMPORTANT:
         * Your parser function appears to be called parse(),
         * not parser().
         */

        if (parse(&tokens, &pipeline))
        {
            expand_variables(&pipeline);

            /* Uncomment for debugging */
            /* pipeline_print(&pipeline); */
        }

        /* =============================================
           EXIT COMMAND
           ============================================= */

        if (pipeline.command_count == 1 &&
            pipeline.commands[0].argc > 0 &&
            strcmp(pipeline.commands[0].argv[0], "exit") == 0)
        {
            free(line);
            break;
        }

        /* =============================================
           EXECUTE COMMAND
           ============================================= */

        execute_pipeline(&pipeline);

        free(line);
    }

    return 0;
}
