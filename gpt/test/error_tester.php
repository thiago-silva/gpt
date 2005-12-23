#!/usr/local/bin/php

<?
	function print_source($src) {
		$lines = explode("\n", $src);
		$c = 1;
		foreach($lines as $line) {
			echo $c . " $line". "\n";
			$c++;
		}    
		echo "--------\n";
	}

  $gpt = "/home/jester/projects/upis/trunk/pparser/debug/src/gpt";
  $feedname = "feed_test";

   if(!file_exists($feedname)) {
     die("no file named $feedname\n");
   }

  $feed = file_get_contents($feedname);

    
  $matches = array();
  preg_match_all("/<([^>]*)>/", $feed, $matches);
//   print_r($matches);

  $sources = $matches[1];

  $char = '';
  $idx = 0;
  while(($char != 'q') && ($idx < count($sources))) {
    system("clear");    
    echo "Source idx: $idx\n\n";
    print_source($sources[$idx]);
    system("echo \"".$sources[$idx]."\" | $gpt -p ");
    $char = trim(fgets(STDIN));

    if(is_numeric($char) && ($char > 0) && ($char < count($sources))) {
      $idx = $char;
    } else if($char != 'b') {
      $idx++;
    } else {
			$idx--;
			if($idx < 0) $idx = 0;
		}
  }	
?>
