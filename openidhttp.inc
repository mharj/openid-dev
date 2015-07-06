<?php
namespace openid;
class OpenIDHttp {
	const CONNECT  ='CONNECT';
	const DELETE ='DELETE';
	const GET='GET';
	const HEAD='HEAD';
	const OPTIONS='OPTIONS';
	const PATCH='PATCH';
	const POST='POST';
	const PUT='PUT';
	const TRACE='TRACE';
	
	private $url;
	private $method;
	private $headers;
	private $post;
	private $proxy;
	private $ch;
	
	/**
	 * HTTP class constructor
	 * @param type $url query url
	 * @param type $method query method
	 * @param array $headers query headers
	 * @param type $post query post data
	 * @throws \Exception if no curl functions found
	 */
	public function __construct( $url , $method=self::GET , array $headers=null , $post=null) {
		OpenIDLog::debug(__METHOD__,func_get_args());
		if ( ! function_exists("curl_init") ) {
			throw new \Exception("PHP missing curl module");
		}
		$this->ch = curl_init($url);
		if ( $this->ch === false ) {
			throw new \Exception("curl_init error");
		}
		$this->url = $url;
		$this->setHeaders($headers);
		$this->setPostData($post);
		$this->setMethod($method);
		// default settings for curl
		curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($this->ch, CURLOPT_TIMEOUT, 10);
		curl_setopt($this->ch, CURLOPT_FAILONERROR , false);		
	}
	
	/**
	 * delete curl handle
	 */
	public function __destruct() {
		OpenIDLog::debug(__METHOD__,func_get_args());
		curl_close($this->ch);
	}
	
	/**
	 * set query method
	 * @param string $method self::const method type (string)
	 */
	public function setMethod($method) {
		OpenIDLog::debug(__METHOD__,func_get_args());
		if ( $method == self::POST ) {
			curl_setopt($this->ch, CURLOPT_POST, 1);
		}		
		if ( $method != null ) {
			curl_setopt($this->ch, CURLOPT_CUSTOMREQUEST, $method);		
		}
		$this->method = $method;
	}
	
	/**
	 * set query header data
	 * @param array $headers array of header data
	 */
	public function setHeaders(array $headers) {
		OpenIDLog::debug(__METHOD__,func_get_args());
		if ( $headers != null ) {
			curl_setopt($this->ch, CURLOPT_HTTPHEADER,$headers);
		}
		$this->headers = $headers;
	}
	
	/**
	 * set POST method data
	 * @param mixed $post mixed POST data (string or array)
	 */
	public function setPostData($post) {
		OpenIDLog::debug(__METHOD__,func_get_args());
		if ( $post != null ) {
			if ( is_array($post) ) {
				curl_setopt($this->ch, CURLOPT_POSTFIELDS, http_build_query($post) );
			} else {
				curl_setopt($this->ch, CURLOPT_POSTFIELDS, $post);
			}
		}
		$this->post = $post;
	}
	
	/**
	 * set HTTP proxy for curl
	 * TODO: build proxy class
	 * @param string $proxy http proxy string 
	 */
	public function setProxy(OpenIDHttpProxy $proxy=null) {
		OpenIDLog::debug(__METHOD__,func_get_args());
		curl_setopt($this->ch, CURLOPT_PROXY, ($proxy == null?null:$proxy->toURL()) );
		$this->proxy = $proxy;
	}
	
	/**
	 * Execute HTTP query
	 * @return string http data
	 * @throws OpenIDException if fatal curl error
	 */
	public function exec() {
		OpenIDLog::debug(__METHOD__,func_get_args());
		$return = curl_exec($this->ch);
		if( $return === false ) {
			throw new OpenIDException($this->getHttpError());
		}
		curl_close($this->ch);
		return $return;
	}
	
	/**
	 * get HTTP return code from last exec
	 * @return int HTTP return code
	 */
	public function getHttpCode() {
		OpenIDLog::debug(__METHOD__,func_get_args());
		return curl_getinfo($this->ch,CURLINFO_HTTP_CODE);
	}
	
	/**
	 * get HTTP error string from last exec
	 * @return string get last HTTP error string
	 */
	public function getHttpError() {
		OpenIDLog::debug(__METHOD__,func_get_args());
		return curl_error($this->ch);
	}
}