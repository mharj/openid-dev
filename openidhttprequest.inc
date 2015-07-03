<?php
namespace openid;

class OpenIDHTTPRequest {
	private $method;
	private $headers = array();
	private $postData;
	private $url;
	
	public function __construct($url,$method=OpenIDHTTPMethod::GET,$postData=null) {
		OpenIDLog::debug(__METHOD__,func_get_args());
		$this->url = $url;
		$this->method = $method;
		$this->setPayload($postData);
	}
	
	public function getMethod() {
		OpenIDLog::debug(__METHOD__,func_get_args());
		return $this->method;	
	}
	
	public function getURL() {
		OpenIDLog::debug(__METHOD__,func_get_args());
		return $this->url;	
	}
	
	public function setPayload($postData) {
		OpenIDLog::debug(__METHOD__,func_get_args());
		if ( is_array($postData) ) {
			$this->postData = http_build_query($postData);	
		} else {
			$this->postData = $postData;
		}
	}
	
	public function getPayload() {
		OpenIDLog::debug(__METHOD__,func_get_args());
		return $this->postData;
	}	
	
	public function addHeader($header) {
		OpenIDLog::debug(__METHOD__,func_get_args());
		$this->headers[]=$header;	
	}
	
	public function getHeaders() {
		OpenIDLog::debug(__METHOD__,func_get_args());
		return $this->headers;	
	}
}
