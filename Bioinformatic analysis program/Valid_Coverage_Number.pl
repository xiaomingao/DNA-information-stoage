#!/usr/bin/perl

#对blast数据的不匹配。空白的详细分类，通过id归组的方式。
#perl G01M01_num.pl ../data/comb+blast/s1_test2.txt ./data/M0M1_groups 192

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
mkdir($output);

$file=$output."/G01_M01_seq.txt";
open CC,">$file";

open CA,"$input";
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
				$G01M01{$array1[1]}++;
				print CC $line1;
				$total++;
			}
		}	
		
			
		if($array1[3]<=$length1 && $array1[3]>=$length)
		{
			if($array1[4]<2 && $array1[5]==1)
			{
				$G01M01{$array1[1]}++;
				print CC $line1;
				$total++;
			}
		} 				
		
	}
}
close CA;
close CC;

print "Total reads number:\t".$total."\n";

foreach $keys (keys %G01M01)
{
	if(not defined($count{$G01M01{$keys}}))
	{
		$count{$G01M01{$keys}}=1;
		$id{$G01M01{$keys}} = $keys;
	}
	else {
		$count{$G01M01{$keys}}++;
		$id{$G01M01{$keys}} = $id{$G01M01{$keys}}.",".$keys;
	}
}

$file=$output."/G01_M01.txt";
open CB,">$file";
foreach $keys (sort {$a<=>$b} keys %count)
{
	if($keys>0)
	{
			print CB $keys."\t".$count{$keys}."\t".$id{$keys}."\n";
	}
}
close CB;
%count={};




