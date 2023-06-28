#include <boot/stivale2.h>
#include <devices/keyboard/keyboard.h>
#include <devices/pit/pit.h>
#include <devices/rtc/rtc.h>
#include <devices/screen/draw.h>
#include <devices/screen/screen.h>
#include <devices/serial/serial.h>
#include <libasm/asm.h>
#include <liblog/logging.h>
#include <libmath/math.h>
#include <librand/random.h>
#include <libstr/string.h>
#include <stddef.h>
#include <stdint.h>
#include <system/gdt/gdt.h>
#include <system/interrupts/idt.h>

#define TIMER_HZ 363
#define FPS 30

// Colours
colour_t tile_colours[] = {
    RGB(205, 193, 180),  // 0
    RGB(238, 228, 218),  // 2
    RGB(237, 224, 200),  // 4
    RGB(242, 177, 121),  // 8
    RGB(245, 149, 99),   // 16
    RGB(246, 124, 95),   // 32
    RGB(246, 94, 59),    // 64
    RGB(237, 207, 114),  // 128
    RGB(237, 204, 97),   // 256
    RGB(237, 200, 80),   // 512
    RGB(237, 197, 63),   // 1024
    RGB(237, 194, 46)    // 2048
};

colour_t tile_text_colours[] = {
    RGB(0, 0, 0),        // 0; no text for empty
    RGB(119, 110, 101),  // 2
    RGB(119, 110, 101),  // 4
    RGB(249, 246, 242),  // 8
    RGB(249, 246, 242),  // 16
    RGB(249, 246, 242),  // 32
    RGB(249, 246, 242),  // 64
    RGB(249, 246, 242),  // 128
    RGB(249, 246, 242),  // 256
    RGB(249, 246, 242),  // 512
    RGB(249, 246, 242),  // 1024
    RGB(249, 246, 242),  // 2048
};

#define BLACK RGB(0, 0, 0)
#define WHITE RGB(255, 255, 255)
#define BROWN RGB(120, 110, 101)

#define GRID_COLOUR RGB(187, 173, 160)

// Positioning
#define BOX_SIZE 100
#define PADDING 20

#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)

#define CENTER_TITLE_Y (CENTER_Y / 4)
#define CENTER_GRID_Y (5 * CENTER_Y / 4)

#define GRID_SIZE (5 * PADDING + 4 * BOX_SIZE)
#define GRID_X CENTER_X - (GRID_SIZE / 2)
#define GRID_Y CENTER_GRID_Y - (GRID_SIZE / 2)

#define DIALOG_PADDING 10
#define DIALOG_WIDTH 300
#define DIALOG_HEIGHT 200
#define DIALOG_X CENTER_X - (DIALOG_WIDTH / 2)
#define DIALOG_Y CENTER_GRID_Y - (DIALOG_HEIGHT / 2)

#define DIALOG_PADDING_WIDTH (DIALOG_WIDTH + DIALOG_PADDING)
#define DIALOG_PADDING_HEIGHT (DIALOG_HEIGHT + DIALOG_PADDING)
#define DIALOG_PADDING_X CENTER_X - (DIALOG_PADDING_WIDTH / 2)
#define DIALOG_PADDING_Y CENTER_GRID_Y - (DIALOG_PADDING_HEIGHT / 2)

// State
#define NUM_CONTROLS 5

struct control {
  bool down;
  bool last;
  bool pressed;
};

static struct {
  uint16_t grid[4][4];
  uint16_t score;
  enum { NOT_OVER, WIN, LOSS } progress;
  union {
    struct {
      struct control up;
      struct control left;
      struct control down;
      struct control right;
      struct control restart;
    };
    struct control raw[NUM_CONTROLS];
  } controls;
} state;

