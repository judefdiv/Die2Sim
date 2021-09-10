`include "modules/LSmitll_MyCell2.v"
`include "modules/LSmitll_MyCell1.v"
`timescale 1ps/100fs
module HalfAdder (
	a,
	b,
	clk,
	s,
	cout
);
input
	a,b,clk;

output
	s,cout;

reg  [10:0] net_r = 0;
wire [10:0] net_w;
assign net_w[0] = a;
assign net_w[1] = b;
assign net_w[2] = clk;
assign s = net_r[3];
assign cout = net_r[4];
always @(posedge net_w[0] or negedge net_w[0]) begin
	#0 net_r[0] <= net_w[0];
end
always @(posedge net_w[1] or negedge net_w[1]) begin
	#2 net_r[1] <= net_w[1];
end
always @(posedge net_w[2] or negedge net_w[2]) begin
	#4 net_r[2] <= net_w[2];
end
always @(posedge net_w[3] or negedge net_w[3]) begin
	#6 net_r[3] <= net_w[3];
end
always @(posedge net_w[4] or negedge net_w[4]) begin
	#8 net_r[4] <= net_w[4];
end
always @(posedge net_w[5] or negedge net_w[5]) begin
	#10 net_r[5] <= net_w[5];
end
always @(posedge net_w[6] or negedge net_w[6]) begin
	#12 net_r[6] <= net_w[6];
end
always @(posedge net_w[7] or negedge net_w[7]) begin
	#14 net_r[7] <= net_w[7];
end
always @(posedge net_w[8] or negedge net_w[8]) begin
	#16 net_r[8] <= net_w[8];
end
always @(posedge net_w[9] or negedge net_w[9]) begin
	#18 net_r[9] <= net_w[9];
end
MyCell1 0 (net_r[0], net_r[1], net_w[2]);
MyCell2 1 (net_r[3], net_r[4], net_r[5], net_w[6]);
MyCell1 2 (net_r[7], net_r[8], net_w[9]);
MyCell2 3 (net_r[10], net_r[11], net_r[12], net_w[13]);
MyCell1 4 (net_r[14], net_r[15], net_w[16]);
MyCell2 5 (net_r[17], net_r[18], net_r[19], net_w[20]);
MyCell1 6 (net_r[21], net_r[22], net_w[23]);
MyCell2 7 (net_r[24], net_r[25], net_r[26], net_w[27]);
MyCell1 8 (net_r[28], net_r[29], net_w[30]);
MyCell2 9 (net_r[31], net_r[32], net_r[33], net_w[34]);
endmodule