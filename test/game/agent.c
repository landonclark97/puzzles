#include <stdlib.h>
#include <string.h>

#include "../../include/puzzles.h"

#include "./game_system.h"
#include "./agent.h"
#include "./attributes.h"
#include "./item.h"


void agent_attr_player(agent_attr_t* attr,
                       window_t* win,
                       char* tex_name,
                       float hp,
                       float agility,
                       float strength,
                       float constitution,
                       float intelligence,
                       float wisdom,
                       float charisma,
                       float anim_time,
                       int h_frames,
                       int v_frames) {
    attr->type = PLAYER;
    attr->hp = hp;
    attr->agility = agility;
    attr->strength = strength;
    attr->constitution = constitution;
    attr->intelligence = intelligence;
    attr->wisdom = wisdom;
    attr->charisma = charisma;
    attr->tex_name = tex_name;
    attr->win = win;
    attr->spr.anim_time = anim_time;
    attr->spr.h_frames = h_frames;
    attr->spr.v_frames = v_frames;
}

void agent_attr_npc(agent_attr_t* attr,
                    window_t* win,
                    char* tex_name,
                    float hp,
                    float agility,
                    float strength,
                    float constitution,
                    float intelligence,
                    float wisdom,
                    float charisma,
                    float anim_time,
                    int h_frames,
                    int v_frames) {
    attr->type = NPC;
    attr->hp = hp;
    attr->agility = agility;
    attr->strength = strength;
    attr->constitution = constitution;
    attr->intelligence = intelligence;
    attr->wisdom = wisdom;
    attr->charisma = charisma;
    attr->tex_name = tex_name;
    attr->win = win;
    attr->spr.anim_time = anim_time;
    attr->spr.h_frames = h_frames;
    attr->spr.v_frames = v_frames;
}

void agent_attr_enemy(agent_attr_t* attr,
                      window_t* win,
                      char* tex_name,
                      float hp,
                      float agility,
                      float strength,
                      float constitution,
                      float intelligence,
                      float wisdom,
                      float charisma,
                      float anim_time,
                      int h_frames,
                      int v_frames) {
    attr->type = ENEMY;
    attr->hp = hp;
    attr->agility = agility;
    attr->strength = strength;
    attr->constitution = constitution;
    attr->intelligence = intelligence;
    attr->wisdom = wisdom;
    attr->charisma = charisma;
    attr->tex_name = tex_name;
    attr->win = win;
    attr->spr.anim_time = anim_time;
    attr->spr.h_frames = h_frames;
    attr->spr.v_frames = v_frames;
}

agent_t agent_new(agent_attr_t* attr, phys_body_constructor_t* bc) {

    agent_t agent;

    if (attr->tex_name) {
        texture_t tex = texture_from_png(attr->win, attr->tex_name);
        data_entity_id_t t_id = data_world_add_entity(texture);
        data_world_set_ptr(t_id, texture, &tex);
        attr->tex = t_id;
    }

    agent.attr = *attr;
    agent.bc = *bc;

    return agent;
}

