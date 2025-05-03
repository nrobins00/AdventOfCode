#!/usr/bin/perl

$filename = "input.txt";
open(fh, $filename) or die "couldn't open $filename";

$i = 0;
while (<fh>) {
	my @nums = split();
	$left[$i] = $nums[0];
	$right[$i] = $nums[1];
	$i++;
}

@left = sort(@left);
print "@left";
@right = sort(@right);
print "@right";

$sum = 0;
for $j (0..$#left) {
	print "left $left[$j]; right $right[$j]\n";
	$sum += abs($left[$j] - $right[$j]);
}
print "sum: $sum\n";

close;
