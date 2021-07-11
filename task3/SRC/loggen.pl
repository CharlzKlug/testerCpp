#!/usr/bin/env perl

use strict;
use warnings;

use DateTime ;
use DateTime::Duration;

my $dt = DateTime->new(
    year       => 1964,
    month      => 10,
    day        => 16,
    hour       => 16,
    minute     => 12,
    second     => 47,
    nanosecond => 500000000,
    time_zone  => 'Asia/Taipei',
    );

print 300 . "\n" . 21 . "\n" ;

for (my $i = 0 ; $i < 18000 ; $i++) {
    my $dur= DateTime::Duration->new(
	nanoseconds  => int (rand (1000000000000)),
	end_of_month => 'wrap',
	);
    $dt->add_duration ($dur) ;

    print $dt->year . "-" . sprintf ("%02d", $dt->month) . "-" .
	sprintf ("%02d", $dt->day) . "T" .
	sprintf ("%02d", $dt->hour) .
	":" . sprintf ("%02d", $dt->minute) . ":" .
	sprintf ("%02d", $dt->second) . ":" .
	sprintf ("%03d", int ($dt->nanosecond / 1000000)) .
	"Z - [username" . int (rand (10)) .
	"] - wanna " ;

    my $action = int (rand (10)) ;

    print "top up " if ($action >= 5) ;
    print "scoop " if ($action < 5) ;

    print int (rand (100)) . "l\n" ;
    
}
