#include <unistd.h>

int main() {
	setuid(1001);
	seteuid(1001);
	execl("/usr/bin/perl", "perl", "/home/inzane/public_html/www/updatePage.cgi", NULL);
	return 0;
}
