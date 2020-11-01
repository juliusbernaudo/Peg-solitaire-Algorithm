#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "utils.h"

#define SIZE 9

void initialize_ai();

void copy_state(state_t* dst, state_t* src);
void save_solution( node_t* solution_node );
node_t* create_init_node( state_t* init_state );
node_t* applyAction(node_t* n, position_s* selected_peg, move_t action );

void find_solution( state_t* init_state );
void free_memory(node_t* node);

#endif
