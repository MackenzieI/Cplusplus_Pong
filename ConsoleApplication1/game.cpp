#define is_down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1;


int player_1_score, player_2_score;

enum Gamemode {
    GAMEMENU,
    GAMEPLAY,
};

Gamemode current_gamemode;

int hot_button; 
bool enemy_is_ai; 

//global_variable float speed; 

internal void simulate_movement(float* player_p, float* player_dp, float player_ddp, float dt) {
    //player velocity
    player_ddp -= *player_dp * 10.f;

    *player_p = *player_p + *player_dp * dt + player_ddp * dt * dt * .5f;
    *player_dp = *player_dp + player_ddp * dt;

    // collision 
    if (*player_p + player_half_size_y > arena_half_size_y) {
        *player_p = arena_half_size_y - player_half_size_y;
        *player_dp = 0;
    }
    else if (*player_p - player_half_size_y < -arena_half_size_y) {
        *player_p = -arena_half_size_y + player_half_size_y;
        *player_dp = 0;
    }
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
    float p2x, float p2y, float hs2x, float hs2y) {
    return (p1x + hs1x > p2x - hs2x &&
        p1x - hs1x < p2x + hs2x &&
        p1y + hs1y > p2y - hs2y &&
        p1y + hs1y < p2y + hs2y);
}

internal void simulate_game(Input* input, float dt) {
    draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x022222);
    draw_arena_borders(arena_half_size_x, arena_half_size_y, 0x000000); 

    if (current_gamemode == GAMEPLAY) {
        float player_1_ddp = 0.f;
        if (is_down(BUTTON_UP)) player_1_ddp += 2000;
        if (is_down(BUTTON_DOWN)) player_1_ddp -= 2000;

        float player_2_ddp = 0.f;
        if (!enemy_is_ai) {
            if (is_down(BUTTON_W)) player_2_ddp += 2000;
            if (is_down(BUTTON_S)) player_2_ddp -= 2000;
        }
        else {
            player_2_ddp = (ball_p_y - player_2_p) * 100;
            if (player_2_ddp > 1300) player_2_ddp = 1300;
            if (player_2_ddp < -1300) player_2_ddp = -1300;

            if (ball_p_x < player_2_p + 10) {
                if (player_1_p > player_2_p) player_2_ddp = 1000;
                if (player_1_p < player_2_p) player_2_ddp = -1000;
            }
        }

        simulate_movement(&player_1_p, &player_1_dp, player_1_ddp, dt);
        simulate_movement(&player_2_p, &player_2_dp, player_2_ddp, dt);

        //simulate ball
        {
            ball_p_x += ball_dp_x * dt;
            ball_p_y += ball_dp_y * dt;

            if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_1_p, player_half_size_x, player_half_size_y)) {
                ball_p_x = -80 + player_half_size_x + ball_half_size;
                ball_dp_x *= -1;
                ball_dp_y = player_1_dp * .75f;
            }
            else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_2_p, player_half_size_x, player_half_size_y)) {
                ball_p_x = 80 - player_half_size_x - ball_half_size;
                ball_dp_x *= -1;
                ball_dp_y = player_2_dp * .75f;
            }

            if (ball_p_y + ball_half_size > arena_half_size_y) {
                ball_p_y = arena_half_size_y - ball_half_size;
                ball_dp_y *= -1;
            }
            else if (ball_p_y - ball_half_size < -arena_half_size_y) {
                ball_p_y = -arena_half_size_y + ball_half_size;
                ball_dp_y *= -1;
            }

            if (ball_p_x + ball_half_size > arena_half_size_x) {
                ball_dp_x *= -1;
                ball_dp_y = 0;
                ball_p_x = 0;
                ball_p_y = 0;
                player_1_score++;
            }
            else if (ball_p_x - ball_half_size < -arena_half_size_x) {
                ball_dp_x *= -1;
                ball_dp_y = 0;
                ball_p_x = 0;
                ball_p_y = 0;
                player_2_score++;
            }
        }

        draw_number(player_1_score, -10, 40, 1.f, 0xbbffbb);
        draw_number(player_2_score, 10, 40, 1.f, 0xbbffbb);

        draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff);

        draw_rect(-80, player_1_p, 2.5, 12, 0xffff22);
        draw_rect(80, player_2_p, 2.5, 12, 0xffffff);
        /*
        if (speedbtn_clicked == true) {
            speed += 100.f;
        }*/
    }
    else {
        if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
            hot_button = ~hot_button; 
        }

        if (pressed(BUTTON_ENTER)) {
            current_gamemode = GAMEPLAY; 
            enemy_is_ai = hot_button ? 0 : 1; 
        }

        if (hot_button == 0) {
            draw_text("SINGLE PLAYER", -80, -10, 1, 0xff00f0);
            draw_text("MULTIPLAYER", 20, -10, 1, 0xcccccc);
        }
        else {
            draw_text("SINGLE PLAYER", -80, -10, 1, 0xcccccc);
            draw_text("MULTIPLAYER", 20, -10, 1, 0xff00f0);
        }

        draw_text("game of pong", -67, 40, 2, 0xffffff); 
        draw_text("MADE USING THE TUTORIAL ON", -58, 24, .75, 0xffffff); 
        draw_text("YOUTUBE.COM/DANZAIDAN", -73, 15, 1.22, 0xffffff); 
        draw_text("press escape to exit.", -83, -42, .25, 0xffffff);
    }
}
