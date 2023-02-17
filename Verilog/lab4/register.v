`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/16 19:13:16
// Design Name: 
// Module Name: register
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


module register(clk, rst_n, en, d, q);
 parameter WIDTH = 32;
 input clk, rst_n, en;
 input [WIDTH-1:0] d;
 output reg [WIDTH-1:0] q;
 initial begin
    q<=0;
 end
 always @(posedge clk) begin
 if (rst_n) q <=0;
 else if (en) q <= d;
 end
endmodule
