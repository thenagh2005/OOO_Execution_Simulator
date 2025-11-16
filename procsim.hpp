#ifndef PROCSIM_HPP
#define PROCSIM_HPP

#include <cstdint>
#include <cstdio>
#include <string>
#include <queue>
#include <vector>

#define DEFAULT_K0 1
#define DEFAULT_K1 2
#define DEFAULT_K2 3
#define DEFAULT_R 8
#define DEFAULT_F 4

class k0_functional_unit
{
    // Define k0 function unit properties and methods here

    public:
        k0_functional_unit()
        {
            available = true;
            // Constructor
        }

        ~k0_functional_unit()
        {
            // Destructor
        }

    private:
        bool available;
};

class k1_functional_unit
{
    public:
        k1_functional_unit()
        {
            available = true;
            // Constructor
        }

        ~k1_functional_unit()
        {
            // Destructor
        }
    private:
        bool available;
};

class k2_functional_unit
{
    public:
        k2_functional_unit()
        {
            available = true;
            // Constructor
        }

        ~k2_functional_unit()
        {
            // Destructor
        }
    private:
        bool available;
};



typedef struct _proc_inst_t
{
    uint32_t instruction_address;
    int32_t op_code;
    int32_t src_reg[2];
    int32_t dest_reg;

    // You may introduce other fields as needed

} proc_inst_t;

//Reservation table entry class definition

class res_table_entry {
    // Define resource table entry properties and methods here

    public:
        res_table_entry(proc_inst_t instruction, int tag)
            : instr(instruction), busy(true), complete(false), fired(false), tag(tag)
        {
            // Initialize other fields as needed
        }

        bool is_busy() const { return busy; }
        void set_busy(bool b) { busy = b; }

        bool is_complete() const { return complete; }
        void set_complete(bool c) { complete = c; }

        bool is_fired() const { return fired; }
        void set_fired(bool f) { fired = f; } 

        int get_tag() const { return tag; }
        void set_tag(int t) { tag = t; }

        bool is_src1_ready() const { return src1_ready; }
        bool is_src2_ready() const { return src2_ready; }

        void set_src1_ready(bool r) { src1_ready = r; }
        void set_src2_ready(bool r) { src2_ready = r; }


        proc_inst_t get_instruction() const { return instr; }
    private:
        bool busy;
        std::string op;

        proc_inst_t instr;
        
        bool src1_ready; //Is src 1 ready?
        bool src2_ready; //Is src 2 ready?
        int IMM; //What is the immediate
        int FU; //What FU is it going to use
        bool complete; //Is it complete?
        bool fired; //Has it been fired?
        int tag; //Tag to identify the instruction

};

//Class defining the reservation table/ROB

class res_table {
    public:
        res_table(int size) : max_size(size) {
            
        }

        bool is_full() const {return entries.size() >= max_size;}

        void add_entry(const res_table_entry& entry) {
            if (!is_full()) {
                entries.push_back(entry);
            }
        }

        void remove_entry(int index) {
            if (index >= 0 && index < entries.size()) {
                entries.erase(entries.begin() + index);
            }
        }

        void fire_ready_instructions() {
            for (auto& entry : entries) {
                if (entry.is_src1_ready() && entry.is_src2_ready() && !entry.is_fired()) {
                    entry.set_fired(true);
                }
            }
        }

        size_t size() const { return entries.size();}
        std::vector<res_table_entry>& get_entries() { return entries;}
    private:
        std::vector<res_table_entry> entries;
        int max_size;
};

//Class for dispatch queue entries

class dispatch_queue_entry {
    public:
        dispatch_queue_entry(proc_inst_t instruction, int t) : instr(instruction), tag(t) {}
        proc_inst_t get_instruction() const { return instr; }
        int get_tag() const { return tag; }

    private:
    proc_inst_t instr;
    int tag;

};

