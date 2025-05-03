#!/usr/bin/perl
$filename = "input.txt";
open( fh, $filename ) or die "couldn't open $filename";

$i = 0;
while (<fh>) {
	my @nums = split();
	$left[ $i++ ] = $nums[0];
	print "\$nums[1] = $nums[1]\n";
	$right{ $nums[1] }++;
}

@left = sort(@left);
print "@left\n";
@right = sort(@right);

$score = 0;

for $j ( 0 .. $#left ) {
	$val = $left[$j];
	print "left $left[$j]; right $right{$left[$j]}\n";
	$score += $left[$j] * $right{$val};
}
print "score: $score\n";

close;
