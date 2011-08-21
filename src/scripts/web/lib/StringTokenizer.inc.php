<?

///////////////////////////////////////////////
//class StringTokenizer
//  Breaks up a string into smaller parts using
//  the provided delimiter(s) as break points
//
//////////////////////////////////////////////
class StringTokenizer{

	private $NextToken;
	private $Delimiter;

	public function StringTokenizer( $str , $delim  ){
		if( $str != NULL && $delim != NULL ){
			$this->NextToken = strtok( $str, $delim );
			$this->Delimiter = $delim;
		}else{
			$this->NextToken = false;
			$this->Delimiter = NULL;
		}
	}
	
	
	///////////////////////////////////////////////////
	//public function HasNextToken()
	//  Tells if there are more tokens in the string
	//
	//Parameters
	//  None
	//
	//Return Value
	//  Returns true if there are more tokens.
	//  Returns false if there aren't.
	//
	///////////////////////////////////////////////////
	public function HasNextToken(){
		if( $this->NextToken !== false )
			return true;
		else
			return false;
	}
	
	//////////////////////////////////////////////////
	//public function GetNextToken()
	//  Returns the next token if there is such.
	//
	//Parameters
	//  None.
	//
	//Return Values
	//  Returns the next token if there is such.
	//  Returns NULL if there are no more tokens.
	//
	//////////////////////////////////////////////////
	public function GetNextToken(){
		if( !$this->HasNextToken() )
			return NULL;
		else{
			$return = $this->NextToken;
			$this->NextToken = strtok( $this->Delimiter );
				
			return $return;
		}
	}
}

?>
