#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "type.h"
#include <stdint.h> 

extern Update *scatter(Vertex *v, Edge *e);
extern void gather(Vertex *v, Update *u);

static bool init(unsigned char* vertex_state, unsigned long vertex_index, unsigned long bsp_phase, per_processor_data *cpu_state)
      {
	bool should_start = false;
	struct vertex* sssp_vertex = (struct vertex*)vertex_state;
	sssp_vertex->predecessor = (vertex_t)-1;
	sssp_vertex->distance = (weight_t)std::numeric_limits<double>::max();
	sssp_vertex->active_phase = -1UL;
	if (vertex_index == sssp_source) {
	  should_start = true;
	  sssp_vertex->distance = 0;
	  sssp_vertex->active_phase = 0;
	}
	return should_start;
      }

      static bool need_init(unsigned long bsp_phase)
      {
	return (bsp_phase == 0);
      }

      static bool apply_one_update(unsigned char* vertex_state,
				   unsigned char* update_stream,
				   per_processor_data *per_cpu_data,
				   unsigned long bsp_phase)
      {
	struct update* u = (struct update*)update_stream;
	struct vertex* vertices = (struct vertex*)vertex_state;
	struct vertex* v = &vertices[x_lib::configuration::map_offset(u->target)];
	if (u->distance < v->distance) {
	  v->predecessor = u->predecessor;
	  v->distance = u->distance;
	  v->active_phase = bsp_phase;
	  return true;
	} else {
	  return false;
	}
      }

      static bool generate_update(unsigned char* vertex_state,
				  unsigned char* edge_format,
				  unsigned char* update_stream,
				  per_processor_data *per_cpu_data,
				  unsigned long bsp_phase)
      {
	vertex_t src, dst;
	weight_t edge_distance;
	F::read_edge(edge_format, src, dst, edge_distance);

	struct vertex* vertices = (struct vertex*)vertex_state;
	struct vertex* v = &vertices[x_lib::configuration::map_offset(src)];
	if (v->active_phase == bsp_phase) {
	  struct update* u = (struct update*)update_stream;
	  u->target = dst;
	  u->predecessor = src;
	  u->distance = v->distance + edge_distance;
	  return true;
	} else {
	  return false;
	}
      }

#endif