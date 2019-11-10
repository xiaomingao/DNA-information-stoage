#!/usr/bin/perl
#perl extract_seq_query.pl blast_input seq_input match_output;

$blast_input = $ARGV[0];
$seq_input = $ARGV[1];
$match_output= $ARGV[2];

open CA,"$blast_input";
while($line1 = <CA>)
{
	@array1 = split /\s+/,$line1;
	if(not defined($list{$array1[0]}))
	{
		$list{$array1[0]}=$array1[6].";".$array1[7];
	}
}
close CA;


$id="";
#$seq={};
open CA,"$seq_input";
while($line1 = <CA>)
{
	if($line1 =~ /^>/)
	{
		chomp($line1);
		$line1 =~ s/\r//;
		@array1 = split />|\s+/,$line1;
		$id=$line1."_match_part";
		$match{$id}=$array1[1];
	}
	else {
		chomp($line1);
		$seq{$id}=$seq{$id}.$line1;
	}
}
close CA;

open CB,">$match_output";


foreach $keys (keys %seq)
{
	print CB $keys."\t".$list{$match{$keys}}."\n";
	@sp=split /;/,$list{$match{$keys}};
	$sp[0]=$sp[0]-1;
	#print $sp[0];
	@array = split //,$seq{$keys};
	#print $array[2];
	for($i=$sp[0];$i<$sp[1];$i++)
	{
		print CB $array[$i];
	}
	print CB "\n";
}
close CB;