void compress(void) {
  for (int x = 0; x < 4; x++) {
    int pos = 0;
    for (int y = 0; y < 4; y++) {
      if (state.grid[x][y] != 0) {
        state.grid[x][pos] = state.grid[x][y];
        if (y != pos) {
          state.grid[x][y] = 0;
        }
        pos += 1;
      }
    }
  }
}

void merge(void) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 3; y++) {
      if (state.grid[x][y] == state.grid[x][y + 1] && state.grid[x][y] != 0) {
        state.grid[x][y] = state.grid[x][y] + 1;
        state.grid[x][y + 1] = 0;

        state.score += pow(2, state.grid[x][y]);
      }
    }
  }
}

void reverse(void) {
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 2; y++) {
      uint16_t temp = state.grid[x][y];
      state.grid[x][y] = state.grid[x][3 - y];
      state.grid[x][3 - y] = temp;
    }
  }
}

void transpose(void) {
  for (int x = 0; x < 4; x++) {
    for (int y = x + 1; y < 4; y++) {
      uint16_t temp = state.grid[x][y];
      state.grid[x][y] = state.grid[y][x];
      state.grid[y][x] = temp;
    }
  }
}

void move_up(void) {
  compress();
  merge();
  compress();
}

void move_left(void) {
  transpose();
  move_up();
  transpose();
}

void move_down(void) {
  reverse();
  move_up();
  reverse();
}

void move_right(void) {
  transpose();
  move_down();
  transpose();
}

void insert_new(void) {
  int x = rand() % 4;
  int y = rand() % 4;

  int i = 0;

  while (state.grid[x][y] != 0) {
    // avoids getting stuck in while loop
    if (i > 16) {
      return;
    }

    x = rand() % 4;
    y = rand() % 4;
    i++;
  }

  // 10% chance of a 4, 90% chance of 2
  int chance = rand() % 10;
  if (chance == 0) {
    state.grid[x][y] = 2;
  } else {
    state.grid[x][y] = 1;
  }
}

void update_game_progress(void) {
  int x, y;
  int hasEmptyCell = 0;

  // Check for winning condition and empty cells
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      if (state.grid[x][y] == 11) {
        state.progress = WIN;
        return;
      }
      if (state.grid[x][y] == 0) {
        hasEmptyCell = 1;
      }
      if (x < 3 && state.grid[x][y] == state.grid[x + 1][y]) {
        state.progress = NOT_OVER;
        return;
      }
      if (y < 3 && state.grid[x][y] == state.grid[x][y + 1]) {
        state.progress = NOT_OVER;
        return;
      }
    }
  }

  if (hasEmptyCell) {
    // There is an empty cell, game is not over
    state.progress = NOT_OVER;
  } else {
    state.progress = LOSS;
  }
}

void render_grid_background(void) {
  draw_rect(GRID_X, GRID_Y, GRID_SIZE, GRID_SIZE, GRID_COLOUR);
}

void render_ui(void) {
  font_puts_center("2048os", CENTER_X, CENTER_TITLE_Y, 10, BROWN, BLACK);
  font_puts_center("gitlab.com/Jpac14/2048-os", CENTER_X,
                   CENTER_TITLE_Y + FONT_HEIGHT * 6, 2, WHITE, BLACK);

  font_puts("Score:", GRID_X, GRID_Y - FONT_HEIGHT * 3, 2, WHITE, BLACK);
  font_puts("wasd or arrows to move grid; r to restart", GRID_X,
            GRID_Y + GRID_SIZE + FONT_HEIGHT, 2, WHITE, BLACK);
}

void render_boxes(void) {
  int x, y;
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      uint16_t box_x = GRID_X + (x + 1) * PADDING + x * BOX_SIZE;
      uint16_t box_y = GRID_Y + (y + 1) * PADDING + y * BOX_SIZE;
      colour_t box_colour = tile_colours[state.grid[x][y]];

      draw_rect(box_x, box_y, BOX_SIZE, BOX_SIZE, box_colour);

      if (state.grid[x][y] != 0) {
        char *text = itoa(pow(2, state.grid[x][y]), 10);
        colour_t text_colour = tile_text_colours[state.grid[x][y]];

        font_puts_center(text, box_x + (BOX_SIZE / 2), box_y + (BOX_SIZE / 2),
                         4, text_colour, box_colour);
      }
    }
  }
}

