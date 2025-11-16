#include "procsim.hpp"

// Fetch method

// GLOBALS

int fetch_rate;

int cycleCount = 0;

int next_tag = 0;

bool trace_done = false;

// Initialize result busses

// Set up result busses

// Initialize functional units

register_tracker *reg_tracker;

functional_unit_tracker *fu_tracker;

// initialize dispatch queue

std::queue<dispatch_queue_entry> dispatch_queue;

std::queue<proc_inst_t> instructionQueue;

// Initialize res table/ROB

// std::vector<res_table_entry> res_table;

res_table *res_rob_hybrid;

result_bus_tracker *res_bus_tracker;

void fetch(uint64_t fetch_rate, std::queue<proc_inst_t> &instructionQueue)
{
    for (int i = 0; i < fetch_rate; i++)
    {
        proc_inst_t inst;
        if (read_instruction(&inst))
        {
            printf("Fetched at cycle %d: addr=0x%X, opcode=%d, dest=%d, src0=%d, src1=%d\n",
                   cycleCount,
                   inst.instruction_address,
                   inst.op_code,
                   inst.dest_reg,
                   inst.src_reg[0],
                   inst.src_reg[1]);
            instructionQueue.push(inst); //Add instruction to the instruction queue
        }
        else
        {
            trace_done = true;
            break;
        }
    }

    printf("Done fetching instructions this cycle.\n");
}

// Dispatch method

void dispatch(std::queue<proc_inst_t> &instructionQueue, std::queue<dispatch_queue_entry> &dispatch_queue, int &next_tag, int fetch_rate)
{
    int i = 0;
    while (!instructionQueue.empty() && i < fetch_rate)
    {
        proc_inst_t inst = instructionQueue.front();
        instructionQueue.pop();
        dispatch_queue.push(dispatch_queue_entry(inst, next_tag));
        printf("Dispatched at cycle %d: addr=0x%X, opcode=%d, dest=%d, src0=%d, src1=%d, tag=%d\n",
               cycleCount,
               inst.instruction_address,
               inst.op_code,
               inst.dest_reg,
               inst.src_reg[0],
               inst.src_reg[1],
               next_tag);
        next_tag++;
        i++;
    }
}

void schedule(std::queue<dispatch_queue_entry> &dispatch_queue, res_table &res_rob_hybrid)
{
    // Implement scheduling logic here

    while (!dispatch_queue.empty() && !res_rob_hybrid.is_full())
    {
        dispatch_queue_entry entry = dispatch_queue.front();
        dispatch_queue.pop();
        res_table_entry res_entry(entry.get_instruction(), entry.get_tag());
        res_rob_hybrid.add_entry(res_entry);
        printf("Scheduled at cycle %d: addr=0x%X, opcode=%d, dest=%d, src0=%d, src1=%d, tag=%d\n",
               cycleCount,
               entry.get_instruction().instruction_address,
               entry.get_instruction().op_code,
               entry.get_instruction().dest_reg,
               entry.get_instruction().src_reg[0],
               entry.get_instruction().src_reg[1],
               entry.get_tag());
    }
}

void execute(res_table &res_rob_hybrid)
{
    //Implement execute logic here. 

    // Get instructions from the res table/ROB hybrid to execute

    
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
    // Set up the OOO processor structures and variables here

    // Fetch rate
    fetch_rate = f;

    next_tag = 0;

    int res_table_size = 2 * (k0 + k1 + k2); // res table size

    reg_tracker = new register_tracker(); // Create register tracker

    fu_tracker = new functional_unit_tracker(k0, k1, k2); // Create functional unit tracker

    res_rob_hybrid = new res_table(res_table_size); // Create reservation table / ROB hybrid

    res_bus_tracker = new result_bus_tracker(r); // Create result bus tracker
}

/**
 * Subroutine that simulates the processor.
 *   The processor should fetch instructions as appropriate, until all instructions have executed
 * XXX: You're responsible for completing this routine
 *
 * @p_stats Pointer to the statistics structure
 */
void run_proc(proc_stats_t *p_stats)
{
    //int cycle_count = 0;

    // Begin processor simulation

    // First fetch instructions

    while (true)
    {
        
        schedule(dispatch_queue, *res_rob_hybrid);
        dispatch(instructionQueue, dispatch_queue, next_tag, fetch_rate);
        fetch(fetch_rate, instructionQueue);


        if (trace_done)
        {
            break;
        }

        cycleCount++; //Go to next cycle
    }
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
    delete reg_tracker;
    delete fu_tracker;
    delete res_rob_hybrid;
    delete res_bus_tracker;

    reg_tracker = nullptr;
    fu_tracker = nullptr;
    res_rob_hybrid = nullptr;
    res_bus_tracker = nullptr;
}
