`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/16 19:31:02
// Design Name: 
// Module Name: datapath_top
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


module datapath_top(clk,rst,ld_sum,sum_sel,next_sel,ld_next,next_zero,a_sel,sumout);
    input clk,rst,ld_sum,sum_sel,next_sel,ld_next,a_sel;
    output next_zero;
    output [31:0]sumout;
    wire [31:0]data,sumin,sumsel1,nextin,nextout,asel1,aselout,temout;
    assign sumout=temout;
    full_adder a1(.a(temout),.b(data),.s(sumsel1));
    full_adder a2(.a(1),.b(nextout),.s(asel1));
    //module register(clk, rst_n, en, d, q);
    register r1(clk,rst,ld_sum,sumin,temout);
    register r2(clk,rst,ld_next,nextin,nextout);
    //module mux2_1(out1, a, b, sel) ;
    mux2_1 m1(sumin,sumsel1,0,sum_sel);
    mux2_1 m2(nextin,data,0,next_sel);
    mux2_1 m3(aselout,asel1,nextout,a_sel);
    comparator c(.A(nextin),.B(0),.is_equal(next_zero));
    dual_port_ram d(data,aselout);
endmodule