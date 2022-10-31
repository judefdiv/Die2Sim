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
// (c) 2016-2018 Stellenbosch University
// ---------------------------------------------------------------------------
`timescale 1ps/100fs
module LSmitll_MERGE_v2p1_optimized (a, b, q);

input
  a, b;

output
  q;

reg
  q;

real
  delay_state0_a_q = 10.7,
  delay_state0_b_q = 10.7,
  ct_state0_a_a = 8.4,
  ct_state0_a_b = 3.0,
  ct_state0_b_a = 3.0,
  ct_state0_b_b = 8.9;

reg
   errorsignal_a,
   errorsignal_b;

integer
   outfile,
   cell_state; // internal state of the cell

initial
   begin
      errorsignal_a = 0;
      errorsignal_b = 0;
      cell_state = 0; // Startup state
      q = 0; // All outputs start at 0
   end

always @(posedge a or negedge a) // execute at positive and negative edges of input
   begin
      if ($time>4) // arbitrary steady-state time)
         begin
            if (errorsignal_a == 1'b1)  // A critical timing is active for this input
               begin
                  outfile = $fopen("errors.txt", "a");
                  $fdisplay(outfile, "Violation of critical timing in module %m; %0d ps.\n", $stime);
                  $fclose(outfile);
                  q <= 1'bX;  // Set all outputs to unknown
               end
            if (errorsignal_a == 0)
               begin
                  case (cell_state)
                     0: begin
                           q <= #(delay_state0_a_q) !q;
                           errorsignal_a = 1;  // Critical timing on this input; assign immediately
                           errorsignal_a <= #(ct_state0_a_a) 0;  // Clear error signal after critical timing expires
                           errorsignal_b = 1;  // Critical timing on this input; assign immediately
                           errorsignal_b <= #(ct_state0_a_b) 0;  // Clear error signal after critical timing expires
                        end
                  endcase
               end
         end
   end

always @(posedge b or negedge b) // execute at positive and negative edges of input
   begin
      if ($time>4) // arbitrary steady-state time)
         begin
            if (errorsignal_b == 1'b1)  // A critical timing is active for this input
               begin
                  outfile = $fopen("errors.txt", "a");
                  $fdisplay(outfile, "Violation of critical timing in module %m; %0d ps.\n", $stime);
                  $fclose(outfile);
                  q <= 1'bX;  // Set all outputs to unknown
               end
            if (errorsignal_b == 0)
               begin
                  case (cell_state)
                     0: begin
                           q <= #(delay_state0_b_q) !q;
                           errorsignal_a = 1;  // Critical timing on this input; assign immediately
                           errorsignal_a <= #(ct_state0_b_a) 0;  // Clear error signal after critical timing expires
                           errorsignal_b = 1;  // Critical timing on this input; assign immediately
                           errorsignal_b <= #(ct_state0_b_b) 0;  // Clear error signal after critical timing expires
                        end
                  endcase
               end
         end
   end

endmodule
