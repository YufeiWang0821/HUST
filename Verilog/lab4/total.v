`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/16 21:28:53
// Design Name: 
// Module Name: total
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module total(start,rst,clk,done,sumout);
    input start,clk,rst;
    output done;
    output [31:0]sumout;
    wire next_zero,ld_sum,ld_next,sum_sel,next_sel,a_sel;
    fsm f(clk,rst,start,next_zero,ld_sum,ld_next,sum_sel,next_sel,a_sel,done);
    datapath_top d(clk,rst,ld_sum,sum_sel,next_sel,ld_next,next_zero,a_sel,sumout);
endmodule