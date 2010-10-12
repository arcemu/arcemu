<?php
 include_once("include/config.inc.php");
  
//database connection 
$dbi = mysql_connect($dbhost, $dbuname, $dbupass,true) or die("Couldn't connect to database server!");
mysql_select_db($dbname, $dbi) or die("Q 200603201239");
?>