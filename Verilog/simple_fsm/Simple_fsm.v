module top_module(
    input clk,
    input areset,    // Asynchronous reset to state B
    input in,
    output out);  

    parameter A=0, B=1; 
    reg state, next_state;

    assign out = state;
    always @(*) begin    /
        if (state == A) begin
            if (in == 1'b1)
                next_state = A;
            else if (in == 1'b0)
                next_state = B;
        end
        else if (state == B) begin
            if (in == 1'b1)
                next_state = B;
            else if (in == 1'b0)
                next_state = A;
        end       
    end

    always @(posedge clk, posedge areset) begin    
        // State flip-flops with asynchronous reset
        if (areset == 1'b1)
            state <= B;
        else
            state <= next_state;
    end

endmodule
