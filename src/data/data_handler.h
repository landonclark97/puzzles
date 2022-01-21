#ifndef __DATAHANDLER__
#define __DATAHANDLER__


#include <flecs.h>

#include <string.h>


#define DATA_THREADS (2)


typedef ecs_world_t data_world_t;

typedef u_int64_t data_entity_id_t;

typedef ecs_query_t data_query_t;

typedef ecs_iter_t data_iter_t;


data_world_t* data_world();

void data_world_init();

bool data_world_step(double dt);

#define data_world_query(sig) ecs_query_new(data_world(), sig)

#define data_world_query_order_by(q, comp, func) ecs_query_order_by(data_world(), q, ecs_entity(comp), func)

#define data_world_query_iter(q) ecs_query_iter(q)

#define data_world_query_next(it) ecs_query_next(it)

#define data_world_query_changed(q) ecs_query_changed(q)

#define data_world_query_free(q) ecs_query_free(q)

#define data_world_staging_begin() ecs_staging_begin(data_world())

#define data_world_staging_end() ecs_staging_end(data_world())

#define data_world_add_entity(...) ecs_new(data_world(), __VA_ARGS__)

#define data_world_add_comp(e, comp) ecs_add(data_world(), e, comp)

#define data_world_remove_entity(id) ecs_delete(data_world(), id)

#define data_world_component(comp) ECS_COMPONENT(data_world(), comp)

#define data_world_component_declare(comp) ECS_COMPONENT_DECLARE(comp)

#define data_world_component_extern(comp) ECS_COMPONENT_EXTERN(comp)

#define data_world_component_define(comp) ECS_COMPONENT_DEFINE(data_world(), comp)

#define data_world_type(type, ...)  ECS_TYPE(data_world(), type, __VA_ARGS__)

#define data_world_type_declare(type) ECS_TYPE_DECLARE(type)

#define data_world_type_extern(type) ECS_TYPE_EXTERN(type)

#define data_world_type_define(type, ...) ECS_TYPE_DEFINE(data_world(), type, __VA_ARGS__)

#define data_world_tag(tag) ECS_TAG(data_world(), tag)

#define data_world_tag_declare(tag) ECS_TAG_DECLARE(tag)

#define data_world_tag_extern(tag) ECS_TAG_EXTERN(tag)

#define data_world_tag_define(tag) ECS_TAG_DEFINE(data_world(), tag)

#define data_world_get(e, comp) ecs_get(data_world(), e, comp)

#define data_world_get_mut(e, comp, is_added) ecs_get_mut(data_world(), e, comp, is_added)

#define data_world_modified(e, comp) ecs_modified(data_world(), e, comp)

#define data_world_set(e, comp, ...) ecs_set(data_world(), e, comp, __VA_ARGS__)

#define data_world_set_ptr(e, comp, ptr) ecs_set_ptr(data_world(), e, comp, ptr)

#define data_world_check_tag(t) ecs_type_to_id(data_world(), ecs_type(t))

#define data_world_set_tag(e, tag) ecs_add_id(data_world(), e, tag)

#define data_world_get_term(it, t, index) ecs_term(it, t, index)

#define data_world_system(func, phase, ...) ECS_SYSTEM(data_world(), func, phase, __VA_ARGS__)

#define data_world_add_trigger(func, phase, comp) ECS_TRIGGER(data_world(), func, phase, comp)

void data_world_free();





#endif
