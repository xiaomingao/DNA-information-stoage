#!/usr/bin/perl

$input=$ARGV[0];
$output=$ARGV[1];
open CA,"$input";
open CB,">$output";
$count=1;
while($line1 = <CA>)
{
	@array1 = split /\r|\n/,$line1;
	$len = @array1;
#	print $len."\n";
	for($i=0;$i<$len;$i++)
	{
		print CB ">ID".$count."\n";
		$count++;
		print CB $array1[$i]."\n";
	}
	
}
close CA;
close CB;