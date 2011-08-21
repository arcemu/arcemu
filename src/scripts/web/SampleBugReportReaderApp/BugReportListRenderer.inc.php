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


////////////////////////////////////////////
//class BugReportListRenderer
//  Renders the bugreport list to HTML
//
//
///////////////////////////////////////////
class BugReportListRenderer{
	
	
	/////////////////////////////////////////////////////////////
	//public function Render( $bugreportlist )
	//  Renders the bugreport list to HTML
	//
	//Parameters
	//  $bugreportlist  -  An array containing the bugreports
	//
	//Return Value
	//  None.
	//
	////////////////////////////////////////////////////////////
	public function Render( $bugreportlist ){
		echo "<html>";
		echo "<head><title>Arcemu bug reports</title></head>";
		echo "<body>";
		echo "<table border=\"1\">";
		
		echo "<tr>";
		echo "<td>uid</td>";
		echo "<td>date of submission</td>";
		echo "<td>link</td>";
		echo "</tr>";
		
		foreach( $bugreportlist as $v ){
			$id = $v[ 0 ];
			$date = date( "M j G:i:s T Y", $v[ 1 ] );
			
			echo "<tr>";
			echo "<td> $id </td>";
			echo "<td> $date </td>";
			echo "<td> <a href=\"bugreport.php?uid=$id\">view</a></td>";
			echo "</tr>";
		}
		echo "</table>";
		echo "</body>";
		echo "</html>";
	}
}

?>