//Result bus class
class result_bus {
    public:
        result_bus() {
            busy = false;
        }
        bool is_busy() const { return busy; } //Check if the bus is busy
        void set_busy(bool b) { busy = b; } //Set the bus busy status

    private:
        bool busy; //Is it busy?
};

class register_tracker {
    public:
        register_tracker() {
            for (int i = 0; i < 128; i++) {
                ready[i] = true;
            }
        }

        bool is_ready(int reg) const {
            if (reg == -1) return true;  // -1 means no register
            return ready[reg];
        }

        void set_ready(int reg, bool r) {
            if (reg == -1) return;
            ready[reg] = r;
        }
    private:
        bool ready[128]; //Assuming 128 registers. Overestimate because why not? :P
        
};

//Class to keep track of functional units

class functional_unit_tracker {
    public:
        functional_unit_tracker(int num_k0s, int num_k1s, int num_k2s)
            : num_of_k0s(num_k0s), num_of_k1s(num_k1s), num_of_k2s(num_k2s) {
            k0_tracker.resize(num_k0s, true);
            k0_fus.resize(num_k0s);

            k1_tracker.resize(num_k1s, true);
            k1_fus.resize(num_k1s);

            k2_tracker.resize(num_k2s, true);
            k2_fus.resize(num_k2s);
        }

        //Utility functions to check and set FU status

        bool is_k0_busy(int index) const { return k0_tracker[index]; }
        bool is_k1_busy(int index) const { return k1_tracker[index]; }
        bool is_k2_busy(int index) const { return k2_tracker[index]; }
        void set_k0_status(int index, bool busy) { k0_tracker[index] = busy; }
        void set_k1_status(int index, bool busy) { k1_tracker[index] = busy; }
        void set_k2_status(int index, bool busy) { k2_tracker[index] = busy; }

        int first_free_k0() const {
            for (int i = 0; i < num_of_k0s; i++) {
                if (k0_tracker[i]) {
                    return i;
                }
            }
            return -1; //No free FU
        }

        int first_free_k1() const {
            for (int i = 0; i < num_of_k1s; i++) {
                if (k1_tracker[i]) {
                    return i;
                }
            }
            return -1; //No free FU
        }

        int first_free_k2() const {
            for (int i = 0; i < num_of_k2s; i++) {
                if (k2_tracker[i]) {
                    return i;
                }
            }
            return -1; //No free FU
        }

    private:
        std::vector<bool> k0_tracker; 
        std::vector<bool> k1_tracker; 
        std::vector<bool> k2_tracker; 

        //Maybe not necessary??
        std::vector<k0_functional_unit> k0_fus;
        std::vector<k1_functional_unit> k1_fus;
        std::vector<k2_functional_unit> k2_fus;

        int num_of_k0s;
        int num_of_k1s;
        int num_of_k2s;
};

class result_bus_tracker {
    public:
        result_bus_tracker(int num_buses) : bus_tracker(num_buses, true) {}

        int first_free_bus() const {
            for (int i = 0; i < bus_tracker.size(); i++) {
                if (bus_tracker[i]) {
                    return i;
                }
            }
            return -1; //No free bus
        }

        void set_bus_status(int index, bool busy) {
            bus_tracker[index] = busy;
        }

    private:
        std::vector<bool> bus_tracker;
};

typedef struct _proc_stats_t
{
    float avg_inst_retired;
    float avg_inst_fired;
    float avg_disp_size;
    unsigned long max_disp_size;
    unsigned long retired_instruction;
    unsigned long cycle_count;
} proc_stats_t;

bool read_instruction(proc_inst_t *p_inst);

void setup_proc(uint64_t r, uint64_t k0, uint64_t k1, uint64_t k2, uint64_t f);
void run_proc(proc_stats_t *p_stats);
void complete_proc(proc_stats_t *p_stats);

void fetch(uint64_t fetch_rate, std::queue<proc_inst_t>& instructionQueue);

#endif /* PROCSIM_HPP */
