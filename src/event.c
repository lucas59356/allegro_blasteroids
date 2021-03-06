#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <blasteroids/config.h>

#include <blasteroids/context.h>
#include <blasteroids/main.h> // Função stop
#include <blasteroids/event.h>
#include <blasteroids/spaceship_struct.h>
#include <blasteroids/spaceship_ops.h>
#include <blasteroids/util_log.h>


void event_loop_once(GameContext *ctx, ALLEGRO_EVENT *event) {
    al_wait_for_event(ctx->event_queue, event);
    handle_event(event, ctx);
}

void game_over(GameContext *ctx) {
    printf("========== GAME OVER ==========\n");
    printf("Você morreu :c.\n");
    printf("Você conseguiu %i pontos.\n", ctx->score);
    printf("Tempo de jogo: %i min %i s\n", ctx->HearthBeat/(60*FPS), (ctx->HearthBeat/FPS)%60);
    printf("===============================\n");
    fflush(stdout);
    handle_shutdown(SIGINT); // Finalizando o jogo
}


void handle_event(ALLEGRO_EVENT *ev, GameContext *ctx) {
    if (ctx->ship.health <= 0) {
        game_over(ctx);
    }
    Bullet bt;
    if(ev->type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (ev->keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                blasteroids_ship__left(&ctx->ship);
                blasteroids_context__update(ctx);
                return;
            case ALLEGRO_KEY_RIGHT:
                blasteroids_ship__right(&ctx->ship);
                blasteroids_context__update(ctx);
                return;
            case ALLEGRO_KEY_UP:
                blasteroids_ship__up(&ctx->ship);
                blasteroids_context__update(ctx);
                return;
            case ALLEGRO_KEY_DOWN:
                blasteroids_ship__down(&ctx->ship);
                blasteroids_context__update(ctx);
                return;
            case ALLEGRO_KEY_ESCAPE:
                info("Sair");
                stop(0);
                return;
            case ALLEGRO_KEY_SPACE:
                /*if (ctx->HearthBeat%2)*/ blasteroids_bullet__shot(ctx);
                return;
        }
    }
    if(ev->type == ALLEGRO_EVENT_TIMER) {
        ctx->HearthBeat = ctx->HearthBeat + 1;
        blasteroids_context__tick(ctx);
        if (!(ctx->HearthBeat%(10*FPS)))
            blasteroids_asteroid__generate_and_append(ctx);
    }
    if(ev->type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        stop(0);
        return;
    }
    if(ev->type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
        if (!al_acknowledge_resize(ctx->display))
            error("Não foi possível redimensionar o display");
    }
}


