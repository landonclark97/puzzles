#include <stdio.h>
#include <math.h>

#include "../../include/puzzles.h"

#include "./game_system.h"
#include "./agent.h"
#include "./attributes.h"
#include "./combat.h"
#include "./item.h"


static data_entity_id_t player_id;

static dict_t* phys_dict;

static int attacking;

static data_query_t* render_query,
    * update_pos_query,
    * check_health_query,
    * handle_atk_hit_query,
    * clean_atks_query,
    * check_atk_prep_query;

int game_handle_atk_collision(phys_arbiter_t* ab, phys_world_t* p_world, void* data);

void game_register_hitbox(data_iter_t* it);

void game_update_position(data_iter_t* it);

void game_handle_atk_hit(data_iter_t* it);

void game_clean_atks(data_iter_t* it);

void game_check_atk_prep(data_iter_t* it);

void game_check_health(data_iter_t* it);

void game_update_constraint_force(data_iter_t* it);

void game_cleanup_texture(data_iter_t* it);

void game_cleanup_hitbox(data_iter_t* it);

void game_cleanup_control_body(data_iter_t* it);

void game_cleanup_constraint(data_iter_t* it);

void game_cleanup_sound(data_iter_t* it);

void game_cleanup_music(data_iter_t* it);


int compare_pos(data_entity_id_t e1, const void* ptr1, data_entity_id_t e2, const void* ptr2) {
    (void)e1;
    (void)e2;
    const position *p1 = ptr1;
    const position *p2 = ptr2;
    return (p1->y > p2->y) - (p1->y < p2->y);
}


data_world_component_declare(position);
data_world_component_declare(hp);
data_world_component_declare(sp);
data_world_component_declare(mp);
data_world_component_declare(agility);
data_world_component_declare(strength);
data_world_component_declare(constitution);
data_world_component_declare(intelligence);
data_world_component_declare(wisdom);
data_world_component_declare(charisma);
data_world_component_declare(hitbox);
data_world_component_declare(control_body);
data_world_component_declare(move_constraint);
data_world_component_declare(physics_constructor);
data_world_component_declare(texture);
data_world_component_declare(sprite);
data_world_component_declare(hit);
data_world_component_declare(item_prop);
data_world_component_declare(tick);
data_world_component_declare(sound);
data_world_component_declare(music);

data_world_tag_declare(player);
data_world_tag_declare(npc);
data_world_tag_declare(enemy);
data_world_tag_declare(atk_tag);

data_world_type_declare(movement);
data_world_tag_declare(standing);
data_world_tag_declare(walking);
data_world_tag_declare(running);

data_world_type_declare(direction);
data_world_tag_declare(up);
data_world_tag_declare(down);
data_world_tag_declare(left);
data_world_tag_declare(right);

data_world_type_declare(agent_char);
data_world_type_declare(item);
data_world_type_declare(agent);
data_world_type_declare(environment);
data_world_type_declare(atk_prep);
data_world_type_declare(atk);
data_world_type_declare(atk_hit);


