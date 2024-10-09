#!/usr/bin/perl -w
#==========================================================================
# matmul_gendata.pl
#
# Author : Christopher Batten (cbatten@mit.edu)
# Date   : April 29, 2005
#
(our $usageMsg = <<'ENDMSG') =~ s/^\#//gm;
#
# Simple script which creates an input data set and the reference data
# for the matmul benchmark.
#
ENDMSG

use strict "vars";
use warnings;
no  warnings("once");
use Getopt::Long;

#--------------------------------------------------------------------------
# Command line processing
#--------------------------------------------------------------------------

our %opts;

sub usage()
{

  print "\n";
  print " Usage: gendata.pl [options] \n";
  print "\n";
  print " Options:\n";
  print "  --help  print this message\n";
  print "  --dimM  size of input data [64]\n";
  print "  --dimK  size of input data [64]\n";
  print "  --dimN  size of input data [64]\n";
  print "  --seed  random seed [1]\n";
  print "$usageMsg";

  exit();
}

sub processCommandLine()
{

  $opts{"help"} = 0;
  $opts{"dimM"} = 64;
  $opts{"dimK"} = 64;
  $opts{"dimN"} = 64;
  $opts{"seed"} = 1;
  Getopt::Long::GetOptions( \%opts, 'help|?', 'dimM:i', 'dimK:i', 'dimN:i', 'seed:i' ) or usage();
  $opts{"help"} and usage();

}

#--------------------------------------------------------------------------
# Helper Functions
#--------------------------------------------------------------------------

sub printArray
{
  my $arrayName = $_[0];
  my $arrayRef  = $_[1];
  my $dimSize  = $_[2];

  my $numCols = $dimSize;
  my $arrayLen = scalar(@{$arrayRef});


  if ( $arrayLen <= $numCols ) {
    print " ";
    for ( my $i = 0; $i < $arrayLen; $i++ ) {
      print sprintf("%3d",$arrayRef->[$i]);
      if ( $i != $arrayLen-1 ) {
        print " ";
      }
    }
    print "\n";
  }

  else {
    my $numRows = int($arrayLen/$numCols);
    for ( my $j = 0; $j < $numRows; $j++ ) {
      print " ";
      for ( my $i = 0; $i < $numCols; $i++ ) {
        my $index = $j*$numCols + $i;
        print sprintf("%3d",$arrayRef->[$index]);
        if ( $index != $arrayLen-1 ) {
          print " ";
        }
      }
      print "\n";
    }

    if ( $arrayLen > ($numRows*$numCols) ) {
      print " ";
      for ( my $i = 0; $i < ($arrayLen-($numRows*$numCols)); $i++ ) {
        my $index = $numCols*$numRows + $i;
        print sprintf("%3d",$arrayRef->[$index]);
        if ( $index != $arrayLen-1 ) {
          print " ";
        }
      }
      print "\n";
    }

  }

  print  "\n";
}



#--------------------------------------------------------------------------
# Matmul
#--------------------------------------------------------------------------

# http://answers.oreilly.com/topic/418-how-to-multiply-matrices-in-perl/

sub mmult {
    my ($m1,$m2) = @_;
    my ($m1rows,$m1cols) = matdim($m1);
    my ($m2rows,$m2cols) = matdim($m2);

    my $result = [  ];
    my ($i, $j, $k);

    for $i (range($m1rows)) {
        for $j (range($m2cols)) {
            for $k (range($m1cols)) {
                $result->[$i][$j] += $m1->[$i][$k] * $m2->[$k][$j];
            }
        }
    }
    return $result;
}

sub range { 0 .. ($_[0] - 1) }


sub veclen {
    my $ary_ref = $_[0];
    my $type = ref $ary_ref;
    if ($type ne "ARRAY") { die "$type is bad array ref for $ary_ref" }
    return scalar(@$ary_ref);
}

sub matdim {
    my $matrix = $_[0];
    my $rows = veclen($matrix);
    my $cols = veclen($matrix->[0]);
    return ($rows, $cols);
}

#--------------------------------------------------------------------------
# Main
#--------------------------------------------------------------------------

sub main()
{

  processCommandLine();
  srand($opts{"seed"});

  # create random input arrays
  my $mat_values1;
  my $mat_values2;
  for ( my $i = 0; $i < $opts{"dimM"}; $i++ ) {
    for ( my $j = 0; $j < $opts{"dimK"}; $j++ ) {
      $mat_values1->[$i][$j] = int(rand(4));
    }
  }
  for ( my $i = 0; $i < $opts{"dimK"}; $i++ ) {
    for ( my $j = 0; $j < $opts{"dimN"}; $j++ ) {
      $mat_values2->[$i][$j] = int(rand(4));
    }
  }

  # perform matmul
  my $mat_results = mmult( $mat_values1, $mat_values2 );
  
  my @values1;
  my @values2;
  my @results;
  for ( my $i = 0; $i < $opts{"dimM"}; $i++ ) {
    for ( my $j = 0; $j < $opts{"dimK"}; $j++ ) {
        my $value1 = $mat_values1->[$i][$j];
        push( @values1, $value1 );
    }
  }
  for ( my $i = 0; $i < $opts{"dimK"}; $i++ ) {
    for ( my $j = 0; $j < $opts{"dimN"}; $j++ ) {
        my $value2 = $mat_values2->[$i][$j];
        push( @values2, $value2 );
    }
  }
  for ( my $i = 0; $i < $opts{"dimM"}; $i++ ) {
    for ( my $j = 0; $j < $opts{"dimN"}; $j++ ) {
        my $result = $mat_results->[$i][$j];
        push( @results, $result );
    }
  }
  
  print $opts{"dimM"}," ",$opts{"dimK"}," ",$opts{"dimN"},"\n"; 
  printArray( "input1_data", \@values1, $opts{"dimK"});
  printArray( "input2_data", \@values2, $opts{"dimN"});
  printArray( "verify_data", \@results, $opts{"dimN"});

 
}

main();

