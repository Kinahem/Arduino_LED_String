#include <FastLED.h>

#define NUM_LEDS 59
#define DATA_PIN 13
#define STICK_Y_PIN 4
#define STICK_X_PIN 3
#define STICK_SW_PIN 8

#define NUM_MODS 4

CRGB leds[NUM_LEDS];

// single color mode
uint8_t cur_mode_idx = 0;
uint8_t chosen_color = 0, cur_brightness = 50;

// snake mode
bool snake_control = true;
uint8_t snake_color_change = 0;
int snake_head = 0, snake_len = 30;
int y_val, x_val;

// fade mode
int timer = 0, timer_max = 1000;
uint32_t color_cycle[] = {0, 96, 160};
uint8_t color_cycle_idx = 0;
bool fade = true;
uint8_t fade_brightness = 50;


void setup() {
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    // FastLED.setBrightness(50);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);
    // Serial.begin(9600);
    pinMode(STICK_SW_PIN, INPUT_PULLUP); 
}

void loop() {

    if (digitalRead(STICK_SW_PIN) == 0) {
        cur_mode_idx += 1;
        if (cur_mode_idx >= NUM_MODS) cur_mode_idx = 0;
        delay(300);
    }    
    
    // LOGIC
    switch (cur_mode_idx) {
        case 0:
            single_color_mode();
            break;

        case 1:
            snake_mode(snake_control=false);
            break;
        
        case 2:
            snake_mode(snake_control=true);
            break;

        case 3:
            fade_mode();
            break;

        default:
            break;
    }
}


void single_color_mode() {
    change_params();
    fill_solid(leds, NUM_LEDS, CHSV(chosen_color, 255, cur_brightness));
    FastLED.show();
}

void snake_mode(bool snake_control) {
    draw_snake();

    if (snake_control) {
        move_snake();
    } else {
        ++snake_head;
    }

    if (snake_head >= NUM_LEDS) {
        snake_head -= NUM_LEDS;
    } else if (snake_head < 0) {
        snake_head = NUM_LEDS - 1;
    }

    FastLED.show();
    delay(30);
}

void fade_mode() {
    if (timer > timer_max) {
        timer = 0;
        if (fade) {
            color_cycle_idx += 1;
            if (color_cycle_idx >= sizeof(color_cycle) / sizeof(color_cycle[0])) {
                color_cycle_idx = 0;
            }
        }
        fade = !fade;
    }

    if (timer % 10 == 0) {
        if (fade)
            fade_brightness -= 1;
        else
            fade_brightness += 1;
        fade_brightness = min(50, max(fade_brightness, 1));
        fill_solid(leds, NUM_LEDS, CHSV(color_cycle[color_cycle_idx], 255, fade_brightness));
    }

    ++timer;
    FastLED.show();
}

void change_params() {
    x_val = analogRead(STICK_X_PIN);
    if (x_val > 950) {
        chosen_color -= 3;
    } else if (x_val < 50) {
        chosen_color += 3;
    }

    y_val = analogRead(STICK_Y_PIN);
    if (y_val > 950) {
        cur_brightness -= 3;
    } else if (y_val < 50) {
        cur_brightness += 3;
    }
    if (cur_brightness > 255) {
        cur_brightness = 255;
    } else if (cur_brightness < 0) {
        cur_brightness = 0;
    }

    delay(100);    
}

void move_snake() {
    // uint8_t &snake_color_change, int &snake_head
    x_val = analogRead(STICK_X_PIN);
    // up and right = 0, default <500, max >1000
    if (x_val > 950) {
        --snake_color_change;
    } else if (x_val < 50) {
        ++snake_color_change;
    }

    y_val = analogRead(STICK_Y_PIN);
    if (y_val > 950) {
        --snake_head;
    } else if (y_val < 50) {
        ++snake_head;
    }
}

void draw_snake() {
    // int &snake_head, int &snake_len, uint8_t &snake_color_change, uint8_t &cur_brightness
    for (int i = 0; i < NUM_LEDS; ++i) {
        if (snake_head >= snake_len - 1) {
            if (i > snake_head - snake_len && i <= snake_head) {
                leds[i] = CHSV(255 * (snake_head - i + snake_color_change) / snake_len, 255, cur_brightness);
            } else {
                leds[i] = CHSV(0, 255, 0);
            }
        } else {
            if (i > (NUM_LEDS - snake_len) + snake_head) {
                leds[i] = CHSV(255 * (NUM_LEDS - i + snake_head + snake_color_change) / snake_len, 255, cur_brightness);
            } else {
                if (i <= snake_head) {
                    leds[i] = CHSV(255 * (snake_head - i + snake_color_change) / snake_len, 255, cur_brightness);
                } else {
                    leds[i] = CHSV(0, 255, 0);
                }
            }
        }
    }
}