void game_system_init() {
    data_world_component_define(position);
    data_world_component_define(hp);
    data_world_component_define(sp);
    data_world_component_define(mp);
    data_world_component_define(agility);
    data_world_component_define(strength);
    data_world_component_define(constitution);
    data_world_component_define(intelligence);
    data_world_component_define(wisdom);
    data_world_component_define(charisma);
    data_world_component_define(hitbox);
    data_world_component_define(control_body);
    data_world_component_define(move_constraint);
    data_world_component_define(physics_constructor);
    data_world_component_define(texture);
    data_world_component_define(sprite);
    data_world_component_define(hit);
    data_world_component_define(item_prop);
    data_world_component_define(tick);
    data_world_component_define(sound);
    data_world_component_define(music);

    data_world_tag_define(player);
    data_world_tag_define(npc);
    data_world_tag_define(enemy);
    data_world_tag_define(atk_tag);

    data_world_tag_define(standing);
    data_world_tag_define(walking);
    data_world_tag_define(running);
    data_world_type_define(movement, standing, walking, running);

    data_world_tag_define(up);
    data_world_tag_define(down);
    data_world_tag_define(left);
    data_world_tag_define(right);
    data_world_type_define(direction, up, down, left, right);

    data_world_type_define(agent_char, hp, agility, strength, constitution, intelligence, wisdom, charisma);
    data_world_type_define(item, item_prop, physics_constructor);
    data_world_type_define(agent, position, agent_char, hitbox, control_body, move_constraint, sprite, item, SWITCH | movement, CASE | standing, SWITCH | direction, CASE | down);
    data_world_type_define(environment, position, hitbox, texture);
    data_world_type_define(atk_prep, item_prop, physics_constructor, tick, position);
    data_world_type_define(atk, hitbox, item_prop, atk_tag);
    data_world_type_define(atk_hit, item_prop, hit);

    data_world_add_trigger(game_cleanup_texture, EcsOnRemove, texture);
    data_world_add_trigger(game_cleanup_hitbox, EcsOnRemove, hitbox);
    data_world_add_trigger(game_cleanup_control_body, EcsOnRemove, control_body);
    data_world_add_trigger(game_cleanup_constraint, EcsOnRemove, move_constraint);
    data_world_add_trigger(game_cleanup_sound, EcsOnRemove, sound);
    data_world_add_trigger(game_cleanup_music, EcsOnRemove, music);

    data_world_system(game_register_hitbox, EcsOnSet, hitbox);
    data_world_system(game_update_constraint_force, EcsOnSet, strength, move_constraint);

    render_query = data_world_query("sprite, position, player || npc || enemy, SWITCH | movement, SWITCH | direction");
    update_pos_query = data_world_query("hitbox, position");
    check_health_query = data_world_query("hp, npc || enemy");
    handle_atk_hit_query = data_world_query("item_prop, hit");
    clean_atks_query = data_world_query("hitbox, item_prop, atk_tag");
    check_atk_prep_query = data_world_query("item_prop, physics_constructor, tick, position");
    /* data_world_query_order_by(render_query, position, compare_pos); */

    phys_dict = DICT_NEW();

    attacking = false;

    phys_world_collision_callback((void*) game_handle_atk_collision, PLAYER_ATK_FILTER, NPC_FILTER);
}

void game_set_player_id(data_entity_id_t id) {
    player_id = id;
}

void game_step(double dt) {
    agility agl = *data_world_get(player_id, agility);
    input_state_t* in_state = input_state_get();
    agility x_vel = 0.0, y_vel = 0.0;
    if (in_state->input_mode == KEYBOARD_MODE) {
        if (in_state->inputs[W]) {
            y_vel -= 1.0;
        }
        if (in_state->inputs[S]) {
            y_vel += 1.0;
        }
        if (in_state->inputs[A]) {
            x_vel -= 1.0;
        }
        if (in_state->inputs[D]) {
            x_vel += 1.0;
        }
        if (in_state->inputs[LSHIFT]) {
            agl = agl*1.5;
        }
        if (in_state->inputs[SPACE] && (!attacking)) {
            attacking = 1;
            const physics_constructor* atk_bc = data_world_get(player_id, physics_constructor);
            const position* p = data_world_get(player_id, position);
            const item_prop* ip = data_world_get(player_id, item_prop);
            data_entity_id_t ap_id = data_world_add_entity(atk_prep);
            data_world_set_ptr(ap_id, physics_constructor, atk_bc);
            data_world_set_ptr(ap_id, item_prop, ip);
            data_world_set(ap_id, tick, {.acc_time=0.0, .goal_time=0.1});
            data_world_set_ptr(ap_id, position, p);
        }
    }
    else if (in_state->input_mode == JOYSTICK_MODE) {
        int placeholder;
    }

    if (x_vel != 0.0 || y_vel != 0.0) {
        if (x_vel < 0.0) {
            if (y_vel + x_vel > 0.0) {
                data_world_set_tag(player_id, ECS_CASE | down);
            } else if(y_vel <= x_vel){
                data_world_set_tag(player_id, ECS_CASE | up);
            } else {
                data_world_set_tag(player_id, ECS_CASE | left);
            }
        }
        else if (x_vel > 0.0) {
            if (y_vel + x_vel <= 0.0) {
                data_world_set_tag(player_id, ECS_CASE | up);
            } else if(y_vel > x_vel){
                data_world_set_tag(player_id, ECS_CASE | down);
            } else {
                data_world_set_tag(player_id, ECS_CASE | right);
            }
        } else {
            if (y_vel < 0.0) {
                data_world_set_tag(player_id, ECS_CASE | up);
            } else {
                data_world_set_tag(player_id, ECS_CASE | down);
            }
        }
    }

    control_body* b = (control_body*) data_world_get(player_id, control_body);

    if (x_vel != 0.0 || y_vel != 0.0) {
        agility total_vel = sqrtf((x_vel*x_vel)+(y_vel*y_vel));
        x_vel = agl*(x_vel/total_vel);
        y_vel = agl*(y_vel/total_vel);
    }

    phys_body_set_velocity(b, (phys_vec_t) {x_vel, y_vel});
}

