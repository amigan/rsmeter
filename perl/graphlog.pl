#!/usr/bin/perl
# Takes RSMeter logs and graphs them using GD::Graph.
# (C)2004, Dan Ponte
use GD::Graph::lines;
use GD::Graph::colour qw( :files );
GD::Graph::colour::read_rgb( "/usr/X11R6/lib/X11/rgb.txt");
if($ARGV[0] eq '' or $ARGV[1] eq '')
{
print STDERR "Needs more arguments.\n (@ARGV)";
exit 1;
}
open(TLG, $ARGV[0]);
foreach $c (<TLG>)
{
	if($c =~ /([0-9]*):([0-9]*).([0-9]*): (.*)/) {
		$csec = $3;
		$tdat = $4;
		if($tdat =~ /(mV|V|uA|A|Ohm|KHz|Hz)/i and $units !~ /$1/) 
		{ $units .= "$1/"; }
		$tdat =~ s/[^0-9-]*//g;
		push @labels, $csec;
		push @dater, $tdat;
	}
}
close(TLG);
my @final = (\@labels, \@dater);
my $graph = GD::Graph::lines->new(640, 480);
$graph->set(title => "RSMeter output",
	    y_label => $units,
	    boxclr => "black", dclrs => ["orange", "SteelBlue"],
	    shadowclr => "grey", shadow_depth => 3, bgclr => "LightGrey", fgclr => "LimeGreen", labelclr => "LimeGreen",
		axislabelclr => "LimeGreen", textclr => "orange");
my $image = $graph->plot(\@final);
my $pngData = $image->png();
open(OUTFILE, ">" . $ARGV[1]);
binmode OUTFILE;
print OUTFILE $pngData;
close OUTFILE;
