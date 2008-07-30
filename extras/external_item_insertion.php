<?php
/**
 * Example External Item Insertion Script
 * For Ascent 2.1
 * @author Burlex
 * @copyright Burlex, 2007, We love goats!
 */

/* setting area */
$worlddb_host = "localhost";
$worlddb_user = "moocow";
$worlddb_pass = "moo";
$worlddb_db = "world";
$chardb_host = "localhost";
$chardb_user = "moocow";
$chardb_pass = "moo";
$chardb_db = "character";
$item_to_insert_id = 51;
$player_guid = 41642;
$cash = 1;
$stationary = 61;		// 41 is normal, 61 = blizzard

$wdb_conn = mysql_connect($worlddb_host, $worlddb_user, $worlddb_pass) or die(mysql_error());
mysql_select_db($worlddb_db, $wdb_conn) or die(mysql_error($wdb_conn));

$query = mysql_query("SELECT MaxDurability FROM items WHERE entry = " . $item_to_insert_id, $wdb_conn) or die(mysql_error($wdb_conn));
if(mysql_num_rows($query) == 0)
	die("no rows");

$result = mysql_fetch_array($query);
$item_durability = $result[0];
mysql_free_result($query);
mysql_close($wdb_conn);

$cdb_conn = mysql_connect($chardb_host, $chardb_user, $chardb_pass) or die(mysql_error());
mysql_select_db($chardb_db, $cdb_conn) or die(mysql_error($cdb_conn));

$query = mysql_query("SELECT MAX(guid) FROM playeritems_external", $cdb_conn) or die(mysql_error($cdb_conn));
$result = mysql_fetch_array($query);
$itemguid = $result[0] + 1;
mysql_free_result($query);

$query = mysql_query("SELECT MAX(message_id) FROM mailbox", $cdb_conn) or die(mysql_error($cdb_conn));
$result = mysql_fetch_array($query);
$messageid = $result[0] + 1;
mysql_free_result($query);

$messageq = sprintf("INSERT INTO mailbox VALUES(%u, 0, %u, %u, \"Test Message\", \"Test\", %u, 0, %u, 0, %u, 0, %u, 0, 0, 0)",
		$messageid, $player_guid, $player_guid, $cash, $itemguid, $stationary, time());

if(!mysql_query($messageq, $cdb_conn))
	die(mysql_error($cdb_conn));

$itemq = sprintf("INSERT INTO playeritems_external VALUES(0, %u, %u, 0, 1, 0, 0, 0, 0, 0, %u, -1, -1, '')", $itemguid, $item_to_insert_id, $item_durability);
if(!mysql_query($itemq, $cdb_conn))
	die(mysql_error($cdb_conn));

mysql_close($cdb_conn);

?>
