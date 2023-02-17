`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/16 18:54:32
// Design Name: 
// Module Name: full_adder
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


module full_adder(a, b, c, s); 
parameter WIDTH = 32;//本module内有效，可用于参数传递 
input [WIDTH-1:0] a, b; 
output [WIDTH-1:0] s; 
output [WIDTH-1:0] c; 
assign { c, s } = a + b;
endmodule