void game_check_health(data_iter_t* it) {
    hp* h = data_world_get_term(it, hp, 1);
    for (int i = 0; i < it->count; i++) {
        if (h[i] <= 0.0) {
            data_world_remove_entity(it->entities[i]);
        }
    }
}

void game_update_position(data_iter_t* it) {
    hitbox* bodies = data_world_get_term(it, hitbox, 1);
    position* pos = data_world_get_term(it, position, 2);
    for (int i = 0; i < it->count; i++) {
        pos[i] = phys_body_get_position(&bodies[i]);
    }
}

void game_handle_atk_hit(data_iter_t* it) {
    item_prop* itm = data_world_get_term(it, item_prop, 1);
    hit* h = data_world_get_term(it, hit, 2);
    for (int i = 0; i < it->count; i++) {
        if (h[i] != 0) {
            float damage = item_calc_damage(&itm[i]);
            data_entity_id_t h_id = dict_search(phys_dict, h[i]);
            *((hp*)data_world_get(h_id, hp)) -= damage;
        }
        data_world_remove_entity(it->entities[i]);
    }
}

void game_clean_atks(data_iter_t* it) {
    for (int i = 0; i < it->count; i++) {
        data_world_remove_entity(it->entities[i]);
    }
}

void game_check_atk_prep(data_iter_t* it) {
    item_prop* itm = data_world_get_term(it, item_prop, 1);
    physics_constructor* bc = data_world_get_term(it, physics_constructor, 2);
    tick* t = data_world_get_term(it, tick, 3);
    position* p = data_world_get_term(it, position, 4);
    for(int i = 0; i < it->count; i++) {
        t[i].acc_time += it->delta_time;
        if (t[i].acc_time >= t[i].goal_time) {
            bc[i].pos_x = p[i].x;
            bc[i].pos_y = p[i].y;
            hitbox b = phys_body_new(&bc[i]);
            data_entity_id_t a_id = data_world_add_entity(atk);
            data_world_set_ptr(a_id, hitbox, &b);
            data_world_set_ptr(a_id, item_prop, &itm[i]);
            data_world_remove_entity(it->entities[i]);
            attacking = 0;
        }
    }
}

int game_handle_atk_collision(phys_arbiter_t* ab, phys_world_t* p_world, void* data){
    hitbox a, b;
    phys_arbiter_get_body(ab, &a, &b);
    data_entity_id_t a_id = dict_search(phys_dict, (uint64_t) a.body);
    item_prop itm = *(item_prop*) data_world_get(a_id, item_prop);
    hit h = (uint64_t) b.body;
    data_entity_id_t ah_id = data_world_add_entity(atk_hit);
    data_world_set_ptr(ah_id, hit, &h);
    data_world_set_ptr(ah_id, item_prop, &itm);
    return 0;
}

