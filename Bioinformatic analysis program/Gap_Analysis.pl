#!/usr/bin/perl
$input=$ARGV[0];
$output=$ARGV[1];
open CA,"$input";
open CB,">$output";
while($line1 = <CA>)
{	
	@array1 = split /\s+/,$line1;
	if(not defined $id{$array1[0]})
	{
		$id{$array1[0]}=$array1[5];
	}
	else {
		$id{$array1[0]}=$id{$array1[0]}+$array1[5];
	}
}
$total_nt_number=0;
foreach my $key ( keys %id )
{
	$total_nt_number=$total_nt_number+$id{$key};
	if(not defined($reads_numer{$id{$key}}))
	{
		$reads_numer{$id{$key}}=1;
	}
	else {
		$reads_num{$id{$key}}++;
	}	
}
print "Total Gap nt number\t".$total_nt_number."\n";
foreach my $key ( sort {$reads_num{$b}<=>$reads_num{$a}} keys %reads_num )
{
	print CB "value=".$key."\t".$reads_num{$key}."\n";	
}
close CB;