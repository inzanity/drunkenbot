#!/usr/bin/perl

use strict;
use POSIX;


print "Content-type: text/plain\n\n";
print "Updating pages...\n";

chdir("/home/inzane/public_html/www") || die;
chdir("docs");
system("CVS_RSH=ssh /usr/bin/cvs update");
chdir("..");


my $ch0 = 1;
my $ch1 = 7;
my ($toc, $menu) = generateToc($ch0, $ch1);
my $allinone = readTxt(0);
createPage("index", $allinone, $menu, -1);
createPage("toc", $toc, $menu, 0);

$allinone .= $toc;

for (; $ch0 <= $ch1; $ch0++) {
	my $txt = readTxt($ch0);
	$allinone .= $txt;
	createPage("ch-$ch0", $txt, $menu, $ch0);
}
$allinone =~ s/ch-[0-9]+.html//g;
createPage("allinone", $allinone, $menu, -2);

print "Done.\n";

sub createPage {
	my ($name, $content, $menu, $ch) = @_;
	my $rfh;
	my $wfh;
	open $rfh, "template.html";
	open $wfh, ">$name.html";
	while (<$rfh>) {
		if (/^(\s*)<!-- Content go here -->/) {
			my $indent = $1;
			$content =~ s/\n/\n$indent/g;
			$content =~ s/$indent$//;
			print $wfh $indent.$content;
		} elsif (/^(\s*)<!-- Menu go here -->/) {
			print $wfh $1 . "<a href=\"index.html\">Front page</a>\n";
			print $wfh $1 . "<a href=\"toc.html\">Table of Contents</a>\n";
			for my $ch (sort keys %$menu) {
				print $wfh $1 . "<a href=\"ch-" . $ch . ".html\">" . $$menu{$ch} . "</a>\n";
			}
		} elsif (/^(\s*)<!-- Links go here -->/) {
			print $wfh $1 . "<link rel=\"start\" href=\"index.html\">\n";
			print $wfh $1 . "<link rel=\"toc\" href=\"toc.html\">\n";
			print $wfh $1 . "<link rel=\"first\" href=\"ch-1.html\">\n";
			print $wfh $1 . "<link rel=\"end\" href=\"ch-7.html\">\n";
			print $wfh $1 . "<link rel=\"prev\" href=\"ch-" . ($ch - 1) . ".html\">\n" if ($ch > 1);
			print $wfh $1 . "<link rel=\"next\" href=\"ch-" . ($ch + 1) . ".html\">\n" if ($ch < 7 && $ch >= 0);
		} else {
				
			print $wfh $_;
		}
	}
	close $wfh;
	close $rfh;
}	

sub readTxt {
	my ($ch) = @_;
	my $fh;
	my $cont = "";
	my $ullevel = 0;
	open $fh, "docs/ch-$ch.txt";
	while (<$fh>) {
		chomp;
		chomp;
		if (/^[0-9]+\.\t/) {
			while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			s/^([0-9]+\.)\t(.+)$/<h2 id="$1">$1 $2<\/h2>/; 
		} elsif (/[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+\t/) {
			while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			s/^([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)\t(.+)$/<h5 id="$1">$1 $2<\/h5>/; 
		} elsif (/[0-9]+\.[0-9]+\.[0-9]\t/) {
			while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			s/^([0-9]+\.[0-9]+\.[0-9]+)\t(.+)$/<h4 id="$1">$1 $2<\/h4>/; 
		} elsif (/[0-9]+\.[0-9]+\t/) { 
			while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			s/^([0-9]+\.[0-9]+)\t(.+)$/<h3 id="$1">$1 $2<\/h3>/; 
		} elsif (/^(\t+)-/) {
			my $req = length($1);
			while ($ullevel > $req) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			while ($ullevel < $req) { $cont .= " " x $ullevel . "<ul>\n"; $ullevel++; }
			$req = " " x $req;
			s/^\t+- /$req<li>/;
		} elsif ($_) {
			s/(\(see[^)]+?)([0-9]+)(.[0-9.]+)(\))/$1<a href="ch-$2.html#$2$3">$2$3<\/a>$4/g;
			while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			s/^.*$/<p>$&<\/p>/;
		}
		$cont .= $_ . "\n" if ($_);
	}
	while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
	close $fh;
	return $cont;
}

sub generateToc {
	my ($ch0, $ch1) = @_;
	my $cont = "";
	my $fh;
	my $qlevel = 0;
	my $menuobjs = {};
	for (; $ch0 <= $ch1; $ch0++) {
		open $fh, "docs/ch-$ch0.txt";
		while (<$fh>) {
			chomp;
			chomp;
			if (/^[0-9]+\.\t/) {
				while ($qlevel > 0) { $cont .= "</blockquote>\n"; $qlevel--; }
				s/^([0-9]+\.)\t(.+)$/<a href="ch-$ch0.html#$1">$1 $2<\/a><br>/; 
				my $chap = $1;
				chop $chap;
				$$menuobjs{$chap} = $2;
			} elsif (/[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+\t/) {
				while ($qlevel < 3) { $cont .= "<blockquote>\n"; $qlevel++; }
				s/^([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)\t(.+)$/<a href="ch-$ch0.html#$1">$1 $2<\/a><br>/; 
			} elsif (/[0-9]+\.[0-9]+\.[0-9]+\t/) {
				while ($qlevel > 2) { $cont .= "</blockquote>\n"; $qlevel--; }
				while ($qlevel < 2) { $cont .= "<blockquote>\n"; $qlevel++; }
				s/^([0-9]+\.[0-9]+\.[0-9]+)\t(.+)$/<a href="ch-$ch0.html#$1">$1 $2<\/a><br>/; 
			} elsif (/[0-9]+\.[0-9]+\t/) { 
				while ($qlevel > 1) { $cont .= "</blockquote>\n"; $qlevel--; }
				while ($qlevel < 1) { $cont .= "<blockquote>\n"; $qlevel++; }
				s/^([0-9]+\.[0-9]+)\t(.+)$/<a href="ch-$ch0.html#$1">$1 $2<\/a><br>/; 
			} else {
				next;
			}
			$cont .= $_ . "\n" if ($_);
		}
		close $fh;
	}
	while ($qlevel > 0) { $cont .= "</blockquote>\n"; $qlevel--; }
	return ($cont, $menuobjs);
}