void agent_instantiate(agent_t* agent, phys_vec_t pos) {
    data_entity_id_t e_id;
    hitbox b;
    control_body cb;
    move_constraint c;
    sprite s;

    agent->bc.pos_x = pos.x;
    agent->bc.pos_y = pos.y;

    e_id = data_world_add_entity(agent);

    switch (agent->attr.type) {
        case PLAYER:
            data_world_add_comp(e_id, player);
            data_world_set_tag(e_id, ECS_CASE | down);
            game_set_player_id(e_id);
            b = phys_body_new(&agent->bc);
            data_world_set_ptr(e_id, hitbox, &b);
            cb = phys_control_body_new();
            phys_body_set_position(&cb, pos);
            data_world_set_ptr(e_id, control_body, &cb);
            c = phys_pivot_new(&cb, &b, pos);
            data_world_set_ptr(e_id, move_constraint, &c);
            s = sprite_new((texture_t*) data_world_get(agent->attr.tex, texture), agent->attr.spr.h_frames, agent->attr.spr.v_frames, agent->attr.spr.anim_time);
            data_world_set_ptr(e_id, sprite, &s);
            data_world_set(e_id, position, {pos.x, pos.y});
            data_world_set(e_id, hp, {agent->attr.hp});
            data_world_set(e_id, agility, {agent->attr.agility});
            data_world_set(e_id, strength, {agent->attr.strength});
            data_world_set(e_id, constitution, {agent->attr.constitution});
            data_world_set(e_id, intelligence, {agent->attr.intelligence});
            data_world_set(e_id, wisdom, {agent->attr.wisdom});
            data_world_set(e_id, charisma, {agent->attr.charisma});

            phys_body_constructor_t bc = agent->bc;
            bc.radius = agent->bc.radius * 10.0;
            bc.catg = PLAYER_ATK_FILTER;
            bc.mask = NPC_FILTER | ENEMY_FILTER;
            data_world_set_ptr(e_id, physics_constructor, &bc);
            item_prop itm = item_new(50.0, 1.0, 1.0, 1.0, 1.0, 0);
            data_world_set_ptr(e_id, item_prop, &itm);

            break;
        case NPC:
            data_world_add_comp(e_id, npc);
            data_world_set_tag(e_id, ECS_CASE | down);
            b = phys_body_new(&agent->bc);
            data_world_set_ptr(e_id, hitbox, &b);
            cb = phys_control_body_new();
            phys_body_set_position(&cb, pos);
            data_world_set_ptr(e_id, control_body, &cb);
            c = phys_pivot_new(&cb, &b, pos);
            data_world_set_ptr(e_id, move_constraint, &c);
            s = sprite_new((texture_t*) data_world_get(agent->attr.tex, texture), agent->attr.spr.h_frames, agent->attr.spr.v_frames, agent->attr.spr.anim_time);
            data_world_set_ptr(e_id, sprite, &s);
            data_world_set(e_id, position, {pos.x, pos.y});
            data_world_set(e_id, hp, {agent->attr.hp});
            data_world_set(e_id, agility, {agent->attr.agility});
            data_world_set(e_id, strength, {agent->attr.strength});
            data_world_set(e_id, constitution, {agent->attr.constitution});
            data_world_set(e_id, intelligence, {agent->attr.intelligence});
            data_world_set(e_id, wisdom, {agent->attr.wisdom});
            data_world_set(e_id, charisma, {agent->attr.charisma});
            break;
        case ENEMY:
            data_world_add_comp(e_id, enemy);
            data_world_set_tag(e_id, ECS_CASE | down);
            b = phys_body_new(&agent->bc);
            data_world_set_ptr(e_id, hitbox, &b);
            cb = phys_control_body_new();
            phys_body_set_position(&cb, pos);
            data_world_set_ptr(e_id, control_body, &cb);
            c = phys_pivot_new(&cb, &b, pos);
            data_world_set_ptr(e_id, move_constraint, &c);
            s = sprite_new((texture_t*) data_world_get(agent->attr.tex, texture), agent->attr.spr.h_frames, agent->attr.spr.v_frames, agent->attr.spr.anim_time);
            data_world_set_ptr(e_id, sprite, &s);
            data_world_set(e_id, position, {pos.x, pos.y});
            data_world_set(e_id, hp, {agent->attr.hp});
            data_world_set(e_id, agility, {agent->attr.agility});
            data_world_set(e_id, strength, {agent->attr.strength});
            data_world_set(e_id, constitution, {agent->attr.constitution});
            data_world_set(e_id, intelligence, {agent->attr.intelligence});
            data_world_set(e_id, wisdom, {agent->attr.wisdom});
            data_world_set(e_id, charisma, {agent->attr.charisma});
            break;
        case ENVRN:
            break;
        case BACKGND:
            break;
    }
}

void agent_free(agent_t* agent) {
    data_world_remove_entity(agent->attr.tex);
}
