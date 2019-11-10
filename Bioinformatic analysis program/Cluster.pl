#!/usr/bin/perl

$input = $ARGV[0];
$output = $ARGV[1];
open CA,"$input";
while($line1 = <CA>)
{
	chomp($line1);
	$line1 =~ s/\r//g;
	if($line1 =~/^>/)
	{
		$id = $line1;
		$seq{$id}="";
	}
	else {
		$seq{$id}=$seq{$id}.$line1;
	}
}
close CA;
$m=0;
foreach $keys (keys %seq)
{
	if(not defined $list{$seq{$keys}})
	{
		if(not defined $re_list{$seq{$keys}})
		{
			$list{$seq{$keys}}=1;
			@array1 = split //,$seq{$keys};
			$len = @array1-1;
			$revise="";
			for($i = $len;$i>=0;$i--)
			{
				#print $array1[$i]."\t";
				if($array1[$i]=~ /A|a/)
				{
					$revise=$revise.'T';
				}
				if($array1[$i]=~ /T|t/)
					{
						$revise=$revise.'A';
					}
				if($array1[$i]=~ /G|g/)
					{
						$revise=$revise.'C';
					}
				if($array1[$i]=~ /C|C/)
					{
						$revise=$revise.'G';
					}
				
			}
			$re_list{$revise}=$seq{$keys};
			$clust{$seq{$keys}}=$keys;
			$m++;
		}
		else {
			#print "1\n";
			$revise= $re_list{$seq{$keys}};
			$list{$revise}=$list{$revise}+1;
			$clust{$revise}=$clust{$revise}.";".$keys;
		}
	}
	else {
		$list{$seq{$keys}}=$list{$seq{$keys}}+1;
		$clust{$seq{$keys}}=$clust{$seq{$keys}}.";".$keys;
	}
}

print "Total_cluster_number:$m\n";
open CB,">$output";
foreach $keys (keys %list)
{
	$clust{$keys}=~ s/>//g;
	print CB $list{$keys}."\t".$clust{$keys}."\n";
}
close CB;