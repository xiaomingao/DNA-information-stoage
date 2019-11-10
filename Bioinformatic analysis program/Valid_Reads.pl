#!/usr/bin/perl

#根据输入的长度信息，对blast后的比对结果中的符合参数长度的序列的错误匹配和空白匹配进行统计
#perl Total_reads_mis_gap_num.pl ../combine/data/combine2codeall.txt ./out_put 192 3 3
#perl Total_reads_mis_gap_num.pl blast结果 输出文件 匹配长度 mismarch gap

$input=$ARGV[0];
$output=$ARGV[1];
$length=$ARGV[2];
$gap=$ARGV[4];
$mis=$ARGV[3];
$range = $length+$gap;
$range2 = $length-$gap;


open CA,"$input";
while($line1 = <CA>)
{	
	@array1 = split /\s+/,$line1;
	#print $line1;
	if(not defined $id{$array1[0]})
	{
		$id{$array1[0]}=1;
		if($array1[3]<=$range && $array1[4]<=$mis && $array1[5]<=$gap && $array1[3]>=$length)
			{
				$gene{$array1[1]}=1;
				$total_reads_num++;
				$total_mis = $total_mis+$array1[4];
				$total_gap = $total_gap+$array1[5];
			} 	
	} 
	
}
close CA;

print "Total reads number: ".$total_reads_num."\n"."Total mismarch number: ".$total_mis."\n"."Total gap number: ".$total_gap."\n";

open CB,">$output";
print CB "Total reads number: ".$total_reads_num."\n"."Total mismarch number: ".$total_mis."\n"."Total gap number: ".$total_gap;
