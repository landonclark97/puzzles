#ifndef __AGENT__
#define __AGENT__


#include "../../include/puzzles.h"

#include "./attributes.h"


typedef struct {
    int type;
    hp hp;
    agility agility;
    strength strength;
    constitution constitution;
    intelligence intelligence;
    wisdom wisdom;
    charisma charisma;
    data_entity_id_t tex;
    char* tex_name;
    window_t* win;
    struct {
        int h_frames, v_frames;
        float anim_time;
    } spr;
} agent_attr_t;

typedef struct {
    phys_body_constructor_t bc;
    agent_attr_t attr;
} agent_t;

void agent_attr_player(agent_attr_t* attr,
                       window_t* win,
                       char* tex_name,
                       float agility,
                       float hp,
                       float strength,
                       float constitution,
                       float intelligence,
                       float wisdom,
                       float charisma,
                       float anim_time,
                       int h_frames,
                       int v_frames);

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
                    int v_frames);

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
                      int v_frames);

agent_t agent_new(agent_attr_t* attr, phys_body_constructor_t* bc);

void agent_instantiate(agent_t* agent, phys_vec_t pos);

void agent_free(agent_t* agent);


#endif
