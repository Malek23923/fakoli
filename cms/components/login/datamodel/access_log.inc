<?php

// simple little class to log all attempts to login

class AccessLog extends DataItem
{
	var $fields = array(
			"access_id"			=>	Number,
			"user_id"			=>	Number,
			"timestamp"			=>	DateTime,
			"action"			=>	String,
			"username"			=>	String,
			"password"			=>	String,
			"ip"				=>	String,
	);
	

	function AccessLog()
	{
		$this->primary_key = "access_id";
		$this->table = "access_log";
		
		$this->DataItem(func_get_args());
	}
}
