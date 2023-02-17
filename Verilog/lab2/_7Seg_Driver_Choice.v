`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/25 20:41:30
// Design Name: 
// Module Name: _7Seg_Driver_Choice
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

module _7Seg_Driver_Choice(SW, SEG, AN, LED);
    input [15:0] SW;       // 16λ��������
    output [7:0] SEG;      // 7��������������͵�ƽ��Ч
    output [7:0] AN;       // 7�������Ƭѡ�źţ��͵�ƽ��Ч
    output [15:0] LED;     // 16λLED��ʾ
    selector(SW[15:13], AN[7:0]);
    decoder(SW[3:0], SEG[7:0]);
    assign LED[15:0] = SW[15:0];
endmodule