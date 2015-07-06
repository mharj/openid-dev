<?php
namespace openid;

class OpenIDAccessToken {
	private static $tokens = array();
	private $url;
	private $code;
	private $scope;
	private $sessionKey;
	private function __construct($key,$url,$code,$scope) {
		$this->sessionKey = '__OPENID_CODE_'.$key;
		$this->url = $url;
		$this->code = $code;
		$this->scope = $scope;
	}
	
	
	private function getToken() {
		
		
	}
	
	public static function requestToken($url,$code,$scope) {
		// TODO:replace with URL class some point
		$key = preg_replace("/^https:\/\//","",$url);
		$key = preg_replace("/\/.*?$/","",$key);
		$key = preg_replace("/\./","_",$key);
        if (! isset(self::$instance[$class]) ) {
        	self::$instance[$key] = new $class($key,$code,$openIdConfig);
        }
        $current = self::$instance[$key];
        
        return $current->getToken();
	}
}