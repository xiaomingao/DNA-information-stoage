#perl trans.pl ./code.txt 
$input = $ARGV[0];
#$output = $ARGV[1];
mkdir("./temp");
$output = "./temp/encode_file_cod";
open CA,$input;
$i=0;
while($line1 = <CA>)
{
	if($line1=~/^A|C|G|T/)
	{
		chomp($line1);
		$file =$output.$i;
		system("python trans.py\t$line1\t$file");
		$i++;
	}
}
close CA;