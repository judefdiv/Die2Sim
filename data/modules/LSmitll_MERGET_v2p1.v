// ---------------------------------------------------------------------------
// Author: L. Schindler
// Version: 2.1
// Last modification date: 8 June 2021
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
module LSmitll_MERGET_v2p1_optimized #(parameter begin_time = `begin_time) (a, b, q);

// Define inputs
input
  a, b;

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
  internal_state_0;

assign internal_state_0 = state === 0;

specify
  specparam delay_state0_a_q = 8.0;
  specparam delay_state0_b_q = 8.0;

  specparam ct_state0_a_a = 3.3;
  specparam ct_state0_a_b = 3.2;
  specparam ct_state0_b_a = 2.3;
  specparam ct_state0_b_b = 3.3;

  if (internal_state_0) (a => q) = delay_state0_a_q;
  if (internal_state_0) (b => q) = delay_state0_b_q;

  $hold( posedge a &&& internal_state_0, a, ct_state0_a_a);
  $hold( negedge a &&& internal_state_0, a, ct_state0_a_a);
  $hold( posedge a &&& internal_state_0, b, ct_state0_a_b);
  $hold( negedge a &&& internal_state_0, b, ct_state0_a_b);
  $hold( posedge b &&& internal_state_0, a, ct_state0_b_a);
  $hold( negedge b &&& internal_state_0, a, ct_state0_b_a);
  $hold( posedge b &&& internal_state_0, b, ct_state0_b_b);
  $hold( negedge b &&& internal_state_0, b, ct_state0_b_b);
endspecify

initial begin
   state = 1'bX;
   internal_q = 0; // All outputs start at 0
   #begin_time state = 0;
   end

always @(posedge a or negedge a)
case (state)
   0: begin
      internal_q = !internal_q;
   end
endcase
always @(posedge b or negedge b)
case (state)
   0: begin
      internal_q = !internal_q;
   end
endcase

endmodule
`endcelldefine
