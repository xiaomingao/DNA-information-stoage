#!/usr/bin/perl

$input = $ARGV[0];
$output = $ARGV[1];
open CA,"$input";
while($line1 = <CA>)
{
	chomp($line1);
	@array1 = split /\s+/,$line1;
	if(not defined $count{$array1[0]})
	{
		$count{$array1[0]}=1;
	}
	else {
		$count{$array1[0]}=$count{$array1[0]}+1;
	}
}
close CA;


open CB,">$output";
foreach $keys (keys %count)
{
	print CB $keys."\t".$count{$keys}."\n";
}
close CB;