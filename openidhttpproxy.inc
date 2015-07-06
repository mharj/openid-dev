<?php
namespace openid;
class OpenIDHttpProxy {
	private $server;
	private $port;
	/**
	 * Simple Proxy wrapper
	 * @param type $server proxy server
	 * @param type $port proxy port
	 */
	public function __construct($server=null,$port=8080) {
		$this->server = $server;
		$this->port = $port;
	}
	
	public function setServer($server) {
		$this->server = $server;
	}
	
	public function getServer() {
		return $this->server;
	}
	
	public function setPort($port) {
		$this->port = $port;
	}
	
	public function getPort() {
		return $this->port;
	}
	
	public function toURL() {
		if ( $this->server == null ) {
			return null;
		}
		return 'http://'.$this->server.":".$this->port;
	}
}