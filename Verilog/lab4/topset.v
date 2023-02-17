`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/30 19:12:49
// Design Name: 
// Module Name: topset
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


module topset(clk,rst,start,done,SEG,AN);
    input clk,rst,start;
    output done;
    output [7:0]SEG;
    output [7:0]AN;
    wire [31:0]sumout;
    wire clk_N1, clk_N2;
    divider #(10000000)d1(.clk(clk), .clk_N(clk_N1));
    divider #(100000)d2(.clk(clk), .clk_N(clk_N2));
    numdis dis(.clk(clk_N2), .sum_out(sumout), .AN(AN), .SEG(SEG));
    total t1(start,rst,clk_N1,done,sumout);
endmodule
