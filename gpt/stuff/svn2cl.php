<?
class LogEntry {
  var $author;
  var $files;
  var $msgs;

  function LogEntry() {
    $this->author = '';
    $this->files = array();
    $this->msgs = array();
  }
}

class ChangeLog {
  
  var $entries;

  function ChangeLog() {
    $xml = simplexml_load_string($this->getLog());

    foreach($xml->logentry as $log) {
      $this->processEntry($log);
    }
  }

  function getLog() {    
    $filec = `svn log --xml -v https://thiago_silva@svn.berlios.de/svnroot/repos/gpt/trunk/gpt`;
  
  //   if($GLOBALS['argc'] != 2) {
  //     die("-missing arg\n");
  //   }  
  //   $filec = file_get_contents($GLOBALS['argv'][1]);
  
    return $filec;
  }

  function processEntry($log) {
    $date = $this->processDate((string)$log->date);
    $entry = new LogEntry();
    $entry->author = (string)$log->author;

    $msg = $this->processMsg((string)$log->msg);
    if($msg) {
      $entry->msgs = $msg;
    }

    $entry->files = $this->processFiles($log->paths);

    if(!isset($this->entries[$date])) {
      $this->entries[$date] = array();
    }

    array_push($this->entries[$date], $entry);
  }

  function getEntry($date) {
    foreach($this->entries as $entry) {
      if($this->processDate($date) == $entry->date) {
        return $entry;
      }
    }
    return null;
  }

  function processDate($date) {
    //"2006-03-19T01:04:36.108693Z"
    return substr($date, 0, 10);
  }

  function processMsg($logmsg) {
    //1: ignores "nomsg" and "*** empty log message ***"
    //2: break into BUGFIX, NEWS, REGULAR
    //4: remove initial "-"

    $lns = explode("\n", $logmsg);
    $lines = array();
    foreach($lns as $l) {
      array_push($lines, trim($l));
    }

    $msgs["REGULAR"] = array();
    $msgs["NEW"] = array();
    $msgs["BUGFIX"] = array();
    $msgs["DEVNULL"] = array();

    $flag = "REGULAR";
    foreach($lines as $line) {

      if(!$line) continue;

      if((strpos($line, "nomsg")!==FALSE) || (strpos($line, "*** empty log message ***")!==FALSE)) {
        continue;
      }

      if((strpos($line, "NEW")!==FALSE) || (strpos($line, "FEATURE")!==FALSE)) {
        $flag = "NEW";  
        continue;
      } else if(strpos($line, "FIX")!==FALSE) {
        $flag = "BUGFIX";
        continue;
      } else if(strpos($line, "SILENT") !== FALSE) {
        $flag = "DEVNULL";
        continue;
      }

      if(substr($line, 0, 1) == '-') {
        $line = substr($line, 1);
      }
      array_push($msgs[$flag], $line);
    }

    if((count($msgs["REGULAR"]) == 0) &&
       (count($msgs["NEW"]) == 0) &&
       (count($msgs["BUGFIX"]) == 0) ) {
      return null;
    } else {
      return $msgs;
    }
  }

  function processFiles($files) {
    if(!count($files->path)) {
      return null;
    }
    
    $ret = array();
    foreach($files->path as $file) {
      array_push($ret, $file);
    }
    return $ret;
  }

  function dumpChangeLog() {
    $printedMsgs = array();

    foreach($this->entries as $date => $logs) {

      if(!$this->hasClMessages($logs)) {
        //don't print this day
        continue;
      }

      echo "$date";
      
      foreach($logs as $log) {
        if(!$log->msgs) {
          continue;
        }

        if(!count($log->files)) {
          continue;
        }

        echo "\n\n";
        foreach($log->files as $file) {
          echo "\t* $file:\n";
        }        

        if(count($log->msgs['REGULAR'])) {
          foreach($log->msgs['REGULAR'] as $reg) {
            if(!array_search($reg, $printedMsgs)) {
              $this->printMsg($reg, "\n\t-");
              array_push($printedMsgs, $reg);
            }
          }
          $printedMsgs = array();
        }

        if(count($log->msgs['NEW'])) {
          foreach($log->msgs['NEW'] as $new) {            
            $this->printMsg($new, "\n\t-");
          }
        }

        if(count($log->msgs['BUGFIX'])) {
          foreach($log->msgs['BUGFIX'] as $new) {            
            $this->printMsg($new, "\n\tFix: ", true);
          }
        }
      }
      echo "\n\n";      
    }
  }

  function hasClMessages($logs) {
    //check if has any kind of message
    foreach($logs as $log) {
      if($log->msgs != null) {
        return true;  
      }
    }
    return false;
  }

  function printMsg($msg, $indent, $keepIndent = false) {
    //messages > 60 should be exploded in multiple lines
    if(strlen($msg) < 60) {
      echo $indent.$msg;
    } else {
      $lines  = array();
      $words = explode(' ', $msg);
      $num = count($words);
      while(1) {
        $line = implode(' ', array_slice($words, 0, $num));
        if(strlen($line) > 60) {
          $num--;
        } else {
          array_push($lines, $line);
          $words = array_slice($words, $num);
          $num = count($words);
          if(!count($words)) break;
        }
      }
      
      $first = $indent;
      if(!$keepIndent) {
        $other = substr($indent, 0, strlen($indent)-1);
      } else {
        $other = $indent;
      }     
      $id = $first;
      foreach($lines as $line) {
        echo $id.$line;
        $id = $other;
      }
    } 
  }
}


  $cl = new ChangeLog();
  $cl->dumpChangeLog();
?>