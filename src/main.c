#include <stdio.h>
#include <stdlib.h>

#include "rps.h"
#include "terminal.h"
#include "keys.h"
#include "crayon.h"

#define loop for(;;)

int8_t choose_item(borrowed const char * prompt, borrowed const char * const * items, copied const int8_t count)
{
    int8_t idx = 0;
    loop
    {
        printf("%s", prompt);
        for (int8_t i = 0; i < count; i++)
        {
            if (i == idx)
            {
                printf(CRAYON_TO_REVERSED("%s"), items[i]);
            }
            else
            {
                printf("%s", items[i]);
            }
        }
        fflush(stdout);

        copied key_t key = keyboard_key_event();
        if (key == key_left && idx > 0)
        {
            idx--;
        }
        else if (key == key_right && idx < count - 1)
        {
            idx++;
        }
        else if (key == key_enter)
        {
            printf("\r\n");
            return idx;
        }
        else if (key == (ctrl_mask | 'q'))
        {
            printf("\r\n");
            exit(EXIT_SUCCESS);
        }

        printf("\r");
    }
}

int8_t choose_rock()
{
    return choose_item("Choose the rock style: ", rocks, rocks_count);
}

int8_t choose_paper()
{
    return choose_item("Choose the paper style: ", papers, papers_count);
}

int8_t choose_scissor()
{
    return choose_item("Choose the scissor style: ", scissors, scissors_count);
}

void setup()
{
    terminal_enter_raw_mode();
}

void fin()
{
    terminal_leave_raw_mode();
}

int main()
{
    setup();

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

    fin();

    return 0;
}
