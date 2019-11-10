#!/usr/bin/perl

$input1 = $ARGV[0];
$input2 = $ARGV[1];

open CA,"$input1";
while($line1 = <CA>)
{
	chomp($line1);
	$line1 =~ s/\r//g;
	@array1 = split />/,$line1;
	$list{$array1[1]}=1;
}
close CA;

open CA,"$input2";
while($line1= <CA>)
{
	@array1 = split /\s+/,$line1;
	$list2{$array1[1]}=1;
}
close CA;

$m=0;
foreach $keys ( keys %list )
{
	if(not defined($list2{$keys}))
	{
		print $keys."\n";
		$m++;
	}
}
print "Unmapped number:".$m."\n";