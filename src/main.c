#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rps.h"
#include "terminal.h"
#include "keys.h"
#include "crayon.h"

#define loop for(;;)

typedef enum {
    move_rock     = 0,
    move_paper    = 1,
    move_scissors = 2,
} move_t;

typedef enum {
    result_draw = 0,
    result_win  = 1,
    result_lose = 2,
} result_t;

/* Player's chosen styles */
static int8_t rock_style    = 0;
static int8_t paper_style   = 0;
static int8_t scissor_style = 0;

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

borrowed const char * get_move_emoji(copied move_t move)
{
    switch (move)
    {
        case move_rock:     return rocks[rock_style];
        case move_paper:    return papers[paper_style];
        case move_scissors: return scissors[scissor_style];
    }
    return "?";
}

borrowed const char * get_move_name(copied move_t move)
{
    switch (move)
    {
        case move_rock:     return "Rock";
        case move_paper:    return "Paper";
        case move_scissors: return "Scissors";
    }
    return "Unknown";
}

copied result_t judge(copied move_t player, copied move_t computer)
{
    if (player == computer)
    {
        return result_draw;
    }

    /* Rock beats Scissors, Scissors beats Paper, Paper beats Rock */
    if ((player == move_rock     && computer == move_scissors) ||
        (player == move_scissors && computer == move_paper)    ||
        (player == move_paper    && computer == move_rock))
    {
        return result_win;
    }

    return result_lose;
}

copied move_t computer_choose()
{
    return (move_t)(rand() % 3);
}

copied move_t player_choose()
{
    borrowed const char * moves[3] = {
        rocks[rock_style],
        papers[paper_style],
        scissors[scissor_style],
    };

    int8_t idx = choose_item("Your move: ", moves, 3);
    return (move_t) idx;
}

void display_result(copied move_t player, copied move_t computer, copied result_t result)
{
    printf("\r\n");
    printf("You:      %s %s\r\n", get_move_emoji(player), get_move_name(player));
    printf("Computer: %s %s\r\n", get_move_emoji(computer), get_move_name(computer));
    printf("\r\n");

    switch (result)
    {
        case result_win:
            printf("%s " CRAYON_TO_GREEN("You win!") "\r\n", trophy);
            break;
        case result_lose:
            printf("%s " CRAYON_TO_RED("You lose!") "\r\n", defeated);
            break;
        case result_draw:
            printf("%s " CRAYON_TO_YELLOW("It's a draw!") "\r\n", attention);
            break;
    }
}

copied bool ask_play_again()
{
    printf("\r\nPlay again? [Y/n] ");
    fflush(stdout);

    loop
    {
        copied key_t key = keyboard_key_event();
        if (key == 'y' || key == 'Y' || key == key_enter)
        {
            printf("Yes\r\n\r\n");
            return true;
        }
        else if (key == 'n' || key == 'N' || key == (ctrl_mask | 'q'))
        {
            printf("No\r\n");
            return false;
        }
    }
}

void choose_styles()
{
    printf("=== Choose Your Styles ===\r\n\r\n");

    rock_style    = choose_item("Rock style:     ", rocks, rocks_count);
    paper_style   = choose_item("Paper style:    ", papers, papers_count);
    scissor_style = choose_item("Scissors style: ", scissors, scissors_count);

    printf("\r\n");
}

void play_round()
{
    copied move_t player_move   = player_choose();
    copied move_t computer_move = computer_choose();
    copied result_t result      = judge(player_move, computer_move);

    display_result(player_move, computer_move, result);
}

void setup()
{
    srand((unsigned int) time(nil));
    terminal_enter_raw_mode();
}

void fin()
{
    terminal_leave_raw_mode();
}

int main()
{
    setup();

    printf(CRAYON_TO_BOLD("=== Rock Paper Scissors ===") "\r\n");
    printf("Ctrl-Q to quit anytime\r\n\r\n");

    choose_styles();

    do
    {
        play_round();
    } while (ask_play_again());

    printf("\r\nThanks for playing!\r\n");

    fin();

    return 0;
}
