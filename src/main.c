#include <stdio.h>

#include "rps.h"
#include "terminal.h"
#include "keys.h"
#include "crayon.h"

#define loop for(;;)

int8_t choose_rock()
{
    int8_t idx = 0;
    loop
    {
        printf("Choose the rock style: ");
        for (int8_t i = 0; i < rocks_count; i++)
        {
            if (i == idx)
            {
                printf(CRAYON_TO_UNDERLINE("%s"), rocks[i]);
            }
            else
            {
                printf("%s", rocks[i]);
            }
        }
        fflush(stdout);
        copied key_t key = keyboard_key_event();
        if (key == key_left && idx > 0)
        {
            printf("\r");
            idx = (idx - 1) % rocks_count;
        }
        else if (key == key_right && idx < rocks_count - 1)
        {
            printf("\r");
            idx = (idx + 1) % rocks_count;
        }
        else if (key == key_enter)
        {
            printf("\r\n");
            return idx;
        }
        else
        {
            printf("\r");
        }
    }
}

int8_t choose_paper()
{
    int8_t idx = 0;
    loop
    {
        printf("Choose the paper style: ");
        for (int8_t i = 0; i < papers_count; i++)
        {
            if (i == idx)
            {
                printf(CRAYON_TO_UNDERLINE("%s"), papers[i]);
            }
            else
            {
                printf("%s", papers[i]);
            }
        }
        fflush(stdout);
        copied key_t key = keyboard_key_event();
        if (key == key_left && idx > 0)
        {
            printf("\r");
            idx = (idx - 1) % papers_count;
        }
        else if (key == key_right && idx < papers_count - 1)
        {
            printf("\r");
            idx = (idx + 1) % papers_count;
        }
        else if (key == key_enter)
        {
            printf("\r\n");
            return idx;
        }
        else
        {
            printf("\r");
        }
    }
}

int8_t choose_scissor()
{
    int8_t idx = 0;
    loop
    {
        printf("Choose the scissor style: ");
        for (int8_t i = 0; i < scissors_count; i++)
        {
            if (i == idx)
            {
                printf(CRAYON_TO_UNDERLINE("%s"), scissors[i]);
            }
            else
            {
                printf("%s", scissors[i]);
            }
        }
        fflush(stdout);
        copied key_t key = keyboard_key_event();
        if (key == key_left && idx > 0)
        {
            printf("\r");
            idx = (idx - 1) % scissors_count;
        }
        else if (key == key_right && idx < scissors_count - 1)
        {
            printf("\r");
            idx = (idx + 1) % scissors_count;
        }
        else if (key == key_enter)
        {
            printf("\r\n");
            return idx;
        }
        else
        {
            printf("\r");
        }
    }
}

int main()
{
    terminal_enter_raw_mode();
    terminal_cursor_hide();

    // printf(CRAYON_TO_BOLD("Ctrl-Q") " to quit\r\n");

    int8_t rock_idx = choose_rock();
    printf("Chosen %s\r\n", rocks[rock_idx]);

    int8_t paper_idx = choose_paper();
    printf("Chosen %s\r\n", papers[paper_idx]);

    int8_t scissor_idx = choose_scissor();
    printf("Chosen %s\r\n", scissors[scissor_idx]);

    key_t key;
    do
    {
        key = keyboard_key_event();
        printf("key = %s\r\n", keyboard_key_event_name_map(key));
    } while (key != (ctrl_mask | 'q'));

    terminal_leave_raw_mode();
    terminal_cursor_show();

    return 0;
}
