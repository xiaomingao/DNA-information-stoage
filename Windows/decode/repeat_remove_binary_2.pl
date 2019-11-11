#perl ./lib/repeat_remove_binary_2.pl ./temp
use File::Compare;
#$input = "./temp";
$output = $ARGV[1];
open CA,$input;
opendir(DIR, $input);
@file = readdir(DIR);
$i=$m=0;
foreach $file (@file)
{
	$new = $input."/".$file;
	open CA,$new;
	$seq="";
	while($line1 = <CA>)
	{
		$seq=$seq.$line1;
	}
	close CA;
	if(not defined($list{$seq}))
	{
		$list{$seq}=$new;
#		$new_file = $output.$i;
		$i++;
		#print $new."\n";
	}
	else {
		unlink($new);
		#print $new."\t".$list{$seq}."\n";
	}
}
print "Total decode set\t".$i."\n";
#meta file