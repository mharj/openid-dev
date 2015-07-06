<?php
namespace openid;

abstract class OpenIDService implements OpenIDServiceInterface {
	const SERVICE_URI = null;
	private $code = null;
	private static $instance = array();
	private $accessToken = null;
	private $openIdConfig = null;
	
	protected function __construct($code,$openIdConfig) {
		OpenIDLog::debug(get_called_class().'::__construct',array("code"=>$code));
		$this->code = $code;
		$this->openIdConfig = $openIdConfig;
	}
	
	public static function getInstance($code,$openIdConfig) {
		OpenIDLog::debug(__METHOD__." = ".get_called_class());
		$class = get_called_class();
        if (! isset(self::$instance[$class]) ) {
        	self::$instance[$class] = new $class($code,$openIdConfig); 
        }
#        echo "<pre>";
#        print_r(self::$instance);
#		echo "</pre>";
        return self::$instance[$class];
	}
	
	public function haveTokens() {
		OpenIDLog::debug(get_called_class().'::haveTokens'." = ".($this->accessToken !== null?"true":"false"));
		return ($this->accessToken !== null);	
	}
	
	public function setTokens($accessToken) {
		OpenIDLog::debug(get_called_class().'::setTokens',$accessToken);
		$this->accessToken = $accessToken;	
	}
	
	protected function httpQuery($URL,array $headers,$post,$method=null) {
		OpenIDLog::debug(get_called_class().'::httpQuery'."('".$URL."')");
		if ( $this->accessToken == null ) {
				throw new OpenIDException("Missing access token!");
		}
		$headers[]='Authorization: '.$this->accessToken->token_type.' '.$this->accessToken->access_token;
		$data = $this->curlLoader($URL,$headers,$post,$method);
		return $data;
	}
	
	// curl wrapper
	private function curlLoader($url,array $header=null,$post=null,$method=null) {
		OpenIDLog::debug(get_called_class().'::curlLoader',func_get_args());
		$process = curl_init($url); 
		curl_setopt($process, CURLOPT_RETURNTRANSFER, 1);
		if ( $method != null ) {
			curl_setopt($process, CURLOPT_CUSTOMREQUEST, $method);		
		}
		if ( $header != null ) {
			curl_setopt($process, CURLOPT_HTTPHEADER,$header);
		}
		curl_setopt($process, CURLOPT_PROXY, $this->openIdConfig->getHttpProxy() );
		if ( $post != null ) {
			curl_setopt($process, CURLOPT_POST, 1);
			if ( is_array($post) ) {
				curl_setopt($process, CURLOPT_POSTFIELDS, http_build_query($post) );
			} else {
				curl_setopt($process, CURLOPT_POSTFIELDS, $post);
			}
		}
		curl_setopt($process, CURLOPT_TIMEOUT, 10);
		curl_setopt($process, CURLOPT_FAILONERROR,false);
		$return = curl_exec($process);
		curl_close($process);
		if( $return === false ) {
			throw new OpenIDException(curl_error($process));
		}
		return $return;
	}
	
	protected function httpLoader(OpenIDHTTPRequest $req) {
		OpenIDLog::debug(get_called_class().'::httpLoader',func_get_args());
		$req->addHeader('Authorization: '.$this->accessToken->token_type.' '.$this->accessToken->access_token);
		return $this->curlLoader( $req->getURL() , $req->getHeaders() , $req->getPayload() , $req->getMethod() );
	}
} 
