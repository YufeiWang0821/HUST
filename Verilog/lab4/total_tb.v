`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/23 20:15:01
// Design Name: 
// Module Name: total_tb
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


module total_tb();
    reg clk,rst,start;
    wire done;
    wire next_zero,ld_sum,ld_next,sum_sel,next_sel,a_sel;
    wire [31:0]sumout;
    //module fsm(clk,rst,start,next_zero,ld_sum,ld_next,sum_sel,next_sel,a_sel,done
    fsm f(clk,rst,start,next_zero,ld_sum,ld_next,sum_sel,next_sel,a_sel,done);
    //module datapath_top(clk,rst,ld_sum,sum_sel,next_sel,ld_next,next_zero,a_sel,sumout);
    datapath_top d(clk,rst,ld_sum,sum_sel,next_sel,ld_next,next_zero,a_sel,sumout);
    initial begin
        clk<=0;
        rst<=0;
        start<=0;
        #50 start<=1;
    end
    always begin
        #20 clk<=~clk;
    end
endmodule