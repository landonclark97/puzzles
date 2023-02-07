#include <stdio.h>

#include "../../include/puzzles.h"

#include "../game/agent.h"
#include "../game/attributes.h"
#include "../game/game_system.h"


#define SCREEN_WIDTH (920)
#define SCREEN_HEIGHT (740)


int main(int argc, char* args[])
{
    window_t window;

    if (!window_init(&window, "engine systems testing", SCREEN_WIDTH, SCREEN_HEIGHT, VSYNC_OFF | FILTER_OFF)) {
        window_free(&window);
        return -1;
    }
    if (!audio_init()) {
        window_free(&window);
        return -1;
    }

    puzzles_thread_t th = input_handler_init(&window);
    phys_world_init((phys_vec_t){0.0,0.0});
    data_world_init();

    game_system_init();

    fprintf(stderr, "system init complete\n");

    double fps = 120000.0;
    if (window._vsync) {
        fps = window.refresh;
    }

    ticker_t ticker;
    ticker_init(&ticker, fps);

    agent_attr_t attr;
    phys_body_constructor_t bc;
    agent_attr_player(&attr, &window, "../assets/allen_stand.png", 10.0, 500.0, 2000.0, 10.0, 10.0, 10.0, 10.0, 0.1, 4, 8);
    phys_bc_set_circle(&bc, 1.0, 11.0, 0.0, (phys_vec_t) {0.0,0.0}, 0.0, 0.0, PLAYER_FILTER, NPC_FILTER | NPC_ATK_FILTER);

    agent_t player = agent_new(&attr, &bc);

    agent_instantiate(&player, (phys_vec_t){-70.0, 70.0});

    agent_attr_npc(&attr, &window, "../assets/skeleton_sheet.png", 10.0, 250.0, 500.0, 10.0, 10.0, 10.0, 10.0, 0.1, 4, 9);
    phys_bc_set_circle(&bc, 1.0, 36.0, 0.0, (phys_vec_t) {0.0,0.0}, 0.0, 0.0, NPC_FILTER, PLAYER_FILTER | NPC_FILTER | PLAYER_ATK_FILTER);

    agent_t npc = agent_new(&attr, &bc);

    for (float i = 0; i < 2000; i+=100) {
        for (float j = 0; j < 1000; j+=100) {
            agent_instantiate(&npc, (phys_vec_t){i, j});
        }
    }

    fprintf(stderr, "running main\n");
    double dt = 0.0;
    while (!(window._exit)) {
        if ((dt = ticker_decr_acc(&ticker))) {
            if (ticker._running) {
                game_step(dt);
                phys_world_step(dt);
                game_update(dt);
            }
            fprintf(stderr, "\rfps: %5.1f", 1.0/dt);
        }
    }
    input_handler_close(th);

    fprintf(stderr, "check for SDL Errors: %s\n", SDL_GetError());

    agent_free(&player);
    agent_free(&npc);

    game_free();
    audio_free();
    window_free(&window);

    return 0;
}
