#!/usr/bin/env perl
use strict;
use warnings;
use File::Slurp;

my ($out, @files) = @ARGV;
my @sizes;
open(my $handle, '>', $out);
print $handle "static char *INPUT_FILES_NAME[] = {\n";
for (@files) {
    print $handle "    \"$_\",\n";    
}
print $handle "};\n\n";
print $handle "static unsigned char INPUT_FILES_DATA[] = {\n";
for (@files) {
    my $data = read_file($_);
    print $handle "/* start file $_ */\n";
    print $handle "    ";
    for (my $i = 0; $i < length($data); $i++) {
	printf $handle "%3d,", ord(substr($data, $i, 1));
	print $handle "\n    " unless (($i + 1) % 16);
    }
    print $handle "\n/* end file $_ */\n";
    push @sizes, length($data);
}
print $handle "};\n\n";

print $handle "static unsigned int INPUT_FILES_SIZE[] = {\n";
for (@sizes) {
    print $handle "    $_,\n";
}
print $handle "};\n";
