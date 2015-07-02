<?php
namespace openid;
class OpenIDHttp {
	private $url;
	private $headers;
	private $post;
	private $proxy;
	
	public function __construct($url,array $headers=null,$post=null) {
		$this->url = $url;
		$this->headers = $headers;
		$this->post = $post;
	}
	
	public function setHeaders(array $headers) {
		$this->headers = $headers;
	}
	
	public function setPostData(array $post) {
		$this->post = $post;
	}
	
	public function setProxy($proxy) {
		$this->proxy = $proxy;
	}
	
	public function exec() {
		OpenIDLog::debug(__METHOD__,array('url'=>$this->url,'header'=>$this->header,'post'=>$this->post));
		$process = curl_init($this->url); 
		curl_setopt($process, CURLOPT_RETURNTRANSFER, 1);
		if ( $this->header != null ) {
			curl_setopt($process, CURLOPT_HTTPHEADER,$this->header);
		}
		curl_setopt($process, CURLOPT_PROXY, $this->proxy );
		if ( $this->post != null ) {
			curl_setopt($process, CURLOPT_POST, 1);
			curl_setopt($process, CURLOPT_POSTFIELDS, http_build_query($this->post) );
		}
		curl_setopt($process, CURLOPT_TIMEOUT, 10);
		curl_setopt($process, CURLOPT_FAILONERROR,false);
		$return = curl_exec($process);
		if( $return === false ) {
			throw new OpenIDException(curl_error($process));
		}
		curl_close($process);
		return $return;
	}
}