void render_score(void) {
  font_puts(itoa(state.score, 10), GRID_X + FONT_WIDTH * 14,
            GRID_Y - FONT_HEIGHT * 3, 2, WHITE, BLACK);
}

void render_dialog(void) {
  draw_rect(DIALOG_PADDING_X, DIALOG_PADDING_Y, DIALOG_PADDING_WIDTH,
            DIALOG_PADDING_HEIGHT, WHITE);
  draw_rect(DIALOG_X, DIALOG_Y, DIALOG_WIDTH, DIALOG_HEIGHT, BROWN);
}

void render_win(void) {
  render_dialog();

  font_puts_center("You Win!", CENTER_X, CENTER_GRID_Y - FONT_HEIGHT, 3,
                   WHITE, BROWN);
  font_puts_center("press r to restart", CENTER_X,
                   CENTER_GRID_Y + (FONT_HEIGHT * 2), 2, WHITE, BROWN);
}

void render_loss(void) {
  render_dialog();

  font_puts_center("You Lose!", CENTER_X, CENTER_GRID_Y - FONT_HEIGHT, 2,
                   WHITE, BROWN);
  font_puts_center("press r to restart", CENTER_X,
                   CENTER_GRID_Y + (FONT_HEIGHT * 2), 1, WHITE, BROWN);
}

void render(void) {
  set_screen(get_colour(&BLACK));
  render_grid_background();
  render_ui();
  render_boxes();
  render_score();

  switch (state.progress) {
    case WIN:
      render_win();
      break;
    case LOSS:
      render_loss();
      break;
    case NOT_OVER:
      break;
  }
}

void start(void) {
  int x, y;
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      state.grid[x][y] = 0;
    }
  }

  state.progress = NOT_OVER;
  state.score = 0;

  insert_new();
  insert_new();
}

void update(void) {
  // Update control states
  // clang-format off
  bool control_states[] = {
    keyboard_char('w') || keyboard_key(KEY_UP),
    keyboard_char('a') || keyboard_key(KEY_LEFT),
    keyboard_char('s') || keyboard_key(KEY_DOWN),
    keyboard_char('d') || keyboard_key(KEY_RIGHT),
    keyboard_char('r')
  };
  // clang-format on

  for (int i = 0; i < NUM_CONTROLS; i++) {
    struct control *c = &state.controls.raw[i];
    c->last = c->down;
    c->down = control_states[i];
    c->pressed = !c->last && c->down;
  }

  bool inputDetected = false;
  if (state.controls.up.pressed) {
    move_up();
    inputDetected = true;
  } else if (state.controls.left.pressed) {
    move_left();
    inputDetected = true;
  } else if (state.controls.down.pressed) {
    move_down();
    inputDetected = true;
  } else if (state.controls.right.pressed) {
    move_right();
    inputDetected = true;
  } else if (state.controls.restart.pressed) {
    start();
    return;
  }

  update_game_progress();

  if (inputDetected && state.progress == NOT_OVER) {
    insert_new();
  }
}

void _start(struct stivale2_struct *info) {
  init_serial();
  init_gdt();
  init_idt();
  init_pit(TIMER_HZ);
  init_rtc();
  init_screen(info);
  init_keyboard();

  // Set random
  datetime_t current = rtc_get_datetime();
  srand(current.second);

  serial_puts("Hello From OS!\n");

  start();

  uint64_t last = pit_get_ticks();

  for (;;) {
    uint64_t now = pit_get_ticks();

    if ((now - last) > (TIMER_HZ / FPS)) {
      last = now;

      update();
      render();

      screen_swap();
    }
  }
}
