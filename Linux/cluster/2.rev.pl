#!/usr/bin/perl
# 参数 标准信息区域长度，首端长度，

open CA,"./out.txt";
open CB,">./rev.txt";
$list={};
while($line1 = <CA>)
{
	chomp($line1);
	@array1 = split /\s+/,$line1;
	#print $seq."\n";
	if(not defined ($list{$array1[1]}))
	{
		$list{$array1[1]}=$array1[0];
		$seq = reverse($array1[1]);
		$seq =~ s/A/X/g;
		$seq =~ s/T/A/g;
		$seq =~ s/X/T/g;
		$seq =~ s/G/X/g;
		$seq =~ s/C/G/g;
		$seq =~ s/X/C/g;
		#print CB $seq."\n";
		$list{$seq}=$array1[0];
	}
	else {
		#print CB $line1."\n";
		$list{$seq}=$list{$seq}+$array1[0];
	}

}
close CA;

foreach $keys (sort { $list{$b} <=> $list{$a} } keys  %list)
{
	print CB $list{$keys}."\t".$keys."\n";
}