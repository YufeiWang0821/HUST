`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/16 19:19:03
// Design Name: 
// Module Name: dual_port_ram
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


module dual_port_ram(data, addr);
parameter WIDTH=32;
input [WIDTH-1:0]addr;
output [WIDTH-1:0]data;
reg [WIDTH-1:0]ram[16-1:0];
initial $readmemh("D:\\ram_init.txt", ram); //从文件中读取数据到存储器ram
assign data=ram[addr];
endmodule