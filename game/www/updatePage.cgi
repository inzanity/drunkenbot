#!/usr/bin/perl

use strict;
use POSIX;


print "Content-type: text/plain\n\n";
print "Updating pages...\n";

chdir("/home/inzane/public_html/www") || die;
system("CVS_RSH=ssh /usr/bin/cvs update > /dev/null");
chdir("docs/www");
system("CVS_RSH=ssh /usr/bin/cvs update -dA");
chdir("../..");

for (<docs/www/*>) {
	s/^docs\/www\///;
	next if ($_ eq "CVS");
	if (-d "docs/www/$_") {
		mkdir $_ if (! -d $_);
		my @files = sort <docs/www/$_/ch-*.txt>;
		my $index = shift @files;
		my ($toc, $menu) = generateToc(@files);
		my $allinone = readTxt($index);
		createPage("$_/index", $allinone, $menu, -1);
		createPage("$_/tow", $toc, $menu, 0);

		$allinone .= $toc;

		for my $file (@files) {
			my $txt = readTxt($file);
			$file =~ s/^.*\/(ch-[0-9]+)\.txt$/$1/;
			$allinone .= $txt;
			createPage("$_/$file", $txt);
		}

		$allinone =~ s/ch-[0-9]+.html//g;
		createPage("$_/allinone", $allinone);
		print "Done $_.";
	}
}

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
	open $fh, "$ch";
	while (<$fh>) {
		chomp;
		chomp;
		if (/^[0-9]+\.\t/) {
			while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			s/^([0-9]+\.)\t(.+)$/<h2 id="h$1">$1 $2<\/h2>/; 
		} elsif (/[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+\t/) {
			while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			s/^([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)\t(.+)$/<h5 id="h$1">$1 $2<\/h5>/; 
		} elsif (/[0-9]+\.[0-9]+\.[0-9]\t/) {
			while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			s/^([0-9]+\.[0-9]+\.[0-9]+)\t(.+)$/<h4 id="h$1">$1 $2<\/h4>/; 
		} elsif (/[0-9]+\.[0-9]+\t/) { 
			while ($ullevel > 0) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			s/^([0-9]+\.[0-9]+)\t(.+)$/<h3 id="h$1">$1 $2<\/h3>/; 
		} elsif (/^(\t+)-/) {
			my $req = length($1);
			while ($ullevel > $req) { $cont .= " " x ($ullevel - 1) . "</ul>\n"; $ullevel--; }
			while ($ullevel < $req) { $cont .= " " x $ullevel . "<ul>\n"; $ullevel++; }
			$req = " " x $req;
			s/^\t+- /$req<li>/;
			s/(\(see[^)]+?)([0-9]+)(\.[0-9.]+)(\))/$1<a href="ch-$2.html#h$2$3">$2$3<\/a>$4/gi;
		} elsif ($_) {
			s/(\(see[^)]+?)([0-9]+)(\.[0-9.]+)(\))/$1<a href="ch-$2.html#h$2$3">$2$3<\/a>$4/gi;
			s/(see )([0-9]+)(\.[0-9.]+)/$1<a href="ch-$2.html#h$2$3">$2$3<\/a>/gi;
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
	my (@pages) = @_;
	my $cont = "";
	my $fh;
	my $qlevel = 0;
	my $menuobjs = {};
	for (@pages) {
		open $fh, "$_";
		my $ch0 = s/^.*-([0-9]+)\.txt/$1/;
		while (<$fh>) {
			chomp;
			chomp;
			if (/^[0-9]+\.\t/) {
				while ($qlevel > 0) { $cont .= "</blockquote>\n"; $qlevel--; }
				s/^([0-9]+\.)\t(.+)$/<a href="ch-$ch0.html#h$1">$1 $2<\/a><br>/; 
				my $chap = $1;
				chop $chap;
				$$menuobjs{$chap} = $2;
			} elsif (/[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+\t/) {
				while ($qlevel < 3) { $cont .= "<blockquote>\n"; $qlevel++; }
				s/^([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)\t(.+)$/<a href="ch-$ch0.html#h$1">$1 $2<\/a><br>/; 
			} elsif (/[0-9]+\.[0-9]+\.[0-9]+\t/) {
				while ($qlevel > 2) { $cont .= "</blockquote>\n"; $qlevel--; }
				while ($qlevel < 2) { $cont .= "<blockquote>\n"; $qlevel++; }
				s/^([0-9]+\.[0-9]+\.[0-9]+)\t(.+)$/<a href="ch-$ch0.html#h$1">$1 $2<\/a><br>/; 
			} elsif (/[0-9]+\.[0-9]+\t/) { 
				while ($qlevel > 1) { $cont .= "</blockquote>\n"; $qlevel--; }
				while ($qlevel < 1) { $cont .= "<blockquote>\n"; $qlevel++; }
				s/^([0-9]+\.[0-9]+)\t(.+)$/<a href="ch-$ch0.html#h$1">$1 $2<\/a><br>/; 
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
