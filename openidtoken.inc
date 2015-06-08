<?php
namespace openid;

class OpenIDToken {
	public $header = null;
	public $payload = null;
	public $signature = null;
	public function __construct($tokenString=null) {
		if ( $tokenString != null ) {
			$token_data  = explode(".",$tokenString);
			$this->header = json_decode(base64_decode($token_data[0]));
			$this->payload = json_decode(base64_decode($token_data[1]));
			$this->signature = $token_data[2];
		}
	}

}
