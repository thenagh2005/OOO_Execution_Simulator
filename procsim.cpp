#include "procsim.hpp"

//Fetch method

void fetch(uint64_t fetch_rate, std::queue<proc_inst_t>& instructionQueue) {
    for (int i = 0; i < fetch_rate; i++) {
        proc_inst_t inst;
        if (read_instruction(&inst)) {
            instructionQueue.push(inst);
        } else {
            break;
        }
    }
}

//Dispatch method

void dispatch(std::queue<proc_inst_t>& instructionQueue, std::queue<dispatch_queue_entry>& dispatch_queue, int& next_tag, int fetch_rate) {
    int i = 0;
    while (!instructionQueue.empty() && i < fetch_rate) {
        proc_inst_t inst = instructionQueue.front();
        instructionQueue.pop();
        dispatch_queue.push(dispatch_queue_entry(inst, next_tag));
        next_tag++;
        i++;
    }
}

void schedule(std::queue<dispatch_queue_entry>& dispatch_queue, res_table& res_rob_hybrid) {
    //Implement scheduling logic here

    while (!dispatch_queue.empty() && !res_rob_hybrid.is_full()) {
        dispatch_queue_entry entry = dispatch_queue.front();
        dispatch_queue.pop();
        res_table_entry res_entry(entry.get_instruction(), entry.get_tag());
        res_rob_hybrid.add_entry(res_entry);
    }
}

void execute(res_table& res_rob_hybrid) {

}



/**
 * Subroutine for initializing the processor. You many add and initialize any global or heap
 * variables as needed.
 * XXX: You're responsible for completing this routine
 *
 * @r number of result busses
 * @k0 Number of k0 FUs
 * @k1 Number of k1 FUs
 * @k2 Number of k2 FUs
 * @f Number of instructions to fetch
 */
void setup_proc(uint64_t r, uint64_t k0, uint64_t k1, uint64_t k2, uint64_t f) 
{
    //Set up the OOO processor structures and variables here

    //Fetch rate
    int fetch_rate = f;

    //Initialize result busses

    //Set up result busses

    std::vector<result_bus> result_busses;

    for (uint64_t i = 0; i < r; i++) {
        result_busses.push_back(result_bus());
    }
    
    //Initialize functional units

    register_tracker reg_tracker;

    functional_unit_tracker fu_tracker(k0, k1, k2);

    /*

    for (uint64_t i = 0; i < k0; i++) {
        k0_fus.push_back(k0_functional_unit());
    }
    for (uint64_t i = 0; i < k1; i++) {
        k1_fus.push_back(k1_functional_unit());
    }
    for (uint64_t i = 0; i < k2; i++) {
        k2_fus.push_back(k2_functional_unit());
    }*/

    //initialize dispatch queue

    std::queue<dispatch_queue_entry> dispatch_queue;

    //Initialize res table/ROB

    //std::vector<res_table_entry> res_table;

    int res_table_size = 2*(k0 + k1 + k2);
    res_table res_rob_hybrid(res_table_size);

    //Everything is now set up
}

/**
 * Subroutine that simulates the processor.
 *   The processor should fetch instructions as appropriate, until all instructions have executed
 * XXX: You're responsible for completing this routine
 *
 * @p_stats Pointer to the statistics structure
 */
void run_proc(proc_stats_t* p_stats)
{
    int cycle_count = 0;

    //Begin processor simulation

    //First fetch instructions


}

/**
 * Subroutine for cleaning up any outstanding instructions and calculating overall statistics
 * such as average IPC, average fire rate etc.
 * XXX: You're responsible for completing this routine
 *
 * @p_stats Pointer to the statistics structure
 */
void complete_proc(proc_stats_t *p_stats) 
{

}
