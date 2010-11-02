<!--

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

-->
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<title>signup page</title>
</head>

<body>

<div align="center">
  <h1>server signup</h1>
  <form name="signupform" method="post" action="dosignup.php">

<!--

<?php
//require_once('recaptchalib.php');
//$publickey = "Fill in public Recaptcha key here";
//$privatekey = "Fill in private Recaptcha key here";
//$error = null;
?>

-->

  <table width="200" border="0">
    <tr>
      <td>username:</td>
      <td><input name="username" type="text" id="username"></td>
    </tr>
    <tr>
      <td>email:</td>
      <td><input name="email" type="text" id="email"></td>
    </tr>
    <tr>
      <td>password:</td>
      <td><input name="password" type="password" id="password"></td>
    </tr>
    <tr>
      <td colspan="2">&nbsp;</td>
      </tr>
    <tr>
      <td colspan="2"><div align="center">

<!--
<?php
//echo recaptcha_get_html($publickey, $error);
?>
-->

        <input type="submit" name="Submit" value="Sign up!">
      </div></td>
      </tr>
  </table>
  </form>
  <p>&nbsp;</p>
</div>
</body>
</html>
