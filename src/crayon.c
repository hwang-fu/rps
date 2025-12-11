#include "crayon.h"

void crayon_bold(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BOLD, stream);
}

void crayon_dim(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(DIM, stream);
}

void crayon_italic(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(ITALIC, stream);
}

void crayon_underline(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(UNDERLINE, stream);
}

void crayon_blink(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BLINK, stream);
}

void crayon_reversed(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(REVERSED, stream);
}

void crayon_strikethru(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(STRIKETHRU, stream);
}

void crayon_fg_black(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(FOREGROUND_BLACK, stream);
}

void crayon_fg_red(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(FOREGROUND_RED, stream);
}

void crayon_fg_green(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(FOREGROUND_GREEN, stream);
}

void crayon_fg_yellow(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(FOREGROUND_YELLOW, stream);
}

void crayon_fg_blue(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(FOREGROUND_BLUE, stream);
}

void crayon_fg_magenta(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(FOREGROUND_MAGENTA, stream);
}

void crayon_fg_cyan(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(FOREGROUND_CYAN, stream);
}

void crayon_fg_white(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(FOREGROUND_WHITE, stream);
}

void crayon_fg_gray(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(FOREGROUND_GRAY, stream);
}

void crayon_bg_black(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BACKGROUND_BLACK, stream);
}

void crayon_bg_red(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BACKGROUND_RED, stream);
}

void crayon_bg_green(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BACKGROUND_GREEN, stream);
}

void crayon_bg_yellow(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BACKGROUND_YELLOW, stream);
}

void crayon_bg_blue(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BACKGROUND_BLUE, stream);
}

void crayon_bg_magenta(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BACKGROUND_MAGENTA, stream);
}

void crayon_bg_cyan(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BACKGROUND_CYAN, stream);
}

void crayon_bg_white(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BACKGROUND_WHITE, stream);
}

void crayon_bg_gray(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(BACKGROUND_GRAY, stream);
}

void crayon_end(borrowed FILE * stream) {
    if (!stream) {
        return;
    }
    fputs(ENDCRAYON, stream);
}


