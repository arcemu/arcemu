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


require_once( '../lib/MYSQLDatabase.inc.php' );
require_once( 'BugReportTypeFieldParser.inc.php' );
require_once( 'BugReportContentFieldParser.inc.php' );

/////////////////////////////////////////////////////
//class BugReportLoader
//  Loads a single bugreport from the database and,
//  stores it in a list
//
////////////////////////////////////////////////////
class BugReportLoader{
	
	private $db;
	private $res;
	private $reportfields;
	
	public function BugReportLoader(){
		global $dbusername;
		global $dbpassword;
		global $dbhost;
		global $dbport;
		global $dbname;
		
		$this->reportfields = array();
		$this->db = new MYSQLDatabase( $dbusername, $dbpassword, $dbhost, $dbport, $dbname );
	
	}
	
	////////////////////////////////////////////////
	//public function Load( $uid )
	//  Loads a single bugreport from the database
	//
	//Parameters
	//  $uid  -  numeric identifier of the bugreport
	//
	//Return Value
	//  None.
	//
	////////////////////////////////////////////////
	public function Load( $uid ){
		$query = "SELECT `UID`,`AccountID`, `TimeStamp`, `Type`, `Content` FROM `playerbugreports` WHERE `UID` = '$uid';";
		
		
		$this->db->Connect();
		
		$this->res = $this->db->Query( $query );
		
		$row = $this->db->Fetch( $this->res );
		
		$this->reportfields[ 0 ] = $row[ 0 ];
		$this->reportfields[ 1 ] = $row[ 1 ];
		$this->reportfields[ 2 ] = $row[ 2 ];
		
		$typeparser = new BugReportTypeFieldParser( $row[ 3 ] );
		$typeparser->Parse();
		
		$this->reportfields[ 3 ] = $typeparser->GetArray();
		
		$contentparser = new BugReportContentFieldParser( $row[ 4 ] );
		$contentparser->Parse();
		
		$this->reportfields[ 4 ] = $contentparser->GetArray();
		
		return true;
	}
	
	///////////////////////////////////////////////////////
	//public function GetFieldArray()
	//  Returns the bugreport fields as an array
	//
	//Parameters
	//  None.
	//
	//Return Value
	//  Returns an array containing the bugreport fields
	//
	//////////////////////////////////////////////////////
	public function GetFieldArray(){
		return $this->reportfields;
	}
}


?>
