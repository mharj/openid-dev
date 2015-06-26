<?php
namespace openid;

class OpenIDLog {
	private static $instance = null;
	private static $loggerClass = "openid\OpenIDNullLogger";
	public static function setLogFactory($class="openid\OpenIDNullLogger")  {
		self::$loggerClass = $class;
	}
	public static function debug($msg) {
		if (is_null(self::$instance)) { self::$instance = new self::$loggerClass();	}
		self::$instance->debug($msg);	
	}

	public static function error($msg) {
		if (is_null(self::$instance)) { self::$instance = new self::$loggerClass();	}
		self::$instance->error($msg);	
	}

	public static function getInstance() {
		return self::$instance;	
	}
}
