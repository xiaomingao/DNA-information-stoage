#!/usr/bin/perl
# 参数 标准信息区域长度，首端长度，

#open CA,"./out.txt";
open CA,"./rev.txt";
open CB,">./database.txt";
$i=0;
while($line1 = <CA>)
{
	chomp($line1);
	@array1 = split /\s+/,$line1;
	#print $seq."\n";
	if($array1[0] >0)
	{
		$i++;
		$list{$array1[1]}=1;
		$seq = reverse($array1[1]);
		$seq =~ s/A/X/g;
		$seq =~ s/T/A/g;
		$seq =~ s/X/T/g;
		$seq =~ s/G/X/g;
		$seq =~ s/C/G/g;
		$seq =~ s/X/C/g;
		print CB ">ID".$i."\n";
		print CB $seq."\n";
		$list{$seq}=1;
		
	}

}
close CA;

print $i."\t";
$k=0;
open CA,"./encode.txt";
open CB,">temp.txt";
while($line1 =<CA>)
{
	chomp($line1);
	if(defined($list{$line1}))
	{
		$k++;
	}
	else {
		print CB ">ID".$k."\n";
		print CB $line1."\n";
	}
	#print $line1;
}
close CA;
close CB;
print $k."\n";

