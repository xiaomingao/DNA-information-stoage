#perl decode_random.pl ./temp 10751 3
use List::Util 'shuffle';
use File::Copy;
#$input = "./temp";
$input = $ARGV[0];
$num = $ARGV[1];
$times = $ARGV[2];
#$output = "./temp2/decode_file_cod";

opendir(DIR, $input);
@file = readdir(DIR);
print @file."\n";
for($i=0;$i<$times;$i++)
{
	
	$folder = "./temp".$i;	
	mkdir($folder);
	copy("./encode/encode_file_meta.txt", $folder);
	@array1 = shuffle(@file);
	for($m=0;$m<$num;$m++)
	{
		$one = $input."/".$array1[$m];
		$other = $folder."/decode_file_cod".$m;
		copy($one,$other);
	}
}

#meta file