void game_update(double dt) {
    data_iter_t it;

    it = data_world_query_iter(update_pos_query);
    while (data_world_query_next(&it)) {
        it.delta_time = dt;
        game_update_position(&it);
    }
    it = data_world_query_iter(check_health_query);
    while (data_world_query_next(&it)) {
        it.delta_time = dt;
        game_check_health(&it);
    }
    it = data_world_query_iter(handle_atk_hit_query);
    while (data_world_query_next(&it)) {
        it.delta_time = dt;
        game_handle_atk_hit(&it);
    }
    it = data_world_query_iter(clean_atks_query);
    while (data_world_query_next(&it)) {
        it.delta_time = dt;
        game_clean_atks(&it);
    }
    it = data_world_query_iter(check_atk_prep_query);
    while (data_world_query_next(&it)) {
        it.delta_time = dt;
        game_check_atk_prep(&it);
    }

    window_t* win = ((sprite*)data_world_get(player_id, sprite))->win;
    window_render_clear(win);
    phys_vec_t* p = (phys_vec_t*) data_world_get(player_id, position);
    camera_set_target(&win->cam, p->x, p->y, dt);
    it = data_world_query_iter(render_query);
    while (data_world_query_next(&it)) {
        sprite* sprites = data_world_get_term(&it, sprite, 1);
        phys_vec_t* pos = data_world_get_term(&it, phys_vec_t, 2);
        data_entity_id_t* m = data_world_get_term(&it, data_entity_id_t, 4);
        data_entity_id_t* d = data_world_get_term(&it, data_entity_id_t, 5);
        for (int i = 0; i < it.count; i++) {
            if (d[i] == data_world_check_tag(down)) {
                sprite_set_row(&sprites[i], 0);
            }
            if (d[i] == data_world_check_tag(up)) {
                sprite_set_row(&sprites[i], 1);
            }
            if (d[i] == data_world_check_tag(left)) {
                sprite_set_row(&sprites[i], 2);
            }
            if (d[i] == data_world_check_tag(right)) {
                sprite_set_row(&sprites[i], 3);
            }
            sprite_render_ex(&sprites[i], pos[i].x, pos[i].y, 1.0, 0.0, dt);
        }
    }
    window_render_present(win);
}

void game_register_hitbox(data_iter_t* it) {
    hitbox* b = data_world_get_term(it, hitbox, 1);
    for (int i = 0; i < it->count; i++) {
        dict_insert(&phys_dict, (uint64_t)b[i].body, it->entities[i]);
    }
}

void game_update_constraint_force(data_iter_t* it) {
    strength* str = data_world_get_term(it, strength, 1);
    move_constraint* c = data_world_get_term(it, move_constraint, 2);
    for (int i = 0; i < it->count; i++) {
        phys_constraint_set_force(c[i], str[i]);
    }
}

void game_cleanup_hitbox(data_iter_t* it) {
    hitbox* b = data_world_get_term(it, hitbox, 1);
    for (int i = 0; i < it->count; i++) {
        dict_remove(phys_dict, (uint64_t)b[i].body);
        phys_body_free(&b[i]);
    }
}

void game_cleanup_control_body(data_iter_t* it) {
    control_body* b = data_world_get_term(it, control_body, 1);
    for (int i = 0; i < it->count; i++) {
        dict_remove(phys_dict, (uint64_t)b[i].body);
        phys_body_free(&b[i]);
    }
}

void game_cleanup_texture(data_iter_t* it) {
    texture* t = data_world_get_term(it, texture, 1);
    for (int i = 0; i < it->count; i++) {
        texture_free(&t[i]);
    }
}

void game_cleanup_constraint(data_iter_t* it) {
    move_constraint* c = data_world_get_term(it, move_constraint, 1);
    for (int i = 0; i < it->count; i++) {
        phys_constraint_free(c[i]);
    }
}

void game_cleanup_sound(data_iter_t* it) {
    sound* s = data_world_get_term(it, sound, 1);
    for (int i = 0; i < it->count; i++) {
        sound_free(s[i]);
    }
}

void game_cleanup_music(data_iter_t* it) {
    music* m = data_world_get_term(it, music, 1);
    for (int i = 0; i < it->count; i++) {
        music_free(m[i]);
    }
}

void game_free() {
    data_world_query_free(render_query);
    data_world_free();
    dict_free(&phys_dict);
    phys_world_free();
}
