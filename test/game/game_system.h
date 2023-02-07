#ifndef __GAMESYSTEMS__
#define __GAMESYSTEMS__


#include "../../include/puzzles.h"


data_world_component_extern(position);
data_world_component_extern(hp);
data_world_component_extern(sp);
data_world_component_extern(mp);
data_world_component_extern(agility);
data_world_component_extern(strength);
data_world_component_extern(constitution);
data_world_component_extern(intelligence);
data_world_component_extern(wisdom);
data_world_component_extern(charisma);
data_world_component_extern(hitbox);
data_world_component_extern(control_body);
data_world_component_extern(move_constraint);
data_world_component_extern(physics_constructor);
data_world_component_extern(texture);
data_world_component_extern(sprite);
data_world_component_extern(hit);
data_world_component_extern(item_prop);
data_world_component_extern(tick);
data_world_component_extern(sound);
data_world_component_extern(music);

data_world_tag_extern(player);
data_world_tag_extern(npc);
data_world_tag_extern(enemy);
data_world_tag_extern(atk_tag);

data_world_tag_extern(standing);
data_world_tag_extern(walking);
data_world_tag_extern(running);
data_world_type_extern(movement);

data_world_tag_extern(up);
data_world_tag_extern(down);
data_world_tag_extern(left);
data_world_tag_extern(right);
data_world_type_extern(direction);

data_world_type_extern(agent_char);
data_world_type_extern(item);
data_world_type_extern(agent);
data_world_type_extern(environment);
data_world_type_extern(atk_prep);
data_world_type_extern(atk);
data_world_type_extern(atk_hit);


void game_system_init();

void game_step(double dt);

void game_update(double dt);

void game_set_player_id(data_entity_id_t id);

void game_free();


#endif
