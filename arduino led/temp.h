// switch (ir_command) {
    //     case IR_CH_minus:
    //         --cur_mode_idx;
    //         if (cur_mode_idx > MODE_SIZE - 1) {
    //             cur_mode_idx = 8;
    //         }
    //         delay(100);
    //         break;
    //     case IR_CH_plus:
    //         ++cur_mode_idx;
    //         if (cur_mode_idx > MODE_SIZE - 1) {
    //             cur_mode_idx = 0;
    //         }
    //         delay(100);
    //         break;
    //     case IR_VOL_minus:
    //         if (cur_brightness > 1)
    //             cur_brightness -= 2;
    //         delay(5);
    //         break;
    //     case IR_VOL_plus:
    //         if (cur_brightness < 254)
    //             cur_brightness += 2;
    //         delay(5);
    //         break;
    //     case IR_0:
    //         cur_mode_idx = 0;
    //         cur_secondary_mode_idx = 0;
    //         chosen_color = 0;
    //         snake_head = 0;
    //         snake_color_change = 0;
    //         snake_control = false;
    //         cur_brightness = 30;
    //         delay(100);
    //         break;
    //     case IR_1:
    //         cur_mode_idx = 0;
    //         delay(100);
    //         break;
    //     case IR_2:
    //         cur_mode_idx = 1;
    //         delay(100);
    //         break;
    //     case IR_3:
    //         cur_mode_idx = 2;
    //         delay(100);
    //         break;
    //     case IR_4:
    //         cur_mode_idx = 3;
    //         delay(100);
    //         break;
    //     case IR_5:
    //         cur_mode_idx = 4;
    //         delay(100);
    //         break;
    //     case IR_100_plus:
    //         cur_secondary_mode_idx = 1;
    //         delay(100);
    //         break;

    //     default:
    //         break;
    // }