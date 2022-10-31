// ---------------------------------------------------------------------------
// Author: L. Schindler
// Version: 2.1
// Last modification date: 3 June 2021
// Last modification by: L. Schindler
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// Automatically extracted verilog file, created with TimEx v2.05
// Timing description and structural design for IARPA-BAA-14-03 via
// U.S. Air Force Research Laboratory contract FA8750-15-C-0203 and
// IARPA-BAA-16-03 via U.S. Army Research Office grant W911NF-17-1-0120.
// For questions about TimEx, contact CJ Fourie, coenrad@sun.ac.za
// (c) 2016-2020 Stellenbosch University
// ---------------------------------------------------------------------------
`ifndef begin_time
`define begin_time 8
`endif
`timescale 1ps/100fs

`celldefine
module LSmitll_NDROT_v2p1_optimized #(parameter begin_time = `begin_time) (a, b, clk, q);

// Define inputs
input
  a, b, clk;

// Define outputs
output
  q;

// Define internal output variables
reg
  internal_q;
assign q = internal_q;

// Define state
integer state;

wire
  internal_state_0, internal_state_1;

assign internal_state_0 = state === 0;
assign internal_state_1 = state === 1;

specify
  specparam delay_state1_clk_q = 9.0;

  specparam ct_state0_b_a = 5.0;
  specparam ct_state1_a_b = 2.2;
  specparam ct_state1_clk_clk = 10.1;

  if (internal_state_1) (clk => q) = delay_state1_clk_q;

  $hold( posedge b &&& internal_state_0, a, ct_state0_b_a);
  $hold( negedge b &&& internal_state_0, a, ct_state0_b_a);
  $hold( posedge a &&& internal_state_1, b, ct_state1_a_b);
  $hold( negedge a &&& internal_state_1, b, ct_state1_a_b);
  $hold( posedge clk &&& internal_state_1, clk, ct_state1_clk_clk);
  $hold( negedge clk &&& internal_state_1, clk, ct_state1_clk_clk);
endspecify

// Set initial state
initial begin
   state = 1'bX;
   internal_q = 0; // All outputs start at 0
   #begin_time state = 0;
   end

always @(posedge a or negedge a)
case (state)
   0: begin
      state = 1;
   end
   1: begin
   end
endcase
always @(posedge b or negedge b)
case (state)
   0: begin
   end
   1: begin
      state = 0;
   end
endcase
always @(posedge clk or negedge clk)
case (state)
   0: begin
   end
   1: begin
      internal_q = !internal_q;
   end
endcase

endmodule
`endcelldefine
