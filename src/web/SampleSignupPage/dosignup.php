<?php
/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


/////////////////////////// whoever comes here gets logged ////////////////////
$fp = fopen( "iplog.txt", "a" );
fprintf($fp, "%s\t%s\n", date("m-d-Y H:m:s"), $_SERVER["REMOTE_ADDR"] );
fclose($fp);
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////// sanity checks ////////////////////////

/*
$referrer =  $_SERVER['HTTP_REFERER'];

if( strstr( $referrer, "arcemu.org" ) == FALSE ){
  header("Location:htm/err_unknown.html");
  exit;
}
*/

$username = $_POST['username'];
$email    = $_POST['email'];
$password = $_POST['password'];

if( $username == '' || $password == '' || $email == '' ){
  header("Location:htm/err_nodata.html");
  exit;
}

$domain = strstr( $email, '@' );
$tld    = strstr( $domain, '.' );

if( $domain == FALSE || $tld == FALSE ){
  header("Location:htm/err_email.html");
  exit;
}
 
/*
require_once('recaptchalib.php');

$publickey = "Fill in public Recaptcha key here";
$privatekey = "Fill in private Recaptcha key here";
  
$resp = recaptcha_check_answer ($privatekey,
                    $_SERVER["REMOTE_ADDR"],
                    $_POST["recaptcha_challenge_field"],
                    $_POST["recaptcha_response_field"]);
                    
if( !$resp->is_valid ){
  header("Location:htm/err_captcha.html");
  exit;
}
*/
                    
//////////////////////////////////////////////////////////////////////////////

include( 'Config.inc.php' );  
include( '../lib/MYSQLDatabase.inc.php' );

$db = new MYSQLDatabase( $dbusername, $dbpassword, $dbhost, $dbport, $dbname );

if( $db->Connect() == false ){
  $db->Close();
  header("Location:htm/cantconnect.html"); 
  exit;
}

$encrypted_password = sha1( strtoupper( $username.":".$password ) );

$myquery = "INSERT INTO `accounts` (`login`,`password`,`encrypted_password`,`gm`,`flags`,`banned`,`email`) VALUES('$username','','$encrypted_password','3','24','0','$email')";

$myresult = $db->Query( $myquery );

if( $myresult == true){
  $db->Close();
  header("Location:htm/success.html"); 
  exit;
}
else{
  $db->Close();
  header("Location:htm/failed.html"); 
  exit;
}

?>
