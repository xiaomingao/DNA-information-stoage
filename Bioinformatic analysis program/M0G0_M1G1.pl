#!/usr/bin/perl
#perl code_ana_new_number.pl ../data/comb+blast/s1_test2.txt ./data/M0M1_groups 142

$input=$ARGV[0];
$output=$ARGV[1];
$length=$ARGV[2];
#$input="../data/comb+blast/s1_test2.txt";
#$output="../data/M0M1_groups";
$length1=$length+1;
$length2=$length-1;
$limit = 2;
$bound=$limit+1;
%list={};
%count={};
$seq_G00=$seq_G01=$seq_G10=$seq_G11="";
mkdir($output);
$G0M0=$G1M1=0;
open CA,"$input";

open CAA,">./G0M0.txt";
open CAB,">./G0M1.txt";
open CAC,">./G1M0.txt";

open CAD,">./G1M1.txt";


while($line1 = <CA>)
{	
	@array1 = split /\s+/,$line1;
	#print $line1;
	if(not defined $id{$array1[0]})
	{
		$id{$array1[0]}=1;			
		if($array1[3]==$length)
		{
			if($array1[4]>=0 && $array1[4]<$limit && $array1[5]==0)
			{
				$G0{$array1[4]}{$array1[1]}++;
				$G0x{$array1[4]}++;
				if($array1[4] == 0)
				{
					print CAA $array1[0]."\n";
				}
				else {
					print CAB $array1[0]."\n";
				}
			}
		}		
		if($array1[3]<=$length1 && $array1[3]>=$length)
		{
			if($array1[4]<$limit && $array1[5]==1)
			{
				$G1{$array1[4]}{$array1[1]}++;
				$G1x{$array1[4]}++;
				if($array1[4] == 0)
				{
					print CAC $array1[0]."\n";
				}
				else {
					print CAD $array1[0]."\n";
				}
			}
		} 				
	}
}
close CA;
print "G0M0\t".$G0x{0}."\n";
print "G0M1\t".$G0x{1}."\n";
print "G1M0\t".$G1x{0}."\n";
print "G1M1\t".$G1x{1}."\n";


close CAA;
close CAB;
close CAC;
close CAD;


foreach $keys (keys %G0)
{
	#print $keys."\n";
	$file = $output."/G0_M".$keys.".txt";
	open CB,">$file";
	#print $keys."\n";
	foreach $keys2( keys %{$G0{$keys}})
	{

		if(not defined($count{$G0{$keys}{$keys2}}))
				{
					$count{$G0{$keys}{$keys2}}=1;
				}
				else 
				{
					$count{$G0{$keys}{$keys2}}++;
				}			
		
	}
	
	foreach $keys (sort {$a<=>$b} keys %count)
	{
		if($keys>0)
		{
			print CB $keys."\t".$count{$keys}."\n";
		}
	}
	close CB;
	%count={};
}

foreach $keys (keys %G1)
{
	print $keys."\n";
	$file = $output."/G1_M".$keys.".txt";
	open CB,">$file";
	print $keys."\n";
	foreach $keys2( keys %{$G1{$keys}})
	{

		if(not defined($count{$G1{$keys}{$keys2}}))
				{
					$count{$G1{$keys}{$keys2}}=1;
				}
				else 
				{
					$count{$G1{$keys}{$keys2}}++;
				}			
		
	}
	
	foreach $keys (sort {$a<=>$b} keys %count)
	{
		if($keys>0)
		{
			print CB $keys."\t".$count{$keys}."\n";
		}
	}
	close CB;
	%count={};
}




