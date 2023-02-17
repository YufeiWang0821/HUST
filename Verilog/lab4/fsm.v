`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/05/16 20:43:50
// Design Name: 
// Module Name: fsm
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


module fsm(clk,rst,start,next_zero,ld_sum,ld_next,sum_sel,next_sel,a_sel,done);
    input clk,rst,start,next_zero;
    output ld_sum,ld_next,sum_sel,next_sel,a_sel,done;
    reg[3:0]state;
    reg[5:0]out;
    localparam START=0,COMPUTE_SUM=1,GET_NEXT=2,DONE=4;
    assign{ld_sum,ld_next,sum_sel,next_sel,a_sel,done}=out;
    initial begin
        state<=START;
        out<=6'b000000;
    end
    always@(posedge clk)begin
        if(rst)begin
            state<=START;
            out<=6'b000000;
        end
        else begin
            case(state)
                START:begin
                    if(start==1)begin
                        state<=COMPUTE_SUM;
                        out<=6'b101110;
                    end   
                    else if(start==0)begin
                        state<=START;
                        out<=6'b000000;
                    end
                end
                COMPUTE_SUM:begin
                    state<=GET_NEXT;
                    out<=6'b011100;
                end
                GET_NEXT:begin
                    if(next_zero==1)begin
                        state<=DONE;
                        out<=6'b000001;
                    end   
                    else if(next_zero==0)begin
                        state<=COMPUTE_SUM;
                        out<=6'b101110;
                    end
                end
                DONE:begin
                    if(start==1)begin
                        state<=DONE;
                        out<=6'b000001;
                    end   
                    else if(start==0)begin
                        state<=START;
                        out<=6'b000000;
                    end
                end
             endcase
      end      
    end
endmodule
