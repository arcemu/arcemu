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


require_once('../lib/MYSQLDatabase.inc.php');

/////////////////////////////////////////////////////
//class BugReportListLoader
//  Loads the bugreport list from the database and,
//  stores it in an array
//
////////////////////////////////////////////////////
class BugReportListLoader{
	private $db;
	private $res;
	private $reportlist;
	private $query;
	private $count;
	
	public function BugReportListLoader(){
		global $dbusername;
		global $dbpassword;
		global $dbhost;
		global $dbport;
		global $dbname;
		
		$this->reportlist = array();
		$this->count = 0;
		$this->db = new MYSQLDatabase( $dbusername, $dbpassword, $dbhost, $dbport, $dbname );
		$this->query = "SELECT `UID`,`TimeStamp` FROM `playerbugreports` WHERE `Suggestion` = '0' ORDER BY `UID` DESC LIMIT 50;";
	}
	
	/////////////////////////////////////////
	//public function Load()
	//  Loads the bugreport list
	//
	//Parameters
	//  None.
	//
	//Return Value
	//  None.
	//
	////////////////////////////////////////
	public function Load(){
		$this->db->Connect();
		$this->res = $this->db->Query( $this->query );
		
		while( ( $row = $this->db->Fetch( $this->res ) ) !== false ){
			$this->reportlist[ $this->count ] = $row;
			$this->count++;
		}
	}
	
	////////////////////////////////////////////
	//public function GetList()
	//  Returns the bugreport list as an array
	//
	//Parameters
	//  None.
	//
	//Return Value
	//  Returns the bugreport list as an array
	//
	///////////////////////////////////////////
	public function GetList(){
		return $this->reportlist;
	}
}

?>
