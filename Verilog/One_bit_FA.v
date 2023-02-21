module adder(input a,input b,input cin, output cout, sum);
    assign sum = a^b^cin;
    assign cout = (a&b)|(b&cin)|(cin&a);
endmodule
