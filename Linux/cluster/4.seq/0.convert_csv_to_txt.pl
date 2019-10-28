#!/usr/bin/perl
# 参数 标准信息区域长度，首端长度，
#perl 0.convert_csv_to_txt.pl ./encode.csv ./code.txt 
$input = $ARGV[0];
$output = $ARGV[1];
#open CA,"./encode.csv";
#open CB,">./code.txt";
$i=0;
while($line1 = <CA>)
{
	chomp($line1);
	$line1 =~ s/,//g;
	$line1 =~ s/\r//g;
	print CB $line1."\n";
}
close CB;
close CA;


