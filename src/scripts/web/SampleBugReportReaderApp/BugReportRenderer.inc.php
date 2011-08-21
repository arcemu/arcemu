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


//////////////////////////////////////////
//class BugReportRenderer
//  Renders a single bugreport to HTML
//
//
//////////////////////////////////////////
class BugReportRenderer{
	
	//////////////////////////////////////////////////////////////
	//public function Render( $arr )
	//  Renders a single bugreport to HTML
	//
	//Parameters
	//  $arr  -  An array containing the fields of the bugreport
	//
	//Return Value
	//  None.
	//
	/////////////////////////////////////////////////////////////
	public function Render( $arr ){
		echo "<html>";
		echo "<head><title>Arcemu bug reports</title></head>";
		echo "<body>";
		echo "<table border=\"1\">";
		echo "<tr><td>field</td><td>value</td></tr>";
		
		echo "<tr><td>uid</td><td>$arr[0]</td></tr>";
		echo "<tr><td>Account ID</td><td>$arr[1]</td></tr>";
		
		$date = date( "M j G:i:s T Y", $arr[2] );
		echo "<tr><td>Submission Date</td><td>$date</td></tr>";
		
		$typefields = $arr[3];
		
		echo "<tr><td>Object</td><td>$typefields[43]</td></tr>";
		echo "<tr><td>Who</td><td>$typefields[23]</td></tr>";
		echo "<tr><td>Where</td><td>$typefields[24]</td></tr>";
		echo "<tr><td>When</td><td>$typefields[25]</td></tr>";
		echo "<tr><td>Issue</td><td>$typefields[41]</td></tr>";
		
		$contentfields = $arr[4];
		
		$map = $contentfields["Map"];
		echo "<tr><td>Map</td><td>$map</td></tr>";
		
		$zone = $contentfields["Zone"];
		echo "<tr><td>Zone</td><td>$zone</td></tr>";
		
		$coordinates = $contentfields["Position"];
		echo "<tr><td>Coordinates</td><td>$coordinates</td></tr>";
		
		echo "</table>";
		echo "</body>";
		echo "</html>";
	}
}


?>
