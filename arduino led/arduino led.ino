#include <FastLED.h>

#define NUM_LEDS 60
#define DATA_PIN 13
#define MODE_BUTTON_PIN 8
#define MODE_2_BUTTON_PIN 10
#define COLORS_SIZE 6
#define BRIGHTNESS_SIZE 3
#define MODE_SIZE 2
#define STICK_Y_PIN 0
#define STICK_X_PIN 1

CRGB leds[NUM_LEDS];

enum Color {
	RED = 0,
	ORANGE = 15,
	YELLOW = 50,
	GREEN = 100,
	BLUE = 150,
	PURPLE = 250
};

enum Brightness {
    DEFAULT_BRIGHTNESS = 50,
    HIGHT_BRIGHTNESS = 150,
    LOW_BRIGHTNESS = 0
};

Color array_color[COLORS_SIZE] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
Brightness array_brigt[BRIGHTNESS_SIZE] = {DEFAULT_BRIGHTNESS, HIGHT_BRIGHTNESS, LOW_BRIGHTNESS};

void setup() 
{
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    // FastLED.setBrightness(50);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);
    Serial.begin(9600);
    pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
    pinMode(MODE_2_BUTTON_PIN, INPUT_PULLUP);
}

// buttons control
bool snake_control = false;

uint8_t cur_mode, prev_mode, cur_mode_2, prev_mode_2;

uint8_t cur_mode_idx = 0, cur_mode_idx_2 = 0, chosen_color = 0;

uint8_t snake_color_change = 0;
int snake_head, snake_cur_pos, snake_tail = 0, snake_len = 30;

int y_val, x_val;

void loop()
{
    cur_mode = digitalRead(MODE_BUTTON_PIN);
    if (prev_mode == HIGH && cur_mode == LOW) {
        ++cur_mode_idx;
        if (cur_mode_idx > MODE_SIZE - 1) cur_mode_idx = 0;
    }
    prev_mode = digitalRead(MODE_BUTTON_PIN);

    cur_mode_2 = digitalRead(MODE_2_BUTTON_PIN);
    if (prev_mode_2 == HIGH && cur_mode_2 == LOW) {
        cur_mode_idx_2 = 1;
    }
	prev_mode_2 = digitalRead(MODE_2_BUTTON_PIN);
	
    if (cur_mode_idx == 0) {
        if (cur_mode_idx_2 == 1) {
            chosen_color += 10;
            if (chosen_color > 255) {
                chosen_color = 0;
            }
        }
        cur_mode_idx_2 = 0;
        
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(chosen_color, 255, 30);
        }
        FastLED.show();
    } 
	else {
        for (int i = 0 ; i < NUM_LEDS; i++ ) {
            snake_head = snake_tail + snake_len;
            if (snake_head >= NUM_LEDS) {
                snake_head -= NUM_LEDS; 
                if (i > snake_tail) {
                    snake_cur_pos = i - snake_tail - 1;
                    leds[i] = CHSV(255 * snake_cur_pos / snake_len + snake_color_change, 255, 50);
                } 
                else if (i < snake_head) {
                    snake_cur_pos = i + (NUM_LEDS - snake_tail - 1);
                    leds[i] = CHSV(255 * snake_cur_pos / snake_len + snake_color_change, 255, 50);
                }
                else {
                    leds[i] = CHSV(0, 255, 0);
                }
            }
            else {
                if(i > snake_tail && i < snake_head) {
                    snake_cur_pos = i - snake_tail - 1;
                    leds[i] = CHSV(255 * snake_cur_pos / snake_len + snake_color_change, 255, 50);
                }
                else {
                    leds[i] = CHSV(0, 255, 0);
                }
            }
        }

        if (cur_mode_idx_2 == 1) {
            snake_control = !snake_control;
        }
        cur_mode_idx_2 = 0;

        if (snake_control) {
            // up and right = 0, default <500, max >1000
            x_val = analogRead(STICK_X_PIN);
            if (x_val > 1000) {
                --snake_color_change;
            }
            else if (x_val < 10) {
                ++snake_color_change;
            }

            y_val = analogRead(STICK_Y_PIN);
            if (y_val > 1000) {
                --snake_tail;
            }
            else if (y_val < 10) {
                ++snake_tail;
            }
        }
        else {
            ++snake_tail;
        }

        if (snake_tail >= NUM_LEDS) {
            snake_tail = 0;
        }
        else if (snake_tail < 0) {
            snake_tail = NUM_LEDS - 1;
        }

        FastLED.show();
        delay(30); 
    }
}