`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/09 20:34:20
// Design Name: 
// Module Name: DynamicScan
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


module DynamicScan(clk,  SEG, AN);
input clk;              // ϵͳʱ��
output [7:0] SEG;  		// �ֱ��ӦCA��CB��CC��CD��CE��CF��CG��DP
output [7:0] AN;        // 8λ�����Ƭѡ�ź�
wire clkn;
wire [2:0] num;
wire [3:0] code;
divider(clk,clkn);
counter(clkn,num[2:0]);
decoder3_8(num[2:0],AN[7:0]);
rom8x4(num[2:0],code[3:0]);
pattern(code[3:0],SEG[7:0]);
endmodule