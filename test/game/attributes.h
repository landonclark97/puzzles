#ifndef __ATTRIBUTES__
#define __ATTRIBUTES__


#include "../../include/puzzles.h"


enum {
PLAYER,
NPC,
ENEMY,

ENVRN,
BACKGND,

LAST
};


#define PLAYER_FILTER (0x1)
#define NPC_FILTER (0x2)
#define ENEMY_FILTER (0x4)
#define ENVRN_FILTER (0x8)
#define PLAYER_ATK_FILTER (0x10)
#define NPC_ATK_FILTER (0x20)
#define ENEMY_ATK_FILTER (0x40)


typedef int dummy;
typedef float hp, sp, mp, agility, strength, constitution, intelligence, wisdom, charisma;
typedef phys_vec_t position;
typedef struct {
    float goal_time;
    float acc_time;
} tick;
typedef phys_body_t hitbox;
typedef phys_body_t control_body;
typedef phys_constraint_t* move_constraint;
typedef phys_body_constructor_t physics_constructor;
typedef texture_t texture;
typedef sprite_t sprite;
typedef data_entity_id_t hit;
typedef struct {
    float sharpness, weight, density, length, width;
    float glamor;
    int wearable;
} item_prop;
typedef sound_t* sound;
typedef music_t* music;



#endif
