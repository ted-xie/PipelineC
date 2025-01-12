// Full part string examples:
// xc7a35ticsg324-1l     Artix 7 (Arty)
// xcvu9p-flgb2104-2-i   Virtex Ultrascale Plus (AWS F1)
// xcvu33p-fsvh2104-2L-e Virtex Ultrascale Plus
// xc7v2000tfhg1761-2    Virtex 7
// EP2AGX45CU17I3        Arria II GX
// 10CL120ZF780I8G       Cyclone 10 LP
// 5CGXFC9E7F35C8        Cyclone 5 GX
// 10M50SCE144I7G        Max 10
// LFE5U-85F-6BG381C     ECP5U
// LFE5UM5G-85F-8BG756C  ECP5UM5G
// ICE40UP5K-SG48        ICE40UP
#pragma PART "xc7a35ticsg324-1l"

// Most recent (and more likely working) examples towards the bottom of list \/
//#include "examples/aws-fpga-dma/loopback.c"
//#include "examples/aws-fpga-dma/work.c"
//#include "examples/fosix/hello_world.c"
//#include "examples/fosix/bram_loopback.c"
//#include "examples/arty/src/uart/uart_loopback_no_fc.c"
//#include "examples/arty/src/work/work.c"
//#include "examples/arty/src/uart_ddr3_loopback/app.c"
//#include "examples/arty/src/ddr3/mig_app.c"
//#include "examples/arty/src/eth/loopback_app.c"
//#include "examples/arty/src/mnist/neural_network_fsm.c"
//#include "examples/arty/src/uart/uart_loopback_msg.c"
//#include "examples/arty/src/blink.c"
//#include "examples/async_clock_crossing.c"
//#include "examples/clock_crossing.c"
//#include "examples/littleriscy/riscv.c"
//#include "examples/arty/src/eth/work_app.c"
//#include "examples/blink.c"
//#include "examples/NexusProofOfWork/NXSTest_syn.c"
//#include "examples/fir.c"
//#include "examples/arty/src/i2s/i2s_passthrough_app.c"
//#include "examples/arty/src/audio/distortion.c"
//#include "examples/arty/src/i2s/i2s_app.c"
//#include "examples/pipeline_and_fsm.c"
//#include "examples/fosix/main_bram_loopback.c"
//#include "examples/aes/aes.c"
//#include "examples/groestl/groestl.c"
#include "examples/fosix/main_game.c"

// Below is a bunch of recent scratch work - enjoy

/*
#pragma MAIN_MHZ main 350.0
#include "uintN_t.h"
uint64_t main(uint64_t x, uint64_t y)
{
  return x + y;
}
*/

/*
#pragma MAIN_MHZ array_test 100.0
#pragma PART "xc7a35ticsg324-1l"
#include "uintN_t.h"
uint8_t main(uint8_t i)
{
  __clk++;
  return i;
}

/*
// What is RETURN? is it a valid/done signal? built in for FSMs
// ...yeah?
// Are inputs fixed/registered?
// Need built in valid and output signals used by tool
uint8_t main(uint8_t i)
{
  __clk++;
  return i;
}
*/
/*
// Every expression is a sub fsm of 0 or more cycles?
// Means adding start and done signals to logic from c code?
// All local variables could be regs if used across __clk++'s ?
// Inputs sampled/read in first stage, can be reread in later clocks for new data
uint8_t main(i)
{
  f = foo(i);
  __clk++;
  b = bar(f, i); // i value a clock later
  return b;
}

typedef enum main_STATE_t{
 // Number of states depends on by number of clk++ in code?
 // while loops are sub state machines
 // functions are sub state machines
 CLK0, // Entry point? Comb logic from inputs into through CLK0 logic
 CLK1,
}main_STATE_t;
uint8_t main(i)
{
  static main_STATE_t main_STATE;
  uint8_t RETURN_OUTPUT = 0;
  // All local vars are regs
  static f, b;
  
  if(main_STATE == CLK0)
  {
    f = foo(i);
    // HOW TO KNOW WHEN FOO FSM IS DONE?
    // RETURN becomes done signal/regs? inputs too?
    // What is return val here? default zeros?
  }
  else if(main_STATE == CLK1)
  {
    b = bar(f, i); // i value a clock later
    RETURN_OUTPUT = b;
  }
  
  return RETURN_OUTPUT;
}


uint8_t main(i)
{
  while(i)
  {
    f = foo(i);
    __clk++;
    b = bar(f, i); // i value a clock later
    return b;
  }
}




// Compiler sees this as a Silice function
// A big derived fsm
uint8_t main()
{
  uint8_t led;
  // a 28 bits unsigned integer register
  uint28_t counter = 0;
  
  // How does Silice derive?
  // "the loop takes exactly one cycle to execute:
  // we have one increment per cycle at 50 MHz the clock frequency"  
  while (1) {              // forever
    // LEDs updated every clock with the 8 most significant bits
    led = uint28_27_20(counter);
    counter = counter + 1; // increment counter
    // If its assumed comb logic unless clocks are inserted
    // Ex. :=  assign right to left at each rising clock
    //     ++: wait on clock
    // How to do that in PipelineC?
    __clk++; // Or something?
  }
  return led;
}

/*
// How to un-re-roll differently in time? Aetherling style
// N ratio faster and slower clocks
// Like double pumping dsps?
// 2x faster clock = as low as half resources(iter resource sharing) used iteratively 
//                   OR double throughput
// 2x slower clock = 2x resources 
//                   OR half throughput
uint8_t accum;
uint8_t main(uint1_t en)
{
  if(en) accum += 1;
  return accum;
}
*/


/*
uint8_t the_ram[128];
 main()
{
  
  // A variable latency globally visible memory access
  
  // Async style send and wait two func and fsm?
  
  // IMplemented as pipeline c func replacing this?  
  the_ram[i] = x
  // How to put a state machine into a user design?
    
}
*/


/*
// Tag main func as being compiled as C code and run on RISCV cpu
// What does interaction with PipelineC main func look like?
#pragma MAIN_MHZ cpu_main 100.0
#pragma MAIN_THREAD cpu_main // Run this as a CPU style thread
void cpu_main()
{
  // CPU style control loop
  while(1)
  {
    // Running around sequentially reading and writing memory
    // / Memory mapped registers to interact with hardware
    // The key being in hardware the registers are at 100MHz as specified
    mem[i] = foo
  
  }
  
}

#pragma MAIN_MHZ fpga_main 100.0
void fpga_main()
{
  // Every ten nanoseconds a value from
  // the RISCV CPU registers can be read/write ~like a variable somehow?
  
  
}
*/
