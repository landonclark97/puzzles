#include "./game_system.h"
#include "./attributes.h"
#include "./item.h"


item_prop item_new(float sharp, float weight, float density, float length, float glamor, int wearable) {
    item_prop i;
    i.sharpness = sharp;
    i.weight = weight;
    i.density = density;
    i.length = length;
    i.glamor = glamor;
    i.wearable = wearable;
    return i;
}

float item_calc_damage(item_prop* i) {
    return i->sharpness/(i->length*i->density*i->weight);
}
