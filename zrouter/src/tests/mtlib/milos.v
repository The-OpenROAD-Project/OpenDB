module top (clk, in, out, out4, out5, out6);
input clk, in;
output out, out4, out5, out6;
TC2DFFQXC FF_0 (.DATA( in), .CLK( clk), .Q( n1));
TC2DFFQXC FF_1 (.DATA( net_bufn1), .CLK( clk), .Q( out));
TC2DFFQXC FF_2 (.DATA( net_bufn1), .CLK( clk));
TC2DFFQXC FF_3 (.DATA( in), .CLK( clk), .Q( n2));
TC2DFFQXC FF_4 (.DATA( net_bufn2), .CLK( clk), .Q( out4));
TC2DFFQXC FF_5 (.DATA( net_bufn2), .CLK( clk), .Q( out5));
TC2DFFQXC FF_6 (.DATA( net_bufn2), .Q( out6));
TC2BUFXA bufn1 (.A( n1), .Y( net_bufn1));
TC2BUFXA bufn2 (.A( n2), .Y( net_bufn2));
endmodule
