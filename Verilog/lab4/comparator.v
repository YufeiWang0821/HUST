`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/16 18:57:56
// Design Name: 
// Module Name: comparator
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


module comparator(A, B, is_equal, is_great, is_less); 
parameter WIDTH = 32; 
input [WIDTH-1:0] A, B; 
output is_equal, is_great, is_less; 
assign is_equal = (A==B)? 1'b1 : 1'b0; 
assign is_great = (A>B)? 1'b1 : 1'b0; 
assign is_less = (A<B)? 1'b1 : 1'b0; 
endmodule
