
module adder(input a,input b,input cin, output cout,sum);
    assign sum = a^b^cin;
    assign cout = (a&b)|(b&cin)|(cin&a);
endmodule

module top_module(   //100 bit binary ripple carry adder
    input [99:0] a, b,
    input cin,
    output [99:0] cout,
    output [99:0] sum );
    
    genvar i;
    
    adder FA1(a[0],b[0],cin,cout[0],sum[0]);
    
    generate
        for (i=1; i<$bits(a); i++) begin: Full_adder
            adder FA(a[i],b[i],cout[i-1],cout[i],sum[i]);
        end 
    endgenerate
    

endmodule
