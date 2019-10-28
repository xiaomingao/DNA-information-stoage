#!/usr/bin/perl
# 参数 标准信息区域长度，首端长度，
#perl 1.extract.pl 4317607 40000 ./S44_test.csv
#$head = 30;
#$tail = 30;
$num=$ARGV[0];
$i = $ARGV[1];
$input = $ARGV[2];
#$num = 4317607;
#$i = 11519*3;
#这里加随机
while($i>0)
{
	$p = int(rand($num))+1;
	if(not defined($ran{$p}))
	{
		$ran{$p}=1;
		$i--;
	}
}




open CA,$input;
open CB,">./temp.txt";
$i=0;
while($line1 = <CA>)
{
	$i++;
	if(defined($ran{$i}))
	{
		print CB $line1;
	}

}

close CA;
close CB;

open CA,"./temp.txt";
open CB,">out.txt";

$list={};
while($line1 = <CA>)
{
	$i++;
	if($line1 =~ /^>/)
	{
		chomp($line1);
		$id=$line1;
	}
	else {
		$substr1_1 = substr($line1,25,2);
		$substr1_2 = substr($line1,167,1);
		$substr2_1 = substr($line1,19,1);
		$substr2_2 = substr($line1,160,2);
		if(($substr1_1 =~ /TT|TA/) && ($substr1_2 =~ /A|T/))
		{
			$substrl= substr($line1,17,8).substr($line1,27,140).substr($line1,168,4);
		}
		else {
			$substrl= substr($line1,15,4).substr($line1,20,140).substr($line1,162,8);
		}
#		print CB $substrl."\n";
		if(not (defined ($list{$substrl})))
		{
			$list{$substrl}=1;
#			$seq{$id}=1;
##			if($list{$substr}==100)
##				{
##					last;
##				}
		}
		else {
			$list{$substrl}=$list{$substrl}+1;
		}
	}
#	if($i==100000)
#	{
#		$i=0;
#		#print time."\n";
#	}
}
close CA;

foreach $keys (sort { $list{$b} <=> $list{$a} } keys  %list)
{
	print CB $list{$keys}."\t".$keys."\n";
}
system("perl ./2.rev.pl");
system("perl ./3.ana.pl");