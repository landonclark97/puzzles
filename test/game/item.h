#ifndef __ITEM__
#define __ITEM__


#include "../../include/puzzles.h"

#include "./attributes.h"


item_prop item_new(float sharp, float weight, float density, float length, float glamor, int wearable);

float item_calc_damage(item_prop* i);



#endif
