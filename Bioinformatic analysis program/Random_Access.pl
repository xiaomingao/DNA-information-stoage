#!/usr/bin/perl
#perl random_5M.pl ./G01m01_random/G01_M01_seq.txt 百分比 轮数 输出文件名字
$full_file = $ARGV[0];
$input1 = $ARGV[1];
$input2 = $ARGV[2];
$output = $ARGV[3];

$round = $input2;

$m=$n=1;

open CA,"$full_file";
while($line1 = <CA>)
{
	$random{$n}=$line1;
	$n++;
}
$file_full_length = keys %random;
print "Total dataset size = $file_full_length\n";

$length = int($input1*$n/100);
print "select reads number:$length\n";

for(1..$round)
{
	open CB,">$output"."_$m.txt";
	$m++;
	for(1..$length)
	{
		$select = int(rand($n)+1);
		print CB $random{$select};
	}
	close CB;
}



