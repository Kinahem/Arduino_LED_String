#include <FastLED.h>
#include <IRremote.h>

#define IR_PIN 5
#define NUM_LEDS 60
#define DATA_PIN 13
#define MODE_BUTTON_PIN 8
#define MODE_2_BUTTON_PIN 10
#define MODE_SIZE 9
#define STICK_Y_PIN 0
#define STICK_X_PIN 1

// IR buttons
#define IR_CH_minus 69
#define IR_CH 69
#define IR_CH_plus 71
#define IR_BWD 68
#define IR_FWD 64
#define IR_PLAY 67
#define IR_VOL_minus 7
#define IR_VOL_plus 21
#define IR_EQ 9
#define IR_0 22
#define IR_100_plus 25
#define IR_200_plus 13
#define IR_1 12
#define IR_2 24
#define IR_3 94
#define IR_4 8
#define IR_5 28
#define IR_6 90
#define IR_7 66
#define IR_8 82
#define IR_9 74

CRGB leds[NUM_LEDS];

uint16_t ir_command = 0;
uint8_t cur_mode_idx = 0, cur_secondary_mode_idx = 0;
uint8_t chosen_color = 0, cur_brightness = 30;

bool snake_control = false;
uint8_t snake_color_change = 0;
int snake_head = 0, snake_len = 30;
int y_val, x_val;

int timer = 0, timer_max = 1000;
uint32_t color_cycle[] = {0, 96, 160};
uint8_t color_cycle_idx = 0;
bool fade = true;
uint8_t fade_brightness = 50;

// float my_round(float num) {
//     return (float)((int)(num * 100.0) / 100.0);
// }

void setup() {
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    // FastLED.setBrightness(50);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 2000);
    Serial.begin(9600);

    IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
    // IR receiver
    while (!IrReceiver.isIdle()) {
    };
    if (IrReceiver.decode()) {
        ir_command = IrReceiver.decodedIRData.command;
        // Serial.println(IrReceiver.decodedIRData.command);
        // IrReceiver.printIRResultShort(&Serial);
    }

    switch (ir_command) {
        case IR_CH_minus:
            --cur_mode_idx;
            if (cur_mode_idx > MODE_SIZE - 1) {
                cur_mode_idx = 8;
            }
            delay(100);
            break;
        case IR_CH_plus:
            ++cur_mode_idx;
            if (cur_mode_idx > MODE_SIZE - 1) {
                cur_mode_idx = 0;
            }
            delay(100);
            break;
        case IR_VOL_minus:
            if (cur_brightness > 1)
                cur_brightness -= 2;
            delay(5);
            break;
        case IR_VOL_plus:
            if (cur_brightness < 254)
                cur_brightness += 2;
            delay(5);
            break;
        case IR_0:
            cur_mode_idx = 0;
            cur_secondary_mode_idx = 0;
            chosen_color = 0;
            snake_head = 0;
            snake_color_change = 0;
            snake_control = false;
            cur_brightness = 30;
            delay(100);
            break;
        case IR_1:
            cur_mode_idx = 0;
            delay(100);
            break;
        case IR_2:
            cur_mode_idx = 1;
            delay(100);
            break;
        case IR_3:
            cur_mode_idx = 2;
            delay(100);
            break;
        case IR_4:
            cur_mode_idx = 3;
            delay(100);
            break;
        case IR_5:
            cur_mode_idx = 4;
            delay(100);
            break;
        case IR_100_plus:
            cur_secondary_mode_idx = 1;
            delay(100);
            break;

        default:
            break;
    }
    ir_command = 0;
    IrReceiver.resume();  // unlock ir receive

    // LOGIC
    switch (cur_mode_idx) {
        case 0:
            if (cur_secondary_mode_idx == 1) {
                chosen_color += 10;
                if (chosen_color > 255) {
                    chosen_color = 0;
                }
            }
            cur_secondary_mode_idx = 0;

            fill_solid(leds, NUM_LEDS, CHSV(chosen_color, 255, cur_brightness));
            FastLED.show();
            break;

        case 1:
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

            if (cur_secondary_mode_idx == 1) {
                snake_control = !snake_control;
            }
            cur_secondary_mode_idx = 0;

            if (snake_control) {
                // up and right = 0, default <500, max >1000
                x_val = analogRead(STICK_X_PIN);
                if (x_val > 1000) {
                    --snake_color_change;
                } else if (x_val < 10) {
                    ++snake_color_change;
                }

                y_val = analogRead(STICK_Y_PIN);
                if (y_val > 1000) {
                    --snake_head;
                } else if (y_val < 10) {
                    ++snake_head;
                }
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
            break;

        case 2:
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
            break;

        default:
            break;
    }
}