<?php
namespace openid;

class OpenIDLog {
	private static $instance = null;
	private static $loggerClass = "openid\OpenIDNullLogger";
	public static function setLogFactory($class="openid\OpenIDNullLogger")  {
		self::$loggerClass = $class;
	}
	public static function clear() {
		if (is_null(self::$instance)) { self::$instance = new self::$loggerClass();	}
		self::$instance->clear();	
	}
	
	public static function debug($msg,$object=null) {
		if (is_null(self::$instance)) { self::$instance = new self::$loggerClass();	}
		self::$instance->debug($msg,$object);	
	}

	public static function error($msg,$object=null) {
		if (is_null(self::$instance)) { self::$instance = new self::$loggerClass();	}
		self::$instance->error($msg,$object);	
	}

	public static function getInstance() {
		return self::$instance;	
	}
}
