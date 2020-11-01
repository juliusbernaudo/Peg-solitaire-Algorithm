#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "ai.h"
#include "utils.h"
#include "hashtable.h"
#include "stack.h"

void free_memory(node_t* node) {
    if (node != NULL) {
        free_memory(node->parent);
        free(node);
    }
}

void copy_state(state_t* dst, state_t* src){
	
	//Copy field
	memcpy( dst->field, src->field, SIZE*SIZE*sizeof(int8_t) );

	dst->cursor = src->cursor;
	dst->selected = src->selected;
}

/**
 * Saves the path up to the node as the best solution found so far
*/
void save_solution( node_t* solution_node ){
	node_t* n = solution_node;
	while( n->parent != NULL ){
		copy_state( &(solution[n->depth]), &(n->state) );
		solution_moves[n->depth-1] = n->move;

		n = n->parent;
	}
	solution_size = solution_node->depth;
}


node_t* create_init_node( state_t* init_state ){
	node_t * new_n = (node_t *) malloc(sizeof(node_t));
	new_n->parent = NULL;	
	new_n->depth = 0;
	copy_state(&(new_n->state), init_state);
	return new_n;
}

/**
 * Apply an action to node n and return a new node resulting from executing the action
*/
node_t* applyAction(node_t* n, position_s* selected_peg, move_t action ){

    node_t * new_node = create_init_node( &n->state );
    new_node->parent = n;
    new_node->state = n->state;
    new_node->state.cursor = *selected_peg;
    new_node->depth = n->depth + 1;
    new_node->move = action;
    
    
	//FILL IN MISSING CODE
	
    execute_move_t( &(new_node->state), &(new_node->state.cursor), action );
	
	return new_node;

}

/**
 * Find a solution path as per algorithm description in the handout
 */

void find_solution( state_t* init_state  ){
    
    // Creating action and a variable to keep up with the current peg
    move_t moveLeft = left;
    move_t moveRight = right;
    move_t moveUp = up;
    move_t moveDown = down;
    position_s currPeg;
    
	HashTable table;
    
	// Choose initial capacity of PRIME NUMBER 
	// Specify the size of the keys and values you want to store once 
	ht_setup( &table, sizeof(int8_t) * SIZE * SIZE, sizeof(int8_t) * SIZE * SIZE, 16769023);

	// Initialize Stack
	initialize_stack();

	//Add the initial node
	node_t* n = create_init_node( init_state );
	
	//FILL IN THE GRAPH ALGORITHM
    stack_push(n);
    int remainingPegs = num_pegs(&n->state);
    
    // Loop through the stack
    while (is_stack_empty) {
        n = stack_top();
        stack_pop();
        
        expanded_nodes++;
        
        // Saving the solution if the state contains less than the remaining pegs
        if (num_pegs(&n->state) < remainingPegs) {
            save_solution(n);
            remainingPegs = num_pegs(init_state);
        }
        
        // For loop to check every peg for valid jumps
        for (int i=0; i < SIZE; i++) {
            currPeg.x = i;
            
            for (int j=0; j < SIZE; j++) {
                currPeg.y = j;
                
                for (int k=0; k < 4; k++) {
                    
                    /* Checking whether a peg can perform an action and pushing it to the stack
                       and the hash table to check for duplicates */
                    if (can_apply(&n->state, &currPeg, moveLeft) && k==0) {
                        node_t* newNode = applyAction(n, &currPeg, moveLeft);
                        generated_nodes++;
                        
                        // Win condition which ends the program
                        if (won(&newNode->state)){
                            save_solution(newNode);
                            remainingPegs = num_pegs(&newNode->state);
                            ht_clear(&table);
                            ht_destroy(&table);
                            free_stack();
                            free_memory(n);
                            return;
                        }
                        ht_insert(&table, newNode->state.field, newNode->state.field);
                        
                        // Check if this is the first instance of the state
                        if (ht_contains(&table, newNode->state.field)) {
                            
                            stack_push(newNode);
                        } else {
                            free(newNode);
                        }
                    }
                    if (can_apply(&n->state, &currPeg, moveRight) && k==1) {
                        node_t* newNode = applyAction(n, &currPeg, moveRight);
                        generated_nodes++;
                        
                        if (won(&newNode->state)){
                            save_solution(newNode);
                            remainingPegs = num_pegs(&newNode->state);
                            ht_clear(&table);
                            ht_destroy(&table);
                            free_stack();
                            free_memory(n);
                            return;
                        }
                        ht_insert(&table, newNode->state.field, newNode->state.field);
                        
                        if (ht_contains(&table, newNode->state.field)) {
                            
                            stack_push(newNode);
                        } else {
                            free(newNode);
                        }
                    }
                    if (can_apply(&n->state, &currPeg, moveUp) && k==2) {
                        node_t* newNode = applyAction(n, &currPeg, moveUp);
                        generated_nodes++;
                        
                        if (won(&newNode->state)){
                            save_solution(newNode);
                            remainingPegs = num_pegs(&newNode->state);
                            ht_clear(&table);
                            ht_destroy(&table);
                            free_stack();
                            free_memory(n);
                            return;
                        }
                        ht_insert(&table, newNode->state.field, newNode->state.field);
                        
                        if (ht_contains(&table, newNode->state.field)) {
                            
                            stack_push(newNode);
                        } else {
                            free(newNode);
                        }
                    }
                    if (can_apply(&n->state, &currPeg, moveDown) && k==3) {
                        node_t* newNode = applyAction(n, &currPeg, moveDown);
                        generated_nodes++;
                        
                        if (won(&newNode->state)){
                            save_solution(newNode);
                            remainingPegs = num_pegs(&newNode->state);
                            ht_clear(&table);
                            ht_destroy(&table);
                            free_stack();
                            free_memory(n);
                            return;
                        }
                        ht_insert(&table, newNode->state.field, newNode->state.field);
                        
                        if (ht_contains(&table, newNode->state.field)) {
                            
                            stack_push(newNode);
                        } else {
                            free(newNode);
                        }
                    }
                }
            }
        }
        
        // If expanded nodes reaches the budget end algorithm.
        if (expanded_nodes >= budget) {
            ht_clear(&table);
            ht_destroy(&table);
            free_stack();
            free_memory(n);
            return;
        }
    